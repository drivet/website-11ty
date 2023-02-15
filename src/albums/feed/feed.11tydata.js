const pt = require('../../../utils/post-type.js');
const _ = require('lodash');

function eleventyNavigation(data) {
  if (!data.album) {
    return undefined;
  }
  return {
    key: data.key || data.title,
    parent: data.parent || "albums",
    title: data.title
  };
}

module.exports = {
  eleventyComputed: {
    eleventyNavigation
  }
};
