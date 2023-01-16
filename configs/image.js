const Image = require("@11ty/eleventy-img");

async function makeImage(src, width) {
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
}

async function imgShortcode(src, alt, cls, width) {
  const data = await makeImage(src, width);

  const allCls = data['jpeg'][0].height > data['jpeg'][0].width ? `${cls} portrait` : cls;
  return Image.generateHTML(data, {
    alt,
    class: allCls
  });
}

async function imageShortcode(src, alt, cls) {
  return await imgShortcode(src, alt, cls, 800);
}

async function bigThumbShortcode(src, alt, cls) {
  return await imgShortcode(src, alt, cls, 400);
}

async function thumbShortcode(src, alt, cls) {
  return await imgShortcode(src, alt, cls, 200);
}

function imageConfig(eleventyConfig) {
  eleventyConfig.addNunjucksAsyncShortcode("image", imageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("bigthumb", bigThumbShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("thumb", thumbShortcode);
}

module.exports = {
  imageConfig,
}
