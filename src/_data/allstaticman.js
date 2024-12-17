const fs = require('fs');
const yaml = require("js-yaml");
const path = require('path');

const STATICMAN_DIR = "./_staticman";

module.exports = function () {
  return fs.readdirSync(STATICMAN_DIR)
    .filter(file => path.extname(file) === '.yml')
    .map(file => yaml.safeLoad(fs.readFileSync(path.join(STATICMAN_DIR, file))));
}
