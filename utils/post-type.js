const htmlToText = require("html-to-text");
const validUrl = require("valid-url");

/**
 * Gets the plain text value from a
 * html value mf2 field.
 *
 * @private
 * @param {Array|Object} html Item from which value needs to be extracted
 * @return {String} Extracted value
 */
function getHtmlValue(html) {
  return htmlToText.fromString(html, {
    ignoreHref: true,
    uppercaseHeadings: false,
    wordwrap: false,
  });
}

/**
 * Gets the plain text value from a
 * value mf2 field.
 *
 * @private
 * @param {Array|Object} value Item from which value needs to be extracted
 * @return {String} Extracted value
 */
function getPropValue(value) {
  return value ? getValue(Array.isArray(value) ? value[0] : value) : undefined;
}

function getValue(value) {
  return value.value || getHtmlValue(value.html) || value;
}

function postType(item) {
  if (!item || !item.data) {
    return undefined;
  }

  if (item.data.type === "h-event") {
    return "event";
  }

  const props = item.data;
  const propNames = Object.keys(props);

  if (propNames.includes( 'rsvp' ) &&
     (props.rsvp.includes( 'yes' ) ||
      props.rsvp.includes( 'no' ) ||
      props.rsvp.includes( 'maybe' ) ||
      props.rsvp.includes( 'interested' ))) {
    return 'rsvp';
  }

  const impliedTypes = [
    ["repost-of", "repost"],
    ["like-of", "like"],
    ["bookmark-of", "bookmark"],
    ["in-reply-to", "reply"],
    ["video", "video"],
    ["photo", "photo"],
  ];
  
  for (let i = 0; i < impliedTypes.length; i++) {
    const it = impliedTypes[i];
    if (propNames.includes(it[0]) && validUrl.isUri(getPropValue(props[it[0]]))) {
      return it[1];
    }
  }

  return item.data.title ? 'article' : 'note';
}

module.exports = {
  postType
}
