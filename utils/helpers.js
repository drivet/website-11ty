const slugify = require('slugify');
const dayjs = require('dayjs');
const utc = require('dayjs/plugin/utc');
const timezone = require('dayjs/plugin/timezone');
dayjs.extend(utc);
dayjs.extend(timezone);

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

function dateFormat(date, format) {
  return dayjs(date).tz('America/Montreal').format(format);
}

function makePermalink(page, addIndex) {
  const date_part = dateFormat(page.date, 'YYYY/MM/DD');
  const slug = getSlug(page.fileSlug);
  const url = `${date_part}/${slug}`;
  return addIndex ? `${url}/index` : url;
}

function postTypes(collection, postTypes) {
  return collection.filter((item) => postTypes.includes(item.data.postType));
}

function getPosts(collection) {
  return collection.getFilteredByGlob("./src/posts/feed/**/*.md").reverse();
}

function navigation(root, data) {
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
  dateFormat,
  makePermalink,
  postTypes,
  getPosts,
  navigation
}
