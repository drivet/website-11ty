const html = require('../utils/html.js');
const { unfurl } = require('unfurl.js');
const { parse_mf2, interpret_entry } = require('mf2utiljs');
const Image = require('@11ty/eleventy-img');
const slugify = require('slugify');
const clone = require('clone');
const fs = require('fs');

let scraped = null;

const CACHE_DIR = './_cache/scraped'
const OEMBED_LIST = ['https://twitter.com', 'https://youtube.com'];


function loadScraped() {
  if (scraped !== null) {
    return scraped;
  }

  console.log('scraped data not loaded, loading...');
  const filePath = `${CACHE_DIR}/scraped.json`
  if (fs.existsSync(filePath)) {
    const cacheFile = fs.readFileSync(filePath)
    scraped = JSON.parse(cacheFile);
  } else {
    console.warn('no scraped cache, initializing...');
    scraped = {};
  }
  return scraped;
}

function saveScraped() {
  console.log('saving scraped...');
  const filePath = `${CACHE_DIR}/scraped.json`
  const fileContent = JSON.stringify(scraped, null, 2);
  // create cache folder if it doesnt exist already
  if (!fs.existsSync(CACHE_DIR)) {
    fs.mkdirSync(CACHE_DIR)
  }

  fs.writeFileSync(filePath, fileContent);
}

/**
 * Pass in scraped data (i.e. the type, url and actual metadata).
 * 
 * If the type is mf2 then we cache the author avatars and return
 * the metadata otherwise unchanged.
 * 
 * If the type is unfurl, then we need to decide if we're going to use
 * oembed or not.  See useOEmbed function for details on that decision.
 * 
 * If we decide to use oembed, this is pretty easy, we just use the
 * oembed structure as is, because we really only use oembed when it's "rich".
 * 
 * Otherwise we need to massage the metadata for preview use.  We will.
 * 
 * - choose a title
 * - choose a description
 * - choose and cache an image 
 */
async function preparePreview(scraped) {
  if (!scraped) {
    return null;
  }

  if (scraped.type === 'mf2') {
    return await prepareMf2Preview(scraped);
  } else if (useOEmbed(scraped.url, scraped.meta)) {
    return prepareOembedPreview(scraped);
  } else if (useMetadata(scraped)) {
    return await prepareMetadataPreview(scraped);
  } else {
    return null;
  }
}

function truncate(str, chars, replace = '...') {
  const truncated = str.substring(0, chars);
  if (truncated.length === str.length) {
    return str;
  } else {
    const replaceLength = replace.length;
    return truncated.substring(0, chars-replaceLength) + replace;
  }
}

function extractSummary(meta) {
  if (meta.summary) {
    return meta.summary;
  } else if (meta['content-plain']) {
    return truncate(meta['content-plain'], 500);
  }
}

async function cachePreviewImage(url, widths, format) {
  try {
    const stats = await Image(url, {
      widths,
      outputDir: "./_site/static/previews",
      urlPath: "/static/previews",
      formats: [format],
      cacheOptions: {
        duration: "*",
        directory: "_cache/previews",
        removeUrlQueryParams: false,
      },
    });
    return stats[format][0].url;
  } catch (e) {
    if (url.startsWith('http:') || url.startsWith('https:')) {
      console.warn(`could not process preview image ${url}, ${JSON.stringify(e)}`);
    } else {
      console.warn(`could not process non-http URL`);
    }
    return url;
  }
}

async function cacheImages(preview) {
  if (preview?.meta?.favicon) {
    preview.meta.favicon = await cachePreviewImage(preview.meta.favicon, [64], ['png']);
  }
  if (preview?.meta?.image?.url) {
    preview.meta.image.url = await cachePreviewImage(preview.meta.image.url, [750], ['jpeg']);
  }
}

function extractPicture(meta) {
  const picture = meta?.featured || meta?.photo;
  if (picture) {
    return {
      alt: "featured image",
      url: picture
    }; 
  } else {
    return null;
  }
}

/**
 * We just cache the avatar, but otherwise leave this more or less alone.
 * 
 * @param {*} scraped 
 * @returns 
 */
async function prepareMf2Preview(scraped) {
  const scrapedCopy = clone(scraped);
  const meta = scrapedCopy.meta;
 
  const image = extractPicture(meta);
  const preview = {
    type: scrapedCopy.type,
    meta: {
      url: scrapedCopy.url,
      title: meta?.name,
      description: extractSummary(meta),
      published: meta.published || meta['published-str'],
      image,
      site: meta?.author?.name,
      siteUrl: meta?.author?.url,
      favicon: meta?.author?.photo
    }
  };
  await cacheImages(preview);
  return preview;
}

