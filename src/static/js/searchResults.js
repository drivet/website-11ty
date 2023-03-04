(function (window, document) {
  "use strict";
  const queryString = window.location.search;
  const urlParams = new URLSearchParams(queryString);
  const root = urlParams.get('searchRoot');

  const refLinkType = (postType) => {
    if (postType === 'like') {
      return "Like of";
    } else if (postType === 'reply') {
      return "Reply to"
    } else if (postType === 'bookmark') {
      return "Bookmark of";
    } else if (postType === 'repost') {
      return "Repost of";
    } else {
      return "";
    }
  };

  const search = () => {
    const field = urlParams.get('searchField');
    const results = window.searchIndex.search(field);
    const template =  document.getElementById(`searchResultsTemplate-${root}`).innerHTML;
    const compiledResult = Mustache.render(template, {
      field,
      root,
      hasResults: results.length > 0,
      results: results.map(r => ({
        id: r.doc.id,
        title: r.doc.title,
        blurb: r.doc.blurb,
        date: r.doc.date,
        refLinkType: refLinkType(r.doc.postType),
        refLink: r.doc.refLink,
        truncated: r.doc.content.length <= 200 ? r.doc.content : r.doc.content.slice(0, 200) + '[...]'
      }))
    });

    const resEl = document.getElementById("searchResults");
    resEl.innerHTML = compiledResult;
  };

  fetch(`/lunridx-${root}.json`).then((response) =>
    response.json().then((rawIndex) => {
      window.searchIndex = elasticlunr.Index.load(rawIndex);
      search();
    })
  );
})(window, document);
