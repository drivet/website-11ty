module.exports = {
  eleventyComputed: {
    eleventyNavigation: {
      key: data => data.key,
      parent: data => data.parent,
      title: data => data.title
    }
  }
};
