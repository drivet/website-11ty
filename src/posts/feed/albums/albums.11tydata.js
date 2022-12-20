const { navigation } = require('../../../../utils/helpers.js');

const albumRoot = '/albums';

module.exports = {
  eleventyComputed: {
    navigation: data => navigation(albumRoot, data)    
  }  
};
