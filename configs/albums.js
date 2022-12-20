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
    albumNavigation: album.data.albumNavigation
  };
}

function albumToImagePosts(album) {
  const albumPath = makePermalink(album);
  return album.data.photo.map((p, i) => albumPhotoPost(album, albumPath, i, p));
}

function findOrCreateNode(rootUrl, parent, albumMap) {
  if (albumMap.has(parent.title)) {
    return albumMap.get(parent.title);
  } else {
    const slug = slugify(parent.title, { lower: true, strict: true } );
    const newNode = {
      permalink: `${rootUrl}/${slug}`,
      title: parent.title,
      description: parent.description,
      featured: parent.featured,
      albumMap: new Map()
    };
    albumMap.set(parent.title, newNode);
    return newNode;
  }
}

function albumTree(collection) {
  const rootUrl = '/albums';

  const albumMap = new Map();
  
  const root = {
    permalink: rootUrl,
    title: "Albums",
    description: "All my albums",
    albumMap: new Map()
  };

  albumMap.set(root.title, root);

  collection.forEach(album => {
    const parents = (album.data.parent || []).slice().reverse();
    let context = root;
    parents.forEach(p => {
      node = findOrCreateNode(context.permalink, p, albumMap);
      context.albumMap.set(node.title, node);
      context = node;
    });

    // handle actual album
    const albumEntry = {
      permalink: album.url,
      title: album.data.title,
      featured: album.data.featured,
      photos: album.data.photo.length
    }
    context.albumMap.set(albumEntry.title, albumEntry);
  });

  albumMap.forEach((v,k) => v.albums = Array.from(v.albumMap.values()));
  const albums = Array.from(albumMap.values());
  return albums;
}

function addAlbumCollections(eleventyConfig) {
  eleventyConfig.addCollection('albumImages', (collection) => {
    const albums = postTypes(getPosts(collection), ['album']);
    return _.flatten(albums.map(albumToImagePosts));
  });
  
  eleventyConfig.addCollection('albumTree', (collection) => {
    const albums = postTypes(getPosts(collection), ['album']);
    return albumTree(albums);
  });
}

module.exports = {
  addAlbumCollections,
}
