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
    const yamlStr = yaml.dump(wmresults);
    const encoded = Base64.encode(yamlStr);
    const sha = await getWmResultsSHA();
    const response = await octokit.repos.createOrUpdateFileContents({
      owner: "drivet",
      repo: "website-11ty",
      path: WM_RESULTS_PATH,
      message: "save wmresults [skip ci]",
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
  const feedData = fs.readFileSync('_site/posts/feed_all.xml', 'utf8');
  const patom = util.promisify(atom);
  const feed = await patom(feedData);
  return feed.map(a => a.link.replace('https://desmondrivet.com/', ''));
}

function loadWebmentionResults() {
  const wmresultsData = fs.readFileSync('./src/_data/wmresults.yaml', 'utf8');
  return yaml.load(wmresultsData);
}

async function onSuccess({ utils }) {
  const urls = await loadUrls();
  const wmresults = loadWebmentionResults();
  const siteUrl = wmresults.site_url;
  const sources = urls.filter(u => wmresults.results[u] === undefined || wmresults.results[u] === null);
  if (sources.length === 0) {
    console.log('No webmentions to process');
    return;
  }
  console.log(`processing these source URLs for possible webmentions: ${JSON.stringify(sources)}`);
  const report = await sendAllWebmentions(sources.map(s => `${siteUrl}/${s}`));
  console.log(`webmentions (possibly) sent, showing results for ${siteUrl}`);
  for (const source of sources) {
    console.log(`Source URL: ${source}`);
    const resultForSource = report[`${siteUrl}/${source}`] || {};
    console.log(`Result: ${JSON.stringify(resultForSource)}`);
    wmresults.results[source] = resultForSource;
  }
  await commitWmResults(wmresults);
}

module.exports = {
  onSuccess
}
