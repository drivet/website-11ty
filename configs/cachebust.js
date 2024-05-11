const { createHash } = require("crypto");
const fs = require("fs");

function cacheBustUrl(url, rootUrl, file) {
  const buff = fs.readFileSync(file);
  const hash = createHash('md5').update(buff).digest('hex');
  const thisUrl = new URL(url, rootUrl);
  thisUrl.searchParams.set('v', hash);
  return thisUrl.pathname + thisUrl.search;
}

function cacheBustConfig(eleventyConfig) {
  eleventyConfig.addFilter('cachebust', cacheBustUrl);
}

module.exports = {
  cacheBustConfig
}
