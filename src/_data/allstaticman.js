const fs = require('fs');
const yaml = require("js-yaml");
const path = require('path');

const STATICMAN_DIR = "./_staticman";

module.exports = async function () {
  return fs.readdirSync(STATICMAN_DIR).map(file => {
    return yaml.safeLoad(fs.readFileSync(path.join(STATICMAN_DIR, file)));
  });
}
