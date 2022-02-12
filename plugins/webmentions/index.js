
const { identify, atom } = require("feed-read-parser");
const fs = require('fs');
const util = require('util');
const yaml = require('js-yaml');

async function loadUrls() {
  const feed_data = fs.readFileSync('_site/posts/feed_all.xml');
  console.log(`FEED DATA! ${JSON.stringify(feed_data)}`);
  const t = identify(feed_data);
  console.log(`feed type! ${t}`);
  const patom = util.promisify(atom);
  const feed = await patom(feed_data);
  console.log(`feed: ${JSON.stringify(feed)}`);
  return feed.map(a => link);
}

async function loadWebmentionResults() {
  const wmresults_data = await fs.readFile('./src/_data/wmresults.yaml');
  const wmresults = yaml.load(wmresults_data);
  return wmresults.results;
}

function toProcess(urls, wmresults) {
  return urls.filter(u => wmresults[u] === undefined || wmresults[u] === null);
}

module.exports = {
  async onSuccess({ utils }) {
    const urls = await loadUrls();
    console.log(`Urls ${JSON.stringify(urls)}`);
    //const results = loadWebmentionResults();
    //const toSend = toProcess(urls, results);
    //console.log(`Webmentions to send ${JSON.stringify(toSend)}`);
  },
 
}
