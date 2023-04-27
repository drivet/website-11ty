const pt = require('../../../utils/post-type.js');
const _ = require('lodash');

function containsOnlyNumbers(str) {
  return str && /^\d+$/.test(str);
}

/**
 * NOT FOOLPROOF, but should weed out the obviously wrong ones
 * 
 * @param url 
 * @returns 
 */
function looksLikeMastodon(url) {
  if (!url) {
    return false;
  }
  const urlObj = new URL(url);
  const pathParts = urlObj.pathname.split('/');
  return pathParts[1] && pathParts[1].startsWith('@') && containsOnlyNumbers(pathParts[2]);
}

function bridgyMentions(properties) {
  if (!properties) {
    return [];
  }

  const syndicationTokens = properties['mp-syndicate-to'];
  if (!syndicationTokens && looksLikeMastodon(properties['like-of'] || properties['repost-of'])) {
    // the program I use to like and reposts, quill, doesn't let you stamp a 
    // syndication property when you like or repost, so do it here if this is a like or repost 
    // and it kind of looks like a mastodon link
    return ['https://brid.gy/publish/mastodon?bridgy_omit_link=maybe'];
  }

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
