module.exports = {
  eleventyComputed: {
    eleventyNavigation: {
      key: data => data.key || data.title,
      parent: data => data.parent,
      title: data => data.title
    }
  }
};
