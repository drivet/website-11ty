const elasticlunr = require("elasticlunr");


function searchFilter(collection) {
  // what fields we'd like our index to consist of
  const index = elasticlunr(function () {
    this.addField("title");
    this.addField("tags");
    this.addField("ingredients");
    this.addField("repost-of");
    this.addField("like-of");
    this.addField("in-reply-to");
    this.addField("bookmark-of");
    this.addField("content");
    this.setRef("id");
  });

  // loop through each page and add it to the index
  collection.forEach((page) => {
    index.addDoc({
      id: page.url,
      title: page.data.title,
      tags: page.data.tags,
      'like-of': page.data['like-of'],
      'repost-of': page.data['repost-of'],
      'bookmark-of': page.data['bookmark-of'],
      'in-reply-of': page.data['in-reply-of'],
      ingredients: page.data.ingredients,
      content: page.templateContent 
    });
  });

  return index.toJSON();
};


module.exports = {
  searchFilter,
}
