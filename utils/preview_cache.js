const fs = require('fs');

let previews = null;

const CACHE_DIR = './_cache'

function load_previews() {
  if (previews !== null) {
    return previews;
  }

  console.log('previews not loaded, loading...');
  const filePath = `${CACHE_DIR}/previews.json`
  if (fs.existsSync(filePath)) {
    const cacheFile = fs.readFileSync(filePath)
    previews = JSON.parse(cacheFile);
  } else {
    console.warn('no preview cache, initializing...');
    previews = {};
  }
  return previews;
}

function save_previews() {
  console.log('saving previews...');
  const filePath = `${CACHE_DIR}/previews.json`
  const fileContent = JSON.stringify(previews, null, 2);
  // create cache folder if it doesnt exist already
  if (!fs.existsSync(CACHE_DIR)) {
    fs.mkdirSync(CACHE_DIR)
  }

  fs.writeFileSync(filePath, fileContent);
}

module.exports = {
  load_previews, save_previews
};
