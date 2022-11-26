const _ = require('lodash');
const dayjs = require('dayjs');
const utc = require('dayjs/plugin/utc');
const timezone = require('dayjs/plugin/timezone');
const BenchmarkGroup = require('@11ty/eleventy/src/BenchmarkGroup');
dayjs.extend(utc);
dayjs.extend(timezone);

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

function addCollectionGroup(eleventyConfig, name, collectionFn) {
  // standard collection with pagination
  eleventyConfig.addCollection(name, (collection) =>
    collectionFn(collection)
  );

  // index pages
  eleventyConfig.addCollection(`${name}_tagList`, (collection) =>
    tagList(collectionFn(collection))
  );

  eleventyConfig.addCollection(`${name}_archiveList`, (collection) =>
    archiveList(collectionFn(collection))
  );

  // double pagination collections
  eleventyConfig.addCollection(`${name}_tagPages`, (collection) =>
    flatPaginate(indexByTag(collectionFn(collection)), 25)
  );

  eleventyConfig.addCollection(`${name}_archivePages`, (collection) =>
    flatPaginate(indexByDates(collectionFn(collection)), 25)
  );
}

function getPosts(collection) {
  return collection.getFilteredByGlob("./src/posts/feed/**/*.md").reverse();
}

function postTypes(collection, postTypes) {
  return collection.filter((item) => postTypes.includes(item.data.postType));
}

function addAllCollectionGroups(eleventyConfig) {
  addCollectionGroup(eleventyConfig, "all", getPosts);

  addCollectionGroup(eleventyConfig, "notes",
    collection => postTypes(getPosts(collection), ["note", "photo", "video", "album"]));

  addCollectionGroup(eleventyConfig, "blog",
    collection => postTypes(getPosts(collection), ["article"]));

  addCollectionGroup(eleventyConfig, "bookmarks",
    collection => postTypes(getPosts(collection), ["bookmark"]));

  // for backward compatibility
  eleventyConfig.addCollection("posts", (collection) =>
    postTypes(getPosts(collection), ["note", "photo", "video", "article", "album"])
  );
}

function albumPhotoPost(date, albumPath, index, photo) {
  const indexSlug = `${index}`.padStart(6, '0');
  return {
    permalink: `${albumPath}/${indexSlug}`,
    photo,
    date
  };
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

function albumToImagePosts(album) {
  const date_part = dayjs(album.date).tz('America/Montreal').format('YYYY/MM/DD');
  const slug = getSlug(album.fileSlug);
  const albumPath = `${date_part}/${slug}`;
  return album.data.photo.map((p, i) => albumPhotoPost(album.date, albumPath, i, p));
}

function addAlbumImages(eleventyConfig) {
  eleventyConfig.addCollection('albumImages', (collection) => {
    const albums = postTypes(getPosts(collection), ['album']);
    return _.flatten(albums.map(albumToImagePosts));
  });
}

module.exports = {
  addAllCollectionGroups,
  addAlbumImages
}
