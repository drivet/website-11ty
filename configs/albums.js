const _ = require('lodash');
const slugify = require('slugify');
const { postTypes, getPosts, makePermalink } = require('../utils/helpers.js');
const { inspect } = require('util');

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
    prevLink
  };
}

function albumToImagePosts(album) {
  const albumPath = makePermalink(album);
  return album.data.photo.map((p, i) => albumPhotoPost(album, albumPath, i, p));
}

function findOrCreateNode(parent, albums) {
  const match = albums.find(a => a.title === parent.title);
  if (match) {
    return match;
  } else {
    const slug = slugify(parent.title);
    const purl = parent.url || '';
    const newNode = {
      url: `${purl}${slug}/`,
      title: parent.title,
      description: parent.description,
      albums: []
    };
    albums.push(newNode);
    return newNode;
  }
}

function albumTree(collection) {
  const root = {
    url: '/',
    title: "Albums",
    description: "All my albums",
    albums: []
  };

  collection.forEach(album => {
    const parent = album.data.parent || [];
    let context = root;
    parent.forEach(p => {
      context = findOrCreateNode(p, context.albums);
    });
    context.albums.push({ url: album.url });
  });

  return root;
}

function addAlbumCollections(eleventyConfig) {
  eleventyConfig.addCollection('albumImages', (collection) => {
    const albums = postTypes(getPosts(collection), ['album']);
    const albumTreeCol = albumTree(albums);
    console.log(`DDDD ${JSON.stringify(albumTreeCol)}`);
    return _.flatten(albums.map(albumToImagePosts));
  });
  
/*
  eleventyConfig.addCollection('albumTree', (collection) => {

  });
  */
}

module.exports = {
  addAlbumCollections,
}
