const sanitizeHTML = require('sanitize-html');
const rootUrl = require('../src/_data/global.json').URL;
const dayjs = require('dayjs');

function webmentionsForUrl(webmentions, url) {
  if (!webmentions) {
    return [];
  }

  const absUrl = `${rootUrl}${url}`.replace(/\.[^/.]+$/, "");

  const sanitize = entry => {
    const { content } = entry;
    if (content) {
      if (content['content-type'] === 'text/html') {
        content.value = sanitizeHTML(content.value);
      }
      if (content.htl) {
        content.html = sanitizeHTML(content.html);
      }
    }
    
    return entry;
  };
  return webmentions
      .filter(entry => entry['wm-target'] === absUrl)
      .map(sanitize);
}

function commentsForUrl(comments, url) {
  if (!comments) {
    return [];
  }  
  return comments
      .filter(entry => entry['commenting-on'] === url);
}

function webmentionKind(webmentions, ...kinds) {
  return webmentions.filter(entry => kinds.includes(entry['wm-property']));
}

function discussionForUrl(url, allstaticmandata, allwebmentions) {
  const comments = commentsForUrl(allstaticmandata, url);
  const webmentions = webmentionsForUrl(allwebmentions, url);
  const mentions = webmentionKind(webmentions, 'in-reply-to', 'mention-of');
  const likes = webmentionKind(webmentions, 'like-of');
  const reposts = webmentionKind(webmentions, 'repost-of');
  const bookmarks = webmentionKind(webmentions, 'bookmark-of');
  const commentsAndMentions = mergeMentionsAndComments(mentions, comments);
  return {
    commentsAndMentions, likes, reposts, bookmarks,
    total: comments.length + webmentions.length
  };
}

/**
 * Normalized entry is:
 * {
 *   published: date;
 *   url: string;
 *   author: {
 *     url: string;
 *     photo: string;
 *     name: string;
 *   };
 *   content {
 *     html: string;
 *     text: string;
 *   }
 * }
 * @param {*} mentions 
 * @param {*} comments 
 */
function mergeMentionsAndComments(mentions, comments) {
  const normalizedComments = comments.map(normalizeComment);
  return [
    ...mentions,
    ...normalizedComments
  ].sort((m1,m2) => dayjs(m2.published).unix() - dayjs(m1.published).unix());
}

function normalizeComment(staticmanData) {
  const photo = avatarUrl(staticmanData);
  return {
    author: {
      name: staticmanData.name,
      url: staticmanData.url,
      photo,
    },
    published: new Date(staticmanData.date * 1000),
    content: {
      text: staticmanData.message,
    }
  }
}

function avatarUrl(staticmanData) {
  return `https://gravatar.com/avatar/${staticmanData.email}`
  /*
  if (staticmanData.url) {
    const icon = await getFavicon(staticmanData.url);
    return icon ? icon : `https://gravatar.com/avatar/${staticmanData.email}`
  } else {
    return `https://gravatar.com/avatar/${staticmanData.email}`
  }
  */
}

function discussionConfig(eleventyConfig) {
  eleventyConfig.addFilter("discussionForUrl", discussionForUrl);
}

module.exports = {
  discussionConfig
}
