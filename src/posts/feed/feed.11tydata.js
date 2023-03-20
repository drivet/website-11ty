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
      if (s === 'twitter') {
        return 'https://brid.gy/publish/twitter?bridgy_omit_link=maybe'
      } else if (s === 'mastodon') {
        return 'https://brid.gy/publish/mastodon?bridgy_omit_link=maybe'
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

module.exports = {
  eleventyComputed: {
    postType: (data) => pt.postType({data}),
    bridgyMentions,
    referencedLink,
  }
};
