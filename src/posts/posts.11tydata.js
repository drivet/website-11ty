const pt = require('../../utils/post-type.js');

module.exports = {
  eleventyComputed: {
    postType: (data) => pt.postType({data})
  }
};
