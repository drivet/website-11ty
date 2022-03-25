const { loadPreviews, savePreviews } = require('../utils/preview-cache.js');
const html = require('../utils/html.js');
const { unfurl } = require('unfurl.js');
const { parse_mf2, interpret_entry } = require('mf2utiljs');


function normalize(meta) {
  function img(meta) {
    if (meta?.open_graph?.images?.length > 0) {
      return meta.open_graph.images[0];
    } else if (meta?.twitter_card?.images?.length > 0) {
      return meta.twitter_card.images[0];
    } else {
      return null;
    }
  }
  return {
    title: meta?.open_graph?.title || meta?.twitter_card?.title,
    description: meta?.open_graph?.description || meta?.twitter_card?.description,
    image: img(meta)
  };
}

async function scrape(url) {
  try {
    const parsed = await parse_mf2(url);
    const mf2 = await interpret_entry(parsed, url);
    if (mf2 && Object.keys(mf2).length > 0) {
      return { type: 'mf2', url, meta: mf2 };
    }

    const unfurled = await unfurl(url);
    if (unfurled && Object.keys(unfurled).length > 0) {
      return { type: 'unfurl', url, meta: unfurled };
    }
  } catch (error) {
    console.error(`Error scraping ${url}, ${JSON.stringify(error)}`);
  }
  return null;
}

function previewConfig(eleventyConfig) {
  eleventyConfig.addFilter('linkContextType', scraped => {
    if (!scraped) {
      return null;
    }
    const embedlist = ['https://twitter.com', 'https://youtube.com'];
    if (scraped.type == 'mf2') {
      return 'indieweb';
    } else if (scraped.type == 'unfurl' && scraped.meta.oEmbed &&
             (scraped.meta.oEmbed.type == "rich" || scraped.meta.oEmbed.type == "video") &&
             embedlist.some(u => scraped.url.startsWith(u))) {
      return 'oembed';
    } else if (scraped.type == 'unfurl' && (scraped.meta.open_graph || scraped.meta.twitter_card)) {
      return 'unfurl';
    } else {
      return null;
    }
  });

  let previews_changed = false;
  eleventyConfig.addNunjucksAsyncFilter('preview', async (url, key, cb) => {
    const previews = loadPreviews();
    let preview;
    if (key in previews) {
      preview = previews[key];
    } else {
      console.log(`no preview for ${url}, trying to scrape one...`);
      preview = await scrape(url);
      previews[key] = preview;
      previews_changed = true;
    }
    return cb(null, preview);
  });

  eleventyConfig.on('eleventy.after', () => {
    if (previews_changed) {
      savePreviews();
    }
  });

  eleventyConfig.addFilter('contentLink', content => {
    const links = html.links(content);
    return links[0];
  });

  eleventyConfig.addFilter('normalizeUnfurl', unfurled => normalize(unfurled));
}

module.exports = {
  previewConfig
}
