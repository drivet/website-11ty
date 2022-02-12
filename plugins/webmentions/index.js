
const { atom } = require("feed-read-parser");
const fs = require('fs');
const util = require('util');
const yaml = require('js-yaml');

async function loadUrls() {
  const feed_data = fs.readFileSync('_site/posts/feed_all.xml', 'utf8');
  const patom = util.promisify(atom);
  const feed = await patom(feed_data);
  return feed.map(a => a.link.replace('https://desmondrivet.com/', ''));
}

async function loadWebmentionResults() {
  const wmresults_data = fs.readFileSync('./src/_data/wmresults.yaml', 'utf8');
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
    const results = loadWebmentionResults();
    const toSend = toProcess(urls, results);
    console.log(`Webmentions to send ${JSON.stringify(toSend)}`);
  },
 
}
