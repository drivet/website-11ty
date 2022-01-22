const { unfurl } = require('unfurl.js');
const { parse_mf2, interpret_entry } = require('mf2utiljs');

async function interpret(url) {
  try {
    const parsed = await parse_mf2(url);
    return await interpret_entry(parsed, url);
  } catch (error) {
    //console.error(`Error interpreting entry from ${url}, ${JSON.stringify(error)}`);
    return null;
  }
}

async function doUnfurl(url) {
  try {
    return await unfurl(url);
  } catch (error) {
    console.error(`Error unfurling ${url}, ${JSON.stringify(error)}`);
    return null;
  }
}

async function scrape(url) {
  const mf2 = await interpret(url);
  if (mf2 && Object.keys(mf2).length > 0) {
    return { type: 'mf2', url, meta: mf2 };
  }
  const unfurl = await doUnfurl(url);
  if (unfurl && Object.keys(unfurl).length > 0) {
    return { type: 'unfurl', url, meta: unfurl };
  }
  return null;
}

module.exports = {
  interpret,
  unfurl: doUnfurl,
  scrape,
}
