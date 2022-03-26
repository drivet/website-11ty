const fs = require('fs');

let scraped = null;

const CACHE_DIR = './_cache/scraped'

function loadScraped() {
  if (scraped !== null) {
    return scraped;
  }

  console.log('scraped data not loaded, loading...');
  const filePath = `${CACHE_DIR}/scraped.json`
  if (fs.existsSync(filePath)) {
    const cacheFile = fs.readFileSync(filePath)
    scraped = JSON.parse(cacheFile);
  } else {
    console.warn('no scraped cache, initializing...');
    scraped = {};
  }
  return scraped;
}

function saveScraped() {
  console.log('saving scraped...');
  const filePath = `${CACHE_DIR}/scraped.json`
  const fileContent = JSON.stringify(scraped, null, 2);
  // create cache folder if it doesnt exist already
  if (!fs.existsSync(CACHE_DIR)) {
    fs.mkdirSync(CACHE_DIR)
  }

  fs.writeFileSync(filePath, fileContent);
}

module.exports = {
  loadScraped, saveScraped
};
