const _ = require('lodash');
const { makePermalink, getLeafAlbums } = require('../utils/helpers.js');

function photoUrlSlug(photoUrl) {
  const hash = photoUrl.substring(photoUrl.lastIndexOf('/') + 1)
                 .replace(/\.[^/.]+$/, '');
  return hash.substring(0, Math.floor(hash.length / 4));
}

function albumPhotoPost(album, albumPath, index0, photo, slugs) {
  const date = album.data.date;
  const slug = slugs[index0];
  const total = album.data.photo.length;
  const next = index0 < (total - 1) ? index0 + 1 : undefined;
  const prev = index0 > 0 ? index0 - 1 : undefined;
  const nextLink = next != undefined ? slugs[next] : undefined;
  const prevLink = prev != undefined ? slugs[prev] : undefined;
  const title = `${album.data.title} / ${index0 + 1} of ${total}`;
  return {
    permalink: `${albumPath}/${slug}`,
    photo,
    date,
    title,
    albumUrl: album.url,
    slug,
    nextLink,
    prevLink,
    total,
    index0,
    albumKey: album.data.eleventyNavigation?.key
  };
}

function albumToImagePosts(album) {
  const albumPath = makePermalink(album, false);
  const imageSlugs = album.data.photo.map(p => photoUrlSlug(p.value));
  return album.data.photo.map((p, i) => albumPhotoPost(album, albumPath, i, p, imageSlugs));
}

function addAlbumImageCollections(eleventyConfig) {
  eleventyConfig.addCollection('albumImages', (collection) => {
    const albums = getLeafAlbums(collection);
    return _.flatten(albums.map(albumToImagePosts));
  });
}


function enhanceNavigation(nav, allAlbums) {
  const indexed = _.keyBy(allAlbums, a => a.data.eleventyNavigation?.key);
  nav.forEach(n => {
    const album = indexed[n.key];
    n.featured = album ? album.data.featured : undefined;
    n.content = album ? album.content : undefined;
    n.photo = album ? album.data.photo : undefined;
    if (n.children && n.children.length > 0) {
      enhanceNavigation(n.children, allAlbums);
    }
  });
  return nav;
}

function addAlbumFilters(eleventyConfig) {
  eleventyConfig.addFilter('albumImageUrl', (albumPath, photoUrl) => {
    const slug = photoUrlSlug(photoUrl);
    return `${albumPath}/${slug}`;
  });

  eleventyConfig.addFilter('albumTitle', (title, parents) => {
    if (!parents || parents.length === 0) {
      return title;
    } else {
      const parentTitles = parents.map(p => p.title).join(' / ')
      return `${parentTitles} / ${title}`;
    }
  });

   eleventyConfig.addFilter('enhanceNavigation', enhanceNavigation);
}

module.exports = {
  addAlbumImageCollections,
  addAlbumFilters,
}
