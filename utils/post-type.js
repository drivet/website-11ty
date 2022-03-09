const validUrl = require("valid-url");

/**
 * Returns the "value" of the data passed in.
 * 
 * - if it's an array, use the first item, otherwise just use the item passed in
 * - If there's a "value" field in the data, use that, otherwise just use the value itelf
 *
 */
function getPropValue(value) {
  return value ? getValue(Array.isArray(value) ? value[0] : value) : undefined;
}

function getValue(value) {
  return value.value || value;
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
    const [propertyName, impliedType] = impliedTypes[i];
    if (propNames.includes(propertyName) && validUrl.isUri(getPropValue(props[propertyName]))) {
      return impliedType;
    }
  }

  return item.data.title ? 'article' : 'note';
}

module.exports = {
  postType
}
