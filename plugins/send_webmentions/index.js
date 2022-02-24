const { atom } = require("feed-read-parser");
const fs = require('fs');
const util = require('util');
const yaml = require('js-yaml');
const { Octokit } = require("@octokit/rest");
const { Base64 } = require("js-base64");
const { sendAllWebmentions } = require('send-all-webmentions');

const WM_RESULTS_PATH = "src/_data/wmresults.yaml";

const octokit = new Octokit({
  auth: process.env.GITHUB_ACCESS_TOKEN,
});

async function getWmResultsSHA() {
  const result = await octokit.repos.getContent({
    owner: "drivet",
    repo: "website-11ty",
    path: WM_RESULTS_PATH,
  });
  return result?.data?.sha;
}

async function commitWmResults(wmresults) {
  try {
    const yaml_str = yaml.dump(wmresults);
    const encoded = Base64.encode(yaml_str);
    const sha = await getWmResultsSHA();
    const response = await octokit.repos.createOrUpdateFileContents({
      owner: "drivet",
      repo: "website-11ty",
      path: WM_RESULTS_PATH,
      message: "save wmresults",
      committer: {
        name: `Netlify`,
        email: "desmond.rivet@gmail.com",
      },
      author: {
        name: "Netlify",
        email: "desmond.rivet@gmail.com",
      },
      content: encoded,
      sha,
    });
    console.log(`Successfully comitted wmresults: ${JSON.stringify(response)}`);
  } catch(err) {
    console.error(`Error committing wmresults: ${JSON.stringify(err)}`);
  }
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
  const site_url = wmresults.site_url;
  const sources = urls.filter(u => wmresults.results[u] === undefined || wmresults.results[u] === null);
  if (sources.length === 0) {
    console.log('No webmentions to process');
    return;
  }
  console.log(`processing these source URLs for possible webmentions: ${JSON.stringify(sources)}`);
  const report = await sendAllWebmentions(sources.map(s => `${site_url}/${s}`));
  console.log(`webmentions (possibly) sent, showing results for ${site_url}`);
  for (const source of sources) {
    console.log(`Source URL: ${source}`);
    const result_for_source = report[`${site_url}/${source}`] || {};
    console.log(`Result: ${JSON.stringify(result_for_source)}`);
    wmresults.results[source] = result_for_source;
  }
  await commitWmResults(wmresults);
}

module.exports = {
  onSuccess
}
