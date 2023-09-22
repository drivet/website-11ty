module.exports = {
  eleventyComputed: {
    permalink: data => {
      if (process.env['ELEVENTY_ENV'] !== 'development') {
        return false;
      } else {
        return `${data.page.filePathStem}.html`;
      }
    }
  }
};
