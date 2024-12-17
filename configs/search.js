const elasticlunr = require("elasticlunr");
const { dateFormat } = require('../utils/helpers.js');

function searchPostsInit() {
  const index = elasticlunr();
  index.addField("title");
  index.addField("date");
  index.addField("tags");
  index.addField('refLink');
  index.addField("content");
  index.setRef("id");
  return index;
}

function searchPostsIdx(post, content, index) {
  index.addDoc({
    id: post.url,
    title: post.data.title,
    date: dateFormat(post.date, 'MMM D, YYYY, h:mm A Z'),
    tags: post.data.tags,
    postType: post.data.postType,
    refLink: post.data.referencedLink,
    content
  });
}

function searchRecipesInit() {
  const index = elasticlunr();
  index.addField("title");
  index.addField("blurb");
  index.addField("tags");
  index.addField("ingredients");
  index.addField("content");
  index.setRef("id");
  return index;
}

function searchRecipesIdx(post, content, index) {
  index.addDoc({
    id: post.url,      
    title: post.data.title,
    blurb: post.data.blurb,
    tags: post.data.tags,
    ingredients: post.data.ingredients,
    content
  });
}

function idxJson(index) {
  return index.toJSON();
}

module.exports = {
  searchPostsInit,
  searchPostsIdx,
  searchRecipesInit,
  searchRecipesIdx,
  idxJson
}
