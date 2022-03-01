const pt = require('../../utils/post-type.js');
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
  const properties = data;

  if (!properties) {
    return null;
  }
  
  return properties['like-of'] ||
         properties['repost-of'] ||
         properties['in-reply-to'] ||
         properties['bookmark-of'];
} 

module.exports = {
  eleventyComputed: {
    postType: (data) => pt.postType({data}),
    bridgyMentions,
    referencedLink,
  }
};
