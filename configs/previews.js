const { loadPreviews, savePreviews } = require('../utils/preview-cache.js');
const { scrape } = require('../utils/scrape');
const html = require('../utils/html.js');

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
}

module.exports = {
    previewConfig
}
