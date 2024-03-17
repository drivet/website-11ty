const cheerio = require('cheerio');
/*
function excerpt(html, maxCount) {
  if (!html) {
    return '';
  }
  
  const $ = cheerio.load(html);
  const nodes = $('body > *');
  let summary = '';
  let wordCount = 0;
  nodes.each((i, e) => {
      const wrapper = $(e).wrap('<p/>').parent();
      summary += wrapper.html();
      wordCount += wrapper.text().split(/[\s,]+/).length;
      return wordCount < maxCount;
  });
  return summary;
}
*/

function excerpt(content, length) {
  let excerptParagraphs = [];
  let currentLength = 0;
  const paragraphs = content.match(/<p>.*?<\/p>/gs) || [];

  for (let paragraph of paragraphs) {
      // Strip HTML from the paragraph
      const text = paragraph.replace(/(<([^>]+)>)/gi, "");

      if (currentLength > 0 && currentLength + text.length > length) {
          break;
      }

      excerptParagraphs.push(paragraph);
      currentLength += text.length;
  }

  return excerptParagraphs.join(" ");
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
  excerpt, links
}
