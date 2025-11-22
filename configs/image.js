const Image = require("@11ty/eleventy-img");
const Fetch = require("@11ty/eleventy-fetch");
const debug = require('debug')('image');
const icoToPng = require("ico-to-png");

async function makeImage(src, width, format) {
  try {
    return await Image(src, {
      widths: [width],
      formats: [format],
      urlPath: "/static/img/",
      outputDir: "./_site/static/img/",
      cacheOptions: {
        duration: "*",
        directory: "_cache/images",
        removeUrlQueryParams: false,
      },
    });
  } catch (e) {
    debug(`cannot make image ${src}, ${width}, ${format}`);
    return null;
  }
}

async function atomImageShortcode(src, alt) {
  // match the width of regular image
  const data = await makeImage(src, 800, "jpeg");
  if (!data) {
    return `<img src="/static/img/avatar.png" alt=""/>`
  }
  return `<img src="${data.jpeg[0].url}" alt="${alt}" width="600"/>`;
}

async function atomThumbShortcode(src, alt) {
  // match the width of thumb image
  const data = await makeImage(src, 200, "jpeg");
  if (!data) {
    return `<img src="/static/img/avatar.png" alt=""/>`
  }
  return `<img src="${data.jpeg[0].url}" alt="${alt}" width="200"/>`;
}

async function imgShortcode(src, alt, cls, width, onerror) {
  const data = await makeImage(src, width, "jpeg");
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
  return await imgShortcode(src, alt, cls, 1200, onerror);
}

async function imageShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, 800, onerror);
}

async function bigThumbShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, 400, onerror);
}

async function thumbShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, 200, onerror);
}

async function avatarShortcode(src, name) {
  const alt = name;
  const title = name;
  const width = 32;
  const staticAvatar = '/static/img/avatar.png';
  if (!src || !src.startsWith("http")) {
    return `<img src="${staticAvatar}" alt="${alt}" title="${title}" />`
  }

  const data = await makePng(src, width);
  if (!data) {
    return `<img src="${src}" alt="${alt}" title="${title}" />`
  }

  const cls = "u-photo avatar";
  const attributes = {
    alt,
    title,
    class: cls,
    onerror: `if (this.src != '${staticAvatar}') this.src = '${staticAvatar}';`,
  };
  return Image.generateHTML(data, attributes);
}

async function makePng(src, width) {
  if (src.split("?")[0].endsWith(".ico")) {
    try {
      const pngBuffer = await convertIcoToPng(src, width);
      return await makeImage(pngBuffer, width, "png");
    } catch (e) {
      return await makeImage(src, width, "png");
    }
  } else {
    return await makeImage(src, width, "png");
  }
}

async function convertIcoToPng(href, width) {
  const icoBuffer = await Fetch(href, {
    type: "buffer",
    dryRun: true,
  });
  return icoToPng(icoBuffer, width);
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
