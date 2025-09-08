const html = require('./utils/html.js');
const { dateFormat, makePermalink } = require('./utils/helpers.js');
const _ = require('lodash');
const rootUrl = require('./src/_data/global.json').URL;
const syntaxHighlight = require("@11ty/eleventy-plugin-syntaxhighlight");
const yaml = require("js-yaml");
const pluginRss = require("@11ty/eleventy-plugin-rss");
const { addAllCollectionGroups } = require('./configs/collections');
const { previewConfig } = require('./configs/previews.js');
const { imageConfig } = require('./configs/image.js');
const eleventyNavigationPlugin = require("@11ty/eleventy-navigation");
const { enhanceNavigation, photoUrlSlug } = require('./configs/albums.js');
const { inspect } = require('util');
const { searchPostsInit, searchPostsIdx, idxJson } = require('./configs/search');
const { discussionConfig } = require('./configs/discussion.js');
const { cacheBustConfig } = require('./configs/cachebust.js');

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
  } else if (url.startsWith('https://indieweb.social')) {
    return 'fab fa-mastodon';
  } else {
    return 'fas fa-external-link-alt';
  }
}

function albumImageUrl(albumPath, photoUrl) {
  const slug = photoUrlSlug(photoUrl);
  return `${albumPath}/${slug}`;
}

module.exports = (eleventyConfig) => {
  const fs = require('fs');
  const url = require('url');
  eleventyConfig.setServerOptions({
		module: "@11ty/eleventy-server-browsersync",
    server: {
      baseDir: '_site',
      middleware: [
        function (req, res, next) {
          let file = url.parse(req.url);
          file = file.pathname;
          file = file.replace(/\/+$/, ''); // remove trailing hash
          file = `_site/${file}.html`;
  
          if (fs.existsSync(file)) {
            const content = fs.readFileSync(file);
            res.write(content);
            res.writeHead(200);
            res.end();
          } else {
            return next();
          }
        },
      ],
    },
    callbacks: {
      ready: function (_, bs) {
        bs.addMiddleware('*', (_, res) => {
          const content = fs.readFileSync('_site/404.html');
          res.write(content);
          res.writeHead(404);
          res.end();
        });
      },
    },
	});

  const markdownIt = require("markdown-it");
  const options = {
    html: true,
    linkify: true
  };
  const markdown = markdownIt(options);
  eleventyConfig.setLibrary("md", markdown);
  eleventyConfig.addFilter('markdown', function(value) {
    return markdown.render(value);
  });

  eleventyConfig.addPlugin(syntaxHighlight);
  eleventyConfig.addPlugin(pluginRss);
  eleventyConfig.addPlugin(eleventyNavigationPlugin);
  eleventyConfig.addDataExtension('yaml', contents => yaml.load(contents));
  eleventyConfig.addFilter('excerpt', content => html.excerpt(content, 500));
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

  eleventyConfig.addFilter("postPermalink", page => `${makePermalink(page, false)}.html`);
  eleventyConfig.addFilter("date", d => dateFormat(d, 'MMM D, YYYY, h:mm A Z'));
  eleventyConfig.addFilter("syndicationsForUrl", sydicationsForUrl);

  eleventyConfig.addFilter('stripExt', obj => {
    return obj.replace(/\.[^/.]+$/, '');
  });

  eleventyConfig.addFilter('clamp', (arr, limit) => {
    return arr ? arr.slice(0, limit) : null;
  });
  
  eleventyConfig.addFilter('inspect', obj => inspect(obj));

  eleventyConfig.addFilter('aUrl', url => {
    if (url.startsWith('/')) {
      return `${rootUrl}${url}`;
    } else {
      return `${url}`;
    }
  });

  eleventyConfig.addFilter('isArray', obj => Array.isArray(obj));
  
  eleventyConfig.addFilter('albumImageUrl', albumImageUrl);

  eleventyConfig.addFilter('albumTitle', (title, parents) => {
    if (!parents || parents.length === 0) {
      return title;
    } else {
      const parentTitles = parents.map(p => p.title).join(' / ')
      return `${parentTitles} / ${title}`;
    }
  });

  eleventyConfig.addFilter('enhanceNavigation', enhanceNavigation);
  eleventyConfig.addFilter("searchPostsInit", searchPostsInit);
  eleventyConfig.addFilter("searchPostsIdx", searchPostsIdx);
  eleventyConfig.addFilter("idxJson", idxJson);
  
  eleventyConfig.addFilter("debug", obj => JSON.stringify(obj));

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
    "woff2",
    "js",
    "ps"
  ]);
  
  previewConfig(eleventyConfig);
  addAllCollectionGroups(eleventyConfig);
  imageConfig(eleventyConfig);
  discussionConfig(eleventyConfig);
  cacheBustConfig(eleventyConfig);

  return {
    dir: {
        input: 'src'
    },
    htmlTemplateEngine: 'njk',
    markdownTemplateEngine: 'njk'
  };
};
