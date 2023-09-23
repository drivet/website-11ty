const isDevEnv = process.env.ELEVENTY_ENV === 'development';
module.exports = {
  eleventyComputed: {
    permalink: data => {
      if (isDevEnv) {
        return `${data.page.filePathStem}.html`;
      } else {
        return false;
      }
    },
    eleventyExcludeFromCollections: data => {
      if (isDevEnv) {
        return data.eleventyExcludeFromCollections;
      } else {
        return true;
      }
    }
  }
};
