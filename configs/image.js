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

async function imageShortcode(src, alt, cls) {
  const data = await makeImage(src, 800);

  return Image.generateHTML(data, {
    alt,
    class: cls
  });
}

async function thumbShortcode(src, alt, cls) {
  const data = await makeImage(src, 200);

  return Image.generateHTML(data, {
    alt,
    class: cls
  });
}

function imageConfig(eleventyConfig) {
  eleventyConfig.addNunjucksAsyncShortcode("image", imageShortcode);
  eleventyConfig.addNunjucksAsyncShortcode("thumb", thumbShortcode);
}

module.exports = {
  imageConfig,
}
