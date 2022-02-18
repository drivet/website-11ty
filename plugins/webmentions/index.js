const { atom } = require("feed-read-parser");
const fs = require('fs');
const util = require('util');
const yaml = require('js-yaml');
const { Octokit } = require("@octokit/rest");
const { Base64 } = require("js-base64");
const { sendAllWebmentions } = require('send-all-webmentions');
const ConsoleLogger = require("@11ty/eleventy/src/Util/ConsoleLogger");

const octokit = new Octokit({
  auth: process.env.GITHUB_ACCESS_TOKEN,
});

async function commit(wmresults) {
  const yaml_str = yaml.dump(wmresults);
  console.log('committing this yaml');
  console.log(yaml_str);
  /*
  const encoded = Base64.encode(yaml_str);
  const { data } = await octokit.repos.createOrUpdateFileContents({
    owner: "drivet",
    repo: "website-11ty",
    path: "src/_data/wmresults.yaml",
    message: "save wmresults",
    content: encoded,
    committer: {
      name: `Netlify`,
      email: "desmond.rivet@gmail.com",
    },
    author: {
      name: "Netlify",
      email: "desmond.rivet@gmail.com",
    },
  });
  */
}

async function loadUrls() {
  const feed_data = fs.readFileSync('_site/posts/feed_all.xml', 'utf8');
  const patom = util.promisify(atom);
  const feed = await patom(feed_data);
  return feed.map(a => a.link.replace('https://desmondrivet.com/', ''));
}

function loadWebmentionResults() {
  const wmresults_data = fs.readFileSync('./src/_data/wmresults.yaml', 'utf8');
  return yaml.load(wmresults_data);
}

async function onSuccess({ utils }) {
  const urls = await loadUrls();
  const wmresults = loadWebmentionResults();
  const sources = urls.filter(u => wmresults.results[u] === undefined || wmresults.results[u] === null);
  console.log(`sending webmentions for sources: ${JSON.stringify(sources)}`);
  const send_results = sendAllWebmentions(sources);
  for (const source of sources) {
    const result_for_source = send_results[source] || {};
    console.log(`result for source: ${JSON.stringify(result_for_source)}`);
    wmresults.results[source] = result_for_source;
  }
  commit(wmresults);
}

module.exports = {
  onSuccess
}
