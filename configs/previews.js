const { loadScraped, saveScraped } = require('./scraped-cache.js');
const { unfurl } = require('unfurl.js');
const { parse_mf2, interpret_entry } = require('mf2utiljs');
const Image = require('@11ty/eleventy-img');
const slugify = require('slugify');
const clone = require('clone');

const oembedList = ['https://twitter.com', 'https://youtube.com'];


async function cachePreviewImage(parentUrl, url) {
  try {
    const stats = await Image(url, {
      widths: [500],
      outputDir: "./_site/static/previews",
      urlPath: "/static/previews",
      formats: ['jpeg'],
      cacheOptions: {
        duration: "*",
        directory: "_cache/previews",
        removeUrlQueryParams: false,
      },
    });
    return stats.jpeg[0].url;
  } catch (e) {
    console.warn(`could not process preview image from ${parentUrl} with Image`);
    return url;
  }
}

async function cacheAvatar(authorUrl, url) {
  try {
    const stats = await Image(url, {
      widths: [64],
      outputDir: "./_site/static/avatars",
      urlPath: "/static/avatars",
      formats: ['png'],
      cacheOptions: {
        duration: "*",
        directory: "_cache/avatars",
        removeUrlQueryParams: false,
      },
    });
    return stats.png[0].url;
  } catch (e) {
    console.warn(`could not process avatar image for ${authorUrl} with Image`);
    return url;
  }
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

/**
 * We just cache the avatar, but otherwise leave this more or less alone.
 * 
 * @param {*} scraped 
 * @returns 
 */
async function prepareMf2Preview(scraped) {
  const mf2Preview = clone(scraped);
  if (mf2Preview.meta?.author?.photo) {
    mf2Preview.meta.author.photo =
      await cacheAvatar(mf2Preview.meta.author.url, mf2Preview.meta.author.photo);
  }
  return mf2Preview;
}

/**
 * Nothing really to do for the omebed preview except stamp the type
 * 
 * @param {*} scaped 
 */
function prepareOembedPreview(scraped) {
  return {
    ...scraped,
    type: 'oembed'
  };
}

/**
 * We will choose a title, description and image from the open graph and twitter data,
 * based on what is available.
 * 
 * Then we will cache the preview image if there is one
 * 
 * @param {*} scraped 
 * @returns 
 */
async function prepareMetadataPreview(scraped) {
  const scrapedCopy = clone(scraped);
  const meta = scrapedCopy.meta;
  let image = null;
  if (meta?.open_graph?.images?.length > 0) {
    image = meta.open_graph.images[0];
  } else if (meta?.twitter_card?.images?.length > 0) {
    image = meta.twitter_card.images[0];
  }
  const preview = {
    ...scrapedCopy,
    meta: {
      title: meta?.open_graph?.title || meta?.twitter_card?.title,
      description: meta?.open_graph?.description || meta?.twitter_card?.description,
      image
    }
  };
  if (preview.meta.image?.url) {
    preview.meta.image.url = await cachePreviewImage(preview.url, preview.meta.image.url);
  }
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
         oembedList.some(u => url.startsWith(u));
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
}

module.exports = {
  previewConfig
}
