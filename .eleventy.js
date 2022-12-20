const html = require('./utils/html.js');
const { dateFormat, makePermalink, makeAlbumPermalink } = require('./utils/helpers.js');
const _ = require('lodash');
const rootUrl = require('./src/_data/global.json').URL;
const sanitizeHTML = require('sanitize-html');
const syntaxHighlight = require("@11ty/eleventy-plugin-syntaxhighlight");
const yaml = require("js-yaml");
const pluginRss = require("@11ty/eleventy-plugin-rss");
const { addAllCollectionGroups, addAlbumImages } = require('./configs/collections');
const { previewConfig } = require('./configs/previews.js');
const { imageConfig } = require('./configs/image.js');


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

function albumImageUrl(albumPath, index) {
  const indexSlug = `${index}`.padStart(6, '0');
  return `${albumPath}/${indexSlug}`;
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

  eleventyConfig.addFilter("postPermalink", page => `${makePermalink(page)}.html`);
  eleventyConfig.addFilter("albumPermalink", page => `${makeAlbumPermalink(page)}.html`);
  eleventyConfig.addFilter("date", d => dateFormat(d, 'YYYY-MM-DD h:mm A Z'));

  eleventyConfig.addFilter("webmentionsForUrl", webmentionsForUrl);
  eleventyConfig.addFilter("webmentionKind", webmentionKind);
  eleventyConfig.addFilter("syndicationsForUrl", sydicationsForUrl);

  eleventyConfig.addFilter('jsonString', obj => {
    return JSON.stringify(obj);
  });

  eleventyConfig.addFilter('stripExt', obj => {
    return obj.replace(/\.[^/.]+$/, '');
  });

  eleventyConfig.addFilter('clamp', (arr, limit) => {
    return arr ? arr.slice(0, limit) : null;
  });

  eleventyConfig.addFilter('aUrl', obj => {
    if (obj.startsWith('/')) {
      return `${rootUrl}${obj}`;
    } else {
      return `${obj}`;
    }
  });
  
  eleventyConfig.addFilter('albumImageUrl', albumImageUrl);

  eleventyConfig.addFilter('albumTitle', (title, parents) => {
    if (!parents || parents.length === 0) {
      return title;
    } else {
      const parentTitles = parents.map(p => p.title).join(' / ')
      return `${parentTitles} / ${title}`;
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
