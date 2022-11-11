const html = require('./utils/html.js');
const _ = require('lodash');
const rootUrl = require('./src/_data/global.json').URL;
const sanitizeHTML = require('sanitize-html');
const syntaxHighlight = require("@11ty/eleventy-plugin-syntaxhighlight");
const yaml = require("js-yaml");
const pluginRss = require("@11ty/eleventy-plugin-rss");
const { addAllCollectionGroups } = require('./configs/collections');
const { previewConfig } = require('./configs/previews.js');
const { imageConfig } = require('./configs/image.js');
const dayjs = require('dayjs');
const utc = require('dayjs/plugin/utc');
const timezone = require('dayjs/plugin/timezone');
dayjs.extend(utc);
dayjs.extend(timezone);

function webmentionsForUrl(webmentions, url) {
  if (!webmentions) {
    return [];
  }

  const absUrl = `${rootUrl}${url}`.replace(/\.[^/.]+$/, "");

  const sanitize = entry => {
    const { content } = entry;
    if (content && content['content-type'] === 'text/html') {
      content.value = sanitizeHTML(content.value);
    }
    return entry;
  };
  return webmentions
      .filter(entry => entry['wm-target'] === absUrl)
      .map(sanitize);
}

function webmentionKind(webmentions, ...kinds) {
  return webmentions.filter(entry => kinds.includes(entry['wm-property']));
}

function sydicationsForUrl(wmresults, url) {
  if (!wmresults) {
    return [];
  }
  if (url.startsWith('/')) {
    url = url.substring(1);
  }
  url = url.replace(/\.[^/.]+$/, "");
  const target_results = wmresults.results[url];
  if (!target_results) {
    return [];
  }
  return Object.entries(target_results)
               .filter(([k, v]) => k.startsWith('https://brid.gy'))
               .map(([k, v]) => v)
               .filter(v => typeof v === 'object' && v.location && v.location.length > 0)
               .map(v => v.location);
}

function synIcon(url) {
  if (url.startsWith('https://twitter.com')) {
    return 'fab fa-twitter';
  } else {
    return 'fas fa-external-link-alt';
  }
}

function getSlug(fslug) {
  // test for my note files from micropub, which look like this:
  // 20200810123845.md
  if (/^\d\d\d\d\d\d\d\d\d\d\d\d\d\d(.*)/.test(fslug)) {
    // pick up the time portion of the timestamp
    return fslug.substring(8);
  } else {
    // otherwise just return the filename
    return fslug;
  }
}

module.exports = (eleventyConfig) => {
  eleventyConfig.setBrowserSyncConfig(
    require('./configs/browsersync.config')('_site')
  );

  const markdownIt = require("markdown-it");
  const options = {
    html: true,
    linkify: true
  };
  eleventyConfig.setLibrary("md", markdownIt(options));

  eleventyConfig.addPlugin(syntaxHighlight);
  eleventyConfig.addPlugin(pluginRss);
  eleventyConfig.addDataExtension('yaml', contents => yaml.load(contents));
  eleventyConfig.addFilter('excerpt', content => html.excerpt(content, 50));
  eleventyConfig.addFilter('synicon', url => synIcon(url));
  eleventyConfig.addFilter('monthname', month => {
    switch(month) {
      case '01':
        return 'January';
      case '02':
        return 'February';
      case '03':
        return 'March';
      case '04':
        return 'April';
      case '05':
        return 'May';
      case '06':
        return 'June';
      case '07':
        return 'July';
      case '08':
        return 'August';
      case '09':
        return 'September';
      case '10':
        return 'October';
      case '11':
        return 'November';
      case '12':
        return 'December';
    }
  });

  eleventyConfig.addFilter("postPermalink", page => {
    const date_part = dayjs(page.date).tz('America/Montreal').format('YYYY/MM/DD');
    const slug = getSlug(page.fileSlug);
    return `${date_part}/${slug}.html`;
  });

  eleventyConfig.addFilter("date", d => {
    return dayjs(d).tz('America/Montreal').format('YYYY-MM-DD h:mm A Z');
  });

  eleventyConfig.addFilter("webmentionsForUrl", webmentionsForUrl);
  eleventyConfig.addFilter("webmentionKind", webmentionKind);
  eleventyConfig.addFilter("syndicationsForUrl", sydicationsForUrl);

  eleventyConfig.addFilter('jsonString', obj => {
    return JSON.stringify(obj);
  });

  eleventyConfig.addFilter('stripExt', obj => {
    return obj.replace(/\.[^/.]+$/, '');
  });

  eleventyConfig.addFilter('aUrl', obj => {
    if (obj.startsWith('/')) {
      return `${rootUrl}${obj}`;
    } else {
      return `ddd ${obj}`;
    }
  });

  eleventyConfig.setTemplateFormats([
    "md",
    "njk",
    "css",
    "jpeg",
    "jpg",
    "JPG",
    "JPEG",
    "png",
    "PNG",
    "SVG",
    "svg",
    "eot",
    "ttf",
    "woff",
    "woff2"
  ]);
  
  previewConfig(eleventyConfig);
  addAllCollectionGroups(eleventyConfig);
  imageConfig(eleventyConfig);

  return {
    dir: {
        input: 'src'
    },
    htmlTemplateEngine: 'njk',
    markdownTemplateEngine: 'njk'
  };
};
