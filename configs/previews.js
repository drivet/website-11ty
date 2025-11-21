const html = require('../utils/html.js');
const clone = require('clone');
const Image = require('@11ty/eleventy-img');
const slugify = require('slugify');
const fs = require('fs');
const { preview } = require('indieweb-preview');
const debug = require('debug')('previews');

let scraped = null;

const CACHE_DIR = './_cache/scraped'

function loadCached() {
  if (scraped !== null) {
    return scraped;
  }

  debug('scraped data not loaded, loading...');
  const filePath = `${CACHE_DIR}/scraped.json`
  if (fs.existsSync(filePath)) {
    const cacheFile = fs.readFileSync(filePath)
    scraped = JSON.parse(cacheFile);
  } else {
    debug('no scraped cache, initializing...');
    scraped = {};
  }
  return scraped;
}

function saveScraped() {
  debug('saving scraped...');
  const filePath = `${CACHE_DIR}/scraped.json`
  const fileContent = JSON.stringify(scraped, null, 2);
  // create cache folder if it doesnt exist already
  if (!fs.existsSync(CACHE_DIR)) {
    fs.mkdirSync(CACHE_DIR)
  }

  fs.writeFileSync(filePath, fileContent);
}

async function cachePreviewImage(url, widths, format) {
  if (!url.startsWith("http")) {
    return undefined;
  }

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
 
    const f = Object.keys(stats)[0];
    return stats[f][0].url;

  } catch (e) {
    // debug(`could not process preview image ${url}, ${JSON.stringify(e)}`);
    // not a good check but not sure what else to do 
    // interpreting a message to mean there is an image but the plugin couldn't handle it
    return e.message ? url : undefined;
  }
}

function previewConfig(eleventyConfig) {
  /**
   * This filter will
   * 
   * a) load the cached scraped data if not already loaded
   * b) try and find the scraped for the key, and scrape one if it can't
   * c) massage the data and return the preview
   * 
   */
  eleventyConfig.addNunjucksAsyncFilter('preview', async (url, cb) => {
    const cached = loadCached();
    const key = slugify(url);
    if (!(key in cached)) {
      console.log(`no scraped data for ${url}, trying to scrape some...`);
      cached[key] = await preview(url);
    }
    return cb(null, cached[key]);
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