function prepareOembedPreview(scraped) {
  return {
    type: 'oembed',
    meta: {
      oEmbed: {
        html: scraped.meta.oEmbed.html
      }
    }
  };
}

/**
 * Meta has both open_graph and twitter instances.
 * Prefer open_graph, fall back to twitter
 * @param {*} meta 
 */
function select(meta, f1, f2) {
  function valueOrFirst(value) {
    return Array.isArray(value) ? value[0]: value;
  }

  if (!meta) {
    return null;
  }
  let value = null;
  if (meta.open_graph) {
    value = valueOrFirst(meta.open_graph[f1]);
  }
  if (value) {
    return value;
  }
  const twitter_att = f2 ? f2 : f1;
  if (meta.twitter_card) {
    value = valueOrFirst(meta.twitter_card[twitter_att]);
  }
  return value;
}

/**
 * Return a structure that looks like:
 * 
 * {
 *   type: unfurl,
 *   meta: {
 *     title, description, image, site, icon
 *   }
 * }
 * 
 * We will choose a title, description and image from the open graph and twitter data,
 * based on what is available.
 * 
 * Then we will cache the preview image if there is one.
 * 
 * @param {*} scraped 
 * @returns 
 */
async function prepareMetadataPreview(scraped) {
  const scrapedCopy = clone(scraped);
  const meta = scrapedCopy.meta;
 
  const preview = {
    type: scrapedCopy.type,
    meta: {
      url: scrapedCopy.url,
      title: select(meta, 'title'),
      description: select(meta, 'description'),
      image: select(meta, 'images'),
      site: select(meta, 'site_name', 'site'),
      favicon: meta?.favicon
    }
  };
  await cacheImages(preview);
  return preview;
}

/**
 * 
 * We will prefer oEmbed metadata if 
 * 
 * a) the URL is in the oembedList defined above
 * b) the oEmbed data is rich or video
 * 
 * Not every site has a good oEmbed implementation, even if they say it's rich, which
 * is why we only use it for a couple of sites.
 * @param {*} url 
 * @param {*} meta 
 * @returns 
 */
function useOEmbed(url, meta) {
  return meta.oEmbed &&
         ["rich", "video"].includes(meta.oEmbed.type) &&
         OEMBED_LIST.some(u => url.startsWith(u));
}

function useMetadata(scraped) {
  return scraped.meta?.open_graph || scraped.meta?.twitter_card;
}

/**
 * Scrape the metadata from the URL and return a structure like this:
 * {
 *   url: <the url you scraped>
 *   type: "indieweb" | "oembed" | "unfurl",
 *   meta: <either intepreted mf2 data from mf2utilsjs or unfurled data>
 * }
 * 
 * We first try to scraped microformat data from the URL and it that works, we stop, 
 * put the interpreted data in the meta field and slap an "mf2" in the type field.
 * 
 * If there's no mf2 data, we run unfurl (with oEmbed turned on) from the unfurljs
 * package, slap an "unfurl" into the type field, and the data into the meta field
 * 
 * @param {*} url 
 * @returns 
 */
async function scrape(url) {
  try {
    const parsed = await parse_mf2(url);
    const mf2 = await interpret_entry(parsed, url);
    if (mf2 && Object.keys(mf2).length > 0) {
      return { url, type: 'mf2', meta: mf2 };
    }

    const unfurled = await unfurl(url);
    if (unfurled && Object.keys(unfurled).length > 0) {
      return { url, type: 'unfurl', meta: unfurled };
    }
  } catch (error) {
    console.error(`Error scraping ${url}, ${JSON.stringify(error)}`);
  }
  return null;
}

function previewConfig(eleventyConfig) {
  /**
   * This filter will
   * 
   * a) load the cached scraped data
   * b) try and find the scraped for the key, and scrape one if it can't
   * c) massage the data and return the preview
   * 
   */
  eleventyConfig.addNunjucksAsyncFilter('preview', async (url, cb) => {
    const scraped = loadScraped();
    const key = slugify(url);
    if (!(key in scraped)) {
      console.log(`no scraped data for ${url}, trying to scrape some...`);
      scraped[key] = await scrape(url);
    }
    const result = await preparePreview(scraped[key]);
    return cb(null, result);
  });

  /**
   * Save the previews in cache after build
   */
  eleventyConfig.on('eleventy.after', () => {
    saveScraped();
  });

  eleventyConfig.addFilter('contentLink', content => {
    const links = html.links(content);
    return links[0];
  });
}

module.exports = {
  previewConfig
}
