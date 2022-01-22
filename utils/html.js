const cheerio = require('cheerio');

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
