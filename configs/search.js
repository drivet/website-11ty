const elasticlunr = require("elasticlunr");
const { dateFormat } = require('../utils/helpers.js');

function searchIdxPosts(collection) {
  const index = elasticlunr();
  index.addField("title");
  index.addField("date");
  index.addField("tags");
  index.addField('refLink');
  index.addField("content");
  index.setRef("id");

  collection.forEach((page) => {
    index.addDoc({
      id: page.url,
      title: page.data.title,
      date: dateFormat(page.date, 'MMM D, YYYY, h:mm A Z'),
      tags: page.data.tags,
      postType: page.data.postType,
      refLink: page.data.referencedLink,
      content: page.templateContent 
    });
  });

  return index.toJSON();
};

function searchIdxRecipes(collection) {
  const index = elasticlunr();
  index.addField("title");
  index.addField("blurb");
  index.addField("tags");
  index.addField("ingredients");
  index.addField("content");
  index.setRef("id");

  collection.forEach((page) => {
    index.addDoc({
      id: page.url,      
      title: page.data.title,
      blurb: page.data.blurb,
      tags: page.data.tags,
      ingredients: page.data.ingredients,
      content: page.templateContent 
    });
  });

  return index.toJSON();
};

module.exports = {
  searchIdxPosts,
  searchIdxRecipes,
}
