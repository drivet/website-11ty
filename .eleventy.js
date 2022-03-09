const pt = require('./utils/post-type.js');
const html = require('./utils/html.js');
const _ = require('lodash');
const rootUrl = require('./src/_data/global.json').URL
const sanitizeHTML = require('sanitize-html')
const { scrape } = require('./utils/scrape');
const syntaxHighlight = require("@11ty/eleventy-plugin-syntaxhighlight");
const yaml = require("js-yaml");
const { loadPreviews, savePreviews } = require('./utils/preview-cache.js');
const pluginRss = require("@11ty/eleventy-plugin-rss");
const dayjs = require('dayjs');
const utc = require('dayjs/plugin/utc')
const timezone = require('dayjs/plugin/timezone') // dependent on utc plugin
dayjs.extend(utc)
dayjs.extend(timezone)

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
  }
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
  if (/^\d\d\d\d\d\d\d\d\d\d\d\d\d\d(.*)/.test(fslug)) {
    return fslug.substring(8);
  } else {
    return fslug;
  }
}

function getPosts(collection) {
  return collection.getFilteredByGlob("./src/posts/feed/**/*.md").reverse();
}

function getYear(date) {
  return date.getFullYear().toString();
}

function getMonth(date) {
  const month = date.getMonth() + 1;
  return (month < 10 ? '0' : '') + month;
}

function getDay(date) {
  const day = date.getDate();
  return (day < 10 ? '0' : '') + day;
}

function classify(collection, tagExtractor) {
  const classified = {};
  collection.forEach((item) => {
      const tags = tagExtractor(item);
      if (tags.length === 0) {
          return;
      }

      tags.forEach(tag => {
        if (!classified[tag]) {
          classified[tag] = [];
        }
        classified[tag].push(item);
      });
  });
  return classified;
}

function classifyCounts(classified) {
  return Object.fromEntries(Object.entries(classified).map(e => [e[0], e[1].length]));
}

/**
 * 
 * Return object map of tag -> posts
 */
function indexByTag(collection) {
  return classify(collection, item => item.data.tags || []);
}

function tagList(collection) {
  const classified = classifyCounts(indexByTag(collection));
  return Object.keys(classified).sort().map(k => ({tag: k, count: classified[k]}));
}

function indexByDates(collection) {
  return classify(collection, item => [
    getYear(item.date),
    getYear(item.date) + '/' + getMonth(item.date),
    getYear(item.date) + '/' + getMonth(item.date) + '/' + getDay(item.date)
  ]);
}

/* counts of years and months for archive page */
function archiveList(collection) {
  // object keyed by yyyy, yyyy/mm, yyyy/mm/dd to counts
  const dates = classifyCounts(indexByDates(collection));

  const groupedByYear = _.groupBy(
    Object.keys(dates)
      .filter((k) => k.length == 7)
      .map((k) => ({
        year: k.substring(0, 4),
        month: k.substring(5, 7),
        count: dates[k],
      })),
    (e) => e.year
  );

  const c = Object.keys(groupedByYear)
    .sort()
    .reverse()
    .map((y) => {
      return {
        year: y,
        count: _.sumBy(groupedByYear[y], m => m.count),
        months: _.sortBy(groupedByYear[y], (e) => e.month).reverse(),
      };
    });
  return c;
}

function postTypes(collection, postTypes) {
  return collection.filter((item) => postTypes.includes(pt.postType(item)));
}

/**
 * 
 * Take a map of tags to items, return flat list of paged item objects like this:
 * { name, page, items }
 * 
 * page starts from 0
 */
function flatPaginate(indexedCollection, size) {
  const pages = [];
  for(let tagName of Object.keys(indexedCollection)) {
    const pagedItems = _.chunk(indexedCollection[tagName], size);
    for(let pageNumber = 0; pageNumber <  pagedItems.length; pageNumber++) {
      const page = {
        name: tagName,
        page: pageNumber,
        total: pagedItems.length,
        items: pagedItems[pageNumber]
      };
      pages.push(page);
    }
  }
  return pages;
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
  eleventyConfig.addFilter('truncate', str => truncate(str, 500));
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

  eleventyConfig.addFilter('jsonString', obj => {
    return JSON.stringify(obj);
  });

  eleventyConfig.addFilter('stripExt', obj => {
    return obj.replace(/\.[^/.]+$/, '');
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

  // standard collections with pagination
  eleventyConfig.addCollection("all", (collection) =>
    getPosts(collection)
  );

  eleventyConfig.addCollection("posts", (collection) =>
    postTypes(getPosts(collection), ["article", "note", "photo"])
  );
  
  eleventyConfig.addCollection("blog", (collection) =>
    postTypes(getPosts(collection), ["article"])
  );

  eleventyConfig.addCollection("bookmarks", (collection) =>
    postTypes(getPosts(collection), ["bookmark"])
  );

  // index pages
  eleventyConfig.addCollection("tagList", (collection) =>
    tagList(getPosts(collection))
  );

  eleventyConfig.addCollection("archiveList", (collection) =>
    archiveList(getPosts(collection))
  );

  // double pagination collections
  eleventyConfig.addCollection("tagPages", (collection) =>
    flatPaginate(indexByTag(getPosts(collection)), 10)
  );

  eleventyConfig.addCollection("archivePages", (collection) =>
    flatPaginate(indexByDates(getPosts(collection)), 10)
  );

  return {
    dir: {
        input: 'src'
    },
    htmlTemplateEngine: 'njk',
    markdownTemplateEngine: 'njk'
  }
};
