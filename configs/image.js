const Image = require("@11ty/eleventy-img");

async function makeImage(src, width) {
  try {
    return await Image(src, {
      widths: [width],
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
    console.warn(`failed to create image ${src}`);
    return null;
  }
}

async function imgShortcode(src, alt, cls, width, onerror) {
  const data = await makeImage(src, width);
  if (!data) {
    return `<img src="/static/img/avatar.png" alt=""/>`
  }
  const allCls = data['jpeg'][0].height > data['jpeg'][0].width ? `${cls} portrait` : cls;
  const attributes = {
    alt, class: allCls
  };
  if (onerror) {
    attributes.onerror = onerror
  }
  return Image.generateHTML(data, attributes);
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

async function iconShortcode(src, alt, cls, onerror) {
  return await imgShortcode(src, alt, cls, 32, onerror);
}

function imageConfig(eleventyConfig) {
  eleventyConfig.addNunjucksAsyncShortcode("image", imageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("bigthumb", bigThumbShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("thumb", thumbShortcode); 
  eleventyConfig.addNunjucksAsyncShortcode("icon", iconShortcode);
}

module.exports = {
  imageConfig,
}
