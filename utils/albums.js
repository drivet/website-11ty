const slugify = require('slugify');

function albumImageUrl(albumPath, index) {
  const indexSlug = `${index}`.padStart(6, '0');
  return `${albumPath}/${indexSlug}`;
}

function albumNavigation(root, data) {
  if (!data.parent) {
    return undefined;
  }
  const parentList = data.parent.slice().reverse();
  let path = root;
  const navList = parentList.map(p => {
    const slug = slugify(p.title, { lower: true, strict: true } );
    path += '/' + slug;
    return {
      title: p.title,
      description: p.description,
      permalink: path
    };
  });
  return navList;
}

module.exports = {
  albumImageUrl,
  albumNavigation
}
