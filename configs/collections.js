const _ = require('lodash');
const { postTypes, getPosts, getRecipes, getAlbums } = require('../utils/helpers.js');
const { addAlbumCollections } = require('./albums.js');


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

  // index pages, with the tags/dates listed on them
  eleventyConfig.addCollection(`${name}_tagList`, (collection) =>
    tagList(collectionFn(collection))
  );

  eleventyConfig.addCollection(`${name}_archiveList`, (collection) =>
    archiveList(collectionFn(collection))
  );

  // double paginated collections - each tag page
  eleventyConfig.addCollection(`${name}_tagPages`, (collection) =>
    flatPaginate(indexByTag(collectionFn(collection)), 25)
  );

  eleventyConfig.addCollection(`${name}_archivePages`, (collection) =>
    flatPaginate(indexByDates(collectionFn(collection)), 25)
  );
}

function addAllCollectionGroups(eleventyConfig) {
  addCollectionGroup(eleventyConfig, "allPosts", getPosts);

  addCollectionGroup(eleventyConfig, "lifestream",
    collection => postTypes(getPosts(collection), ["article", "note", "photo", "video"]));
  
  addCollectionGroup(eleventyConfig, "articles",
    collection => postTypes(getPosts(collection), ["article"]));

  addCollectionGroup(eleventyConfig, "bookmarks",
    collection => postTypes(getPosts(collection), ["bookmark"]));
  
  eleventyConfig.addCollection("albums", getAlbums);

  addAlbumCollections(eleventyConfig);

  addCollectionGroup(eleventyConfig, "recipes", getRecipes);
}

module.exports = {
  addAllCollectionGroups,
}
