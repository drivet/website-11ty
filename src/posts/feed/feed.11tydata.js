const pt = require('../../../utils/post-type.js');
const _ = require('lodash');

function bridgyMentions(data) {
  const properties = data;
  if (!properties) {
    return [];
  }

  const syndicationTokens = properties['mp-syndicate-to'];
  if (!syndicationTokens) {
    return [];
  }

  return syndicationTokens.
    map(s => {
      if (s === 'twitter_bridgy_no_link') {
        return 'https://brid.gy/publish/twitter?bridgy_omit_link=true'
      } else if (s === 'twitter_bridgy_maybe_link') {
        return 'https://brid.gy/publish/twitter?bridgy_omit_link=maybe'
      } else if (s === 'twitter_bridgy') {
        return 'https://brid.gy/publish/twitter'
      } else {
        return undefined;
      }
    }).
    filter(s => !!s);
}

function referencedLink(data) {
  if (!data) {
    return null;
  }
  
  return data['like-of'] ||
         data['repost-of'] ||
         data['in-reply-to'] ||
         data['bookmark-of'];
} 

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
    postType: (data) => pt.postType({data}),
    bridgyMentions,
    referencedLink, 
    eleventyNavigation
  }
};
