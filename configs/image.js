const Image = require("@11ty/eleventy-img");
const debug = require('debug')('previews');

async function makeImage(src, widths) {
  try {
    return await Image(src, {
      widths,
      formats: ["jpeg"],
      urlPath: "/static/img/",
      outputDir: "./_site/static/img/" ,
      cacheOptions: {
        duration: "*",
        directory: "_cache/images",
        removeUrlQueryParams: false,
      },
    });
  } catch (e) {
    debug(`could not process image ${src}, ${JSON.stringify(e)}`);
    // not a good check but not sure what else to do 
    // interpreting a message to mean there is an image but the plugin couldn't handle it
    // return e.message ? src : null;
    return null;
  }
}

async function atomImageShortcode(src, alt) {
  const data = await makeImage(src, [800]); // match the width of regular image
  return `<img src="${data.jpeg[0].url}" alt="${alt}" width="600"/>`;
}

async function atomThumbShortcode(src, alt) {
  const data = await makeImage(src, [200]); // match the width of thumb image
  return `<img src="${data.jpeg[0].url}" alt="${alt}" width="200"/>`;
}

async function imgShortcode(src, alt, cls, widths, onerror) {
  const data = await makeImage(src, widths);
  if (!data) {
    return `<img src="/static/img/avatar.png" alt=""/>`
  }
  const allCls = data['jpeg'][0].height > data['jpeg'][0].width ? `${cls} portrait` : cls;
  const attributes = {
    alt, class: allCls,
    loading: "lazy",
    decoding: "async",
  };
  if (onerror) {
    attributes.onerror = onerror
  }
  return Image.generateHTML(data, attributes);
}

async function largeImageShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, [1200], onerror);
}

async function imageShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, [800], onerror);
}

async function bigThumbShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, [400], onerror);
}

async function thumbShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, [200], onerror);
}

async function avatarShortcode(src, name) {
  const alt = name;
  const title = name;
  const width = 32;
  const staticAvatar = '/static/img/avatar.png';
  if (!src || !src.startsWith("http")) {
    return `<img src="${staticAvatar}" alt="${alt}" title="${title}" />`
  }
  const data = await makeImage(src, [width]);
  if (!data) {
    return `<img src="${staticAvatar}" alt="${alt}" title="${title}" />`
  }
  const cls = "u-photo avatar";
  const allCls = data['jpeg'][0].height > data['jpeg'][0].width ? `${cls} portrait` : cls;
  const attributes = {
    alt,
    title,
    class: allCls,
    onerror: `if (this.src != '${staticAvatar}') this.src = '${staticAvatar}';`,
  };
  return Image.generateHTML(data, attributes);
 
}

function imageConfig(eleventyConfig) {
  eleventyConfig.addNunjucksAsyncShortcode("largeImage", largeImageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("image", imageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("bigthumb", bigThumbShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("thumb", thumbShortcode); 
  eleventyConfig.addNunjucksAsyncShortcode("avatar", avatarShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("atomImage", atomImageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("atomThumb", atomThumbShortcode);
}

module.exports = {
  imageConfig,
}
