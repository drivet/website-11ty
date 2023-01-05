const _ = require('lodash');
const slugify = require('slugify');
const util = require('util');
const { postTypes, getPosts, makePermalink } = require('../utils/helpers.js');

function indexToSlug(index) {
  return `${index}`.padStart(6, '0');
}

function albumPhotoPost(album, albumPath, index, photoUrl) {
  const date = album.data.date;
  const slug = indexToSlug(index);
  const total = album.data.photo.length;
  const next = index < (total - 1) ? index + 1 : undefined;
  const prev = index > 0 ? index - 1 : undefined;
  const nextLink = next != undefined ? indexToSlug(next) : undefined;
  const prevLink = prev != undefined ? indexToSlug(prev) : undefined;

  return {
    permalink: `${albumPath}/${slug}`,
    photo: photoUrl,
    date,
    title: album.data.title,
    albumUrl: album.url,
    slug,
    nextLink,
    prevLink,
    navigation: album.data.navigation
  };
}

function albumToImagePosts(album) {
  const albumPath = makePermalink(album, false);
  return album.data.photo.map((p, i) => albumPhotoPost(album, albumPath, i, p));
}

function enhanceNavigation(nav, allAlbums) {
  const indexed = _.keyBy(allAlbums, a => a.data.eleventyNavigation?.key);

  nav.forEach(n => {
    const album = indexed[n.key];
    n.featured = album ? album.data.featured : undefined;
    n.content = album ? album.templateContent : undefined;
    if (n.children && n.children.length > 0) {
      enhanceNavigation(n.children, allAlbums);
    }
  });
  return nav;
}

function addAlbumCollections(eleventyConfig) {
  eleventyConfig.addCollection('albumImages', (collection) => {
    const albums = postTypes(getPosts(collection), ['album']);
    return _.flatten(albums.map(albumToImagePosts));
  });

  eleventyConfig.addCollection('albumNav', (collection) => {
    return collection.getFilteredByGlob("./src/posts/feed/albums/**/*.md").reverse();
  });
}

module.exports = {
  addAlbumCollections,
  enhanceNavigation,
}
