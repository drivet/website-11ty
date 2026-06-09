const cheerio = require('cheerio');

function excerptParas(content, length) {
  let excerptParagraphs = [];
  let currentLength = 0;
  const paragraphs = content.match(/<p>.*?<\/p>/gs) || [];

  for (let paragraph of paragraphs) {
      excerptParagraphs.push(paragraph);
      
      // Strip HTML from the paragraph
      const text = paragraph.replace(/(<([^>]+)>)/gi, "");
      currentLength += text.length;

      if (currentLength >= length) {
          break;
      }
  }
  return [excerptParagraphs, paragraphs];
}

function excerpt(content, length) {
  const [excerptParagraphs] = excerptParas(content, length);
  return excerptParagraphs.join(" ");
}

function excerptInfo(content, length) {
  const [excerptParagraphs, paragraphs] = excerptParas(content, length);
  return {
    text: excerptParagraphs.join(" "),
    truncated: paragraphs.length > excerptParagraphs.length
  };
}

function links(html) {
  const $ = cheerio.load(html);
  const linkObjects = $('a');
  const links = [];
  linkObjects.each((_index, element) => {
    links.push($(element).attr('href'));
  });
  return links;
}

module.exports = {
  excerpt, excerptInfo, links
}
