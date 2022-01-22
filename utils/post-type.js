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
  return value ? getValue(value[0]) : undefined;
}

function getValue(value) {
  return value.value || getHtmlValue(value.html) || value;
}

/**
 * Strip the content and name of non-alphanumeric
 * characters and check if the content includes the name.
 *
 * @private
 * @param {String} name Name property of the item
 * @param {String} content Content property of the item
 * @return {Boolean} Whether the content includes the name
 */
 function contentIncludesName(name, content) {
  const trimmedName = name.replace(/\W+/g, ' ');
  const trimmedContent = content.replace(/\W+/g, ' ');
  return trimmedContent.indexOf( trimmedName ) !== -1;
}

function postType(item) {
  if (!item || !item.data) {
    return undefined;
  }

  if (item.data.type === "h-event") {
    return "event";
  }

  const props = item.data.mp_properties ? item.data.mp_properties : {}
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

  const name = props.name ? getPropValue( props.name ) : undefined;
  const title = name ? name : item.data.title;
  return title ? 'article' : 'note';
  
  /*
  const content = getValue( item.content ) || getValue( props.summary );
  
  if (content && title && !contentIncludesName(title, content)) {
    return 'article';
  }

  return "note";
  */
}

module.exports = {
  postType
}
