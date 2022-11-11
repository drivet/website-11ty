const Image = require("@11ty/eleventy-img");

async function imageShortcode(src, alt, cls) {
  const data = await Image(src, {
    widths: [800],
    formats: ["jpeg"],
    urlPath: "/static/img/",
    outputDir: "./_site/static/img/" ,
    cacheOptions: {
      duration: "*",
      directory: "_cache/images",
      removeUrlQueryParams: false,
    },
  });

  return  Image.generateHTML(data, {
    alt,
    class: cls
  });
}

function imageConfig(eleventyConfig) {
  eleventyConfig.addNunjucksAsyncShortcode("image", imageShortcode);

}

module.exports = {
  imageConfig
}
