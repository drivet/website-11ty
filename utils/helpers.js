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

function makePermalink(page, album) {
  const date_part = dateFormat(page.date, 'YYYY/MM/DD');
  const slug = getSlug(page.fileSlug);
  return`${date_part}/${slug}`;
}

function makeAlbumPermalink(page, album) {
  const date_part = dateFormat(page.date, 'YYYY/MM/DD');
  const slug = getSlug(page.fileSlug);
  return`${date_part}/${slug}/index`;
}

module.exports = {
  dateFormat,
  makePermalink,
  makeAlbumPermalink
}
