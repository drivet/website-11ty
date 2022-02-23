const pt = require('../../utils/post-type.js');
const _ = require('lodash');

function bridgy_mentions(data) {
  const properties = data;
  if (!properties) {
    return [];
  }

  const syndication_tokens = properties['mp-syndicate-to'];
  if (!syndication_tokens) {
    return [];
  }

  return syndication_tokens.
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


function referenced_link(data) {
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
    bridgy_mentions,
    referenced_link
  }
};
