---
date: 2022-04-04T15:00:42-04:00
title: "Link Contexts In Eleventy"
blurb: "An #indieweb aware approach to unfurling URLs in @eleven_ty"
post-status: draft
mp-syndicate-to:
- twitter_bridgy
tags:
- eleventy
- indieweb
- blogging
---

As a participant in the [IndieWeb][1], I publish my share of [likes][2],
[replies][3], [reposts][22], and [bookmarks][4].  These posts all have one
thing in common - they refer to another, external URL and only make sense in
the context of that URL.

This being the case, it's nice to give my readers (and myself) some sense of
what I'm referring to when I post that content.  The process of scraping
metadata from URLs for display purposes is called "URL unfurling" and
implementing it efficiently on an [Eleventy][5] based website is not entirely
trivial, especially when you're trying to save your [Netlify][6] build
minutes, so it's worth sharing what I've learned.

The IndieWeb community, according to the wiki, distinguishes between what it
calls "[link previews][7]" and "[reply contexts][8]".  A link preview is the
metadata you show about a post that you've liked or bookmarked, for example.
A reply context is the metadata you show about a post you've specifically
replied to.  The differences, to me at least, seem pretty subtle, so I've
haven't bothered drawing a line between the two cases; my *link contexts*,
as I call them, are used for all my posts that require information about an
external URL to make sense.

## Determining the URL To Unfurl

The first step to unfurling a URL is to figure out exactly which URL needs
to be unfurled.  This isn't that difficult, but still merits a bit of
explanation.

* If my post is a like, repost, bookmark, or reply, then I unfurl the
  u-like-of, u-repost-of, u-bookmark-of, or u-in-reply-to annotated links
  respectively.
* If my post is a [note][9], I pick out the first link in the post and
  unfurl that.
* If the post is an [article][10], then I don't unfurl anything at all.

The first case is easily implemented with Eleventy's [computed data
feature][32]:

``` javascript
function referencedLink(data) {
  if (!data) {
    return null;
  }
  
  return data['like-of'] ||
         data['repost-of'] ||
         data['in-reply-to'] ||
         data['bookmark-of'];
} 

module.exports = {
  eleventyComputed: {
    referencedLink,
  }
};

```

The second case, as far I'm aware, can't be done with computed data because
that feature doesn't look at actual content, so I add a separate filter to
extract the first link of a post:

``` javascript
eleventyConfig.addFilter('contentLink', content => {
  const links = html.links(content);
  return links[0];
});
```

The `html.links()` call is written with the help of a library called
[cheerio][25], which is basically server side [jquery][26]:

``` javascript
const cheerio = require('cheerio');

function links(html) {
  const $ = cheerio.load(html);
  const linkObjects = $('a');
  const links = [];
  linkObjects.each((_index, element) => {
    links.push($(element).attr('href'));
  });
  return links;
}
```

In a nunjunks template to render your post, you'd pull out the link to
unfurl like this:

{% raw %}
``` liquid
{% set linkToUnfurl = referencedLink or (content | contentLink) %}
```
{% endraw %}

## Sources of Metadata

There are several different sources of metadata for a URL including, but not
limited to:

* [Microformats][11]
* [Twitter cards][12]
* [Open graph data][13]
* A separate [oEmbed][14] server

This, obviously, raises the question of metadata priority.  I eventually
settled on the following:

* If I'm dealing specifically with a Twitter post or YouTube video, then I
  use the oEmbed metadata associated with the post, specifically the "rich"
  type.  Other sites have oEmbed metadata, and some even use the "rich"
  type, but in my experience the quality varies.  Twitter posts and YouTube
  videos consistently look good, though.
* Otherwise, I prefer microformats if they are available.
* Lacking microformats, I prefer Open Graph metadata, followed by Twitter
  metadata, on a per attribute basis.

## Unfurling Libraries

Microformats are very useful, but they are not always easy to decipher.  In
particular, it's not always trivial to pull out [author information][23], or
[post type information][24] for use in a link context on your own site.  In
the past, I relied on a Python package called [mf2util][15] to do this kind
of thing, but I wasn't able to find an equivalent library in JavaScript.

Note that I'm *not* talking about a microformat parser here, of which the
JavaScript ecosystem [has a few][16].  I'm talking about a library that
looks at the parsed microformats and simplifies it so that it can be more
easily used on a personal site.

So I wrote a library to help me.  I basically ported (most of) the mf2util
package from Python to JavaScript and called it [mf2utiljs][17].  You may
find it useful.

For posts that do not have any microformats attached to them, I use the
[unfurl.js][18] package, which scrapes various forms of metadata, including
Open Graph, Twitter cards, and oEmbed, all in one shot.

## Scraping and Caching the Metadata

For each URL that I need to unfurl, I do the following:

* Try to scrape the microformats from the URL and interpret the results via
  mf2utiljs.  If this gives me a reasonable answer, then we stop there.
* If mf2utils is unsuccessful, I fall back on unfurl.js, fetching both
  oEmbed and more traditional Twitter/Open Graph metadata.

Given that there are network calls involved, this scraping takes a
not-insignificant amount of time to run, and I have a lot of posts, so
ideally we'd like to cache the results between builds.  As it turns out,
Eleventy does provide caching utilities that makes this fairly easy to
accomplish.

Now, my *first* instinct was to follow [Max Bock's][27] example of [loading
and caching webmentions][28].  His basic approach is to leverage [Eleventy's
global data feature][30]:

* Load all his webmentions as one big JSON object
* Fetch all the new webmentions since the last build
* Merge the new webmentions into the existing cache
* Save the results
* Use the newly updated webmention list as Netlify global data

This is all done *as the global webmention list is being loaded*,
i.e. before any template gets a chance to use.  Templates only ever see the
fully updated webmention data.

It's a tempting idea, but doesn't fully work in my case because, unlike his
webmention list, I don't have a way to update the metadata cache as soon as
I load it.  I don't have, up front, all the new URLs to scrape.  What *I*
need to do is slightly different:

* Load the metadata cache as a JSON object.
* Run the Eleventy build and each time there's a link context to render,
  consult the cache.
* Use the metadata in the cache or, if nothing is there, scrape some new
  metadata and save it in the cache object
* Save the updated cache object at the end of the build.

I'm not aware of a way to make this work with Eleventy's global data
feature.  For example, in order to save the updated cache at the end, I
would need a way to access the global data in JavaScript, from some kind of
API, and I couldn't find a way to do that.  I'm probably missing something.

Instead, I take a different approach:

* Write a preview filter for use in templates, and load the cache *lazily*
  from the filter.
* Consult the metadata cache from the filter, and update the cache (making
  network calls as required).
* Save the cache at the end of the build.

``` javascript
const { unfurl } = require('unfurl.js');
const { parse_mf2, interpret_entry } = require('mf2utiljs');
const slugify = require('slugify');
const fs = require('fs');

let scraped = null;

const CACHE_DIR = './_cache/scraped'

function loadScraped() {
  if (scraped !== null) {
    return scraped;
  }

  console.log('scraped data not loaded, loading...');
  const filePath = `${CACHE_DIR}/scraped.json`
  if (fs.existsSync(filePath)) {
    const cacheFile = fs.readFileSync(filePath)
    scraped = JSON.parse(cacheFile);
  } else {
    console.warn('no scraped cache, initializing...');
    scraped = {};
  }
  return scraped;
}

function saveScraped() {
  console.log('saving scraped...');
  const filePath = `${CACHE_DIR}/scraped.json`
  const fileContent = JSON.stringify(scraped, null, 2);
  // create cache folder if it doesnt exist already
  if (!fs.existsSync(CACHE_DIR)) {
    fs.mkdirSync(CACHE_DIR)
  }

  fs.writeFileSync(filePath, fileContent);
}

/**
 * Scrape the metadata from the URL and return a structure like this:
 * {
 *   url: <the url you scraped>
 *   type: "indieweb" | "oembed" | "unfurl",
 *   meta: <either intepreted mf2 data from mf2utilsjs or unfurled data>
 * }
 * 
 * We first try to scraped microformat data from the URL and it that works, we stop, 
 * put the interpreted data in the meta field and slap an "mf2" in the type field.
 * 
 * If there's no mf2 data, we run unfurl (with oEmbed turned on) from the unfurljs
 * package, slap an "unfurl" into the type field, and the data into the meta field
 * 
 * @param {*} url 
 * @returns 
 */
async function scrape(url) {
  try {
    const parsed = await parse_mf2(url);
    const mf2 = await interpret_entry(parsed, url);
    if (mf2 && Object.keys(mf2).length > 0) {
      return { url, type: 'mf2', meta: mf2 };
    }

    const unfurled = await unfurl(url);
    if (unfurled && Object.keys(unfurled).length > 0) {
      return { url, type: 'unfurl', meta: unfurled };
    }
  } catch (error) {
    console.error(`Error scraping ${url}, ${JSON.stringify(error)}`);
  }
  return null;
}

module.exports = (eleventyConfig) => {
 /**
   * This filter will
   * 
   * a) load the cached scraped data
   * b) try and find the scraped for the key, and scrape one if it can't
   * c) massage the data and return the preview
   * 
   */
  eleventyConfig.addNunjucksAsyncFilter('preview', async (url, cb) => {
    const scraped = loadScraped();
    const key = slugify(url);
    if (!(key in scraped)) {
      console.log(`no scraped data for ${url}, trying to scrape some...`);
      scraped[key] = await scrape(url);
    }
    const result = await preparePreview(scraped[key]);
    return cb(null, result);
  });

  /**
   * Save the previews in cache after build
   */
  eleventyConfig.on('eleventy.after', () => {
    saveScraped();
  });
}
```

We'll get to the `preparePreview` call in a minute.  For the moment, you can
see that we define two functions to load and save the cache, with the load
function only reading the file once and returning the loaded results upon
subsequent calls.

We then define an asynchronous filter, where the cache is checked, and
metadata is scraped if none is found, after which we update the cache.

We then save the cache to disk after the build is finished using the
"eleventy.after" event hook.

You can use the preview filter in a template like this:

{% raw %}
``` liquid
{% set preview_data = linkToUnfurl | preview %}
```
{% endraw %}


## Saving the Cache Between Netlify Builds

Saving the metadata is not very useful unless we can persist the cache
between Netlify builds, so we need a way to so that.  The approach I've
taken was shamelessly pilfered from Max Bock's site, almost verbatim.  Feel
free to consult [that article][29] for information on how to do this.

## Formatting the Results

Once I manage to get an instance of the scrapped metadata, I call
`preparePreview` on it to create a JavaScript object that that be passed to
templates for rendering.

I've tried to create a display model that works for both IndieWeb data and
non-IndieWeb data.  Given that I don't know all that much about metadata
formats, it's very possible that I've screwed up at least part of this.
Nonetheless, the results seem okay for the posts I've looked at.  The
preview data looks roughly like this:

``` json
{
  title: string,
  description: string,
  published: string,
  image: URL,
  site: string,
  siteUrl: string,
  favicon: URL,
}
```

Some things to consider:

* None of these fields are mandatory. The title won't be there, for example,
  if we're dealing with an IndieWeb note.  Outside the IndieWeb we just use
  the title attribute from Open Graph or Twitter.
* The description is a taken from the summary of the IndieWeb post, or a
  truncated version of the content.  Otherwise we use the Open Graph or
  Twitter description attributes.
* The published field is really only extracted for IndieWeb posts as I could
  not find a reliable way to extract it for non-IndieWeb posts.

The image is chosen as follows:

* If this is an IndieWeb post, we'll pick the [featured image][20] if there
  is one, or the photo image if not (or no image if neither are defined).
* If this is a non-IndieWeb post, we pick first image element of either the
  Open Graph or Twitter data (in that order).
  
Potentially controversial is my attempt to shoehorn both IndieWeb author
data and non-IndieWeb site data into one model:

* The site field is the author name from an IndieWeb post, or the site name
  data from Open Graph or Twitter.
* The site URL is really only taken from IndieWeb posts and is the home
  address of the post, as I could figure not out how to reliably extract
  this information in a non-IndieWeb context.
* The favicon is the author's photo from an IndieWeb post or the favicon in
  a non-IndieWeb page.

I realize this is risky.  I'm assuming that there is only ever one author of
an IndieWeb site and, correspondingly, that I'll never be interested in the
author of article from a non-IndieWeb site.  I suspect these assumptions
will bite me in the ass at some point, but it seems to work for now.

Here is the code:

``` javascript
/**
 * Pass in scraped data (i.e. the type, url and actual metadata).
 * Route to the correct function based on the type. 
 */
async function preparePreview(scraped) {
  if (!scraped) {
    return null;
  }

  if (scraped.type === 'mf2') {
    return await prepareMf2Preview(scraped);
  } else if (useOEmbed(scraped.url, scraped.meta)) {
    return prepareOembedPreview(scraped);
  } else if (useMetadata(scraped)) {
    return await prepareMetadataPreview(scraped);
  } else {
    return null;
  }
}

function truncate(str, chars, replace = '...') {
  const truncated = str.substring(0, chars);
  if (truncated.length === str.length) {
    return str;
  } else {
    const replaceLength = replace.length;
    return truncated.substring(0, chars-replaceLength) + replace;
  }
}

function extractSummary(meta) {
  if (meta.summary) {
    return meta.summary;
  } else if (meta['content-plain']) {
    return truncate(meta['content-plain'], 500);
  }
}

function extractPicture(meta) {
  const picture = meta?.featured || meta?.photo;
  if (picture) {
    return {
      alt: "featured image",
      url: picture
    }; 
  } else {
    return null;
  }
}

async function prepareMf2Preview(scraped) {
  const scrapedCopy = clone(scraped);
  const meta = scrapedCopy.meta;
 
  const image = extractPicture(meta);
  const preview = {
    type: scrapedCopy.type,
    meta: {
      url: scrapedCopy.url,
      title: meta?.name,
      description: extractSummary(meta),
      published: meta.published || meta['published-str'],
      image,
      site: meta?.author?.name,
      siteUrl: meta?.author?.url,
      favicon: meta?.author?.photo
    }
  };
  await cacheImages(preview);
  return preview;
}

function prepareOembedPreview(scraped) {
  return {
    type: 'oembed',
    meta: {
      oEmbed: {
        html: scraped.meta.oEmbed.html
      }
    }
  };
}


/**
 * Meta has both open_graph and twitter instances.
 * Prefer open_graph, fall back to twitter
 * @param {*} meta 
 */
function select(meta, f1, f2) {
  function valueOrFirst(value) {
    return Array.isArray(value) ? value[0]: value;
  }

  if (!meta) {
    return null;
  }
  let value = null;
  if (meta.open_graph) {
    value = valueOrFirst(meta.open_graph[f1]);
  }
  if (value) {
    return value;
  }
  const twitter_att = f2 ? f2 : f1;
  if (meta.twitter_card) {
    value = valueOrFirst(meta.twitter_card[twitter_att]);
  }
  return value;
}

/**
 * Return a structure that looks like:
 * 
 * {
 *   type: unfurl,
 *   meta: {
 *     title, description, image, site, favicon
 *   }
 * }
 * 
 * We will choose a title, description and image from the open graph and twitter data,
 * based on what is available.
 * 
 * Then we will cache the preview image if there is one.
 * 
 * @param {*} scraped 
 * @returns 
 */
async function prepareMetadataPreview(scraped) {
  const scrapedCopy = clone(scraped);
  const meta = scrapedCopy.meta;
 
  const preview = {
    type: scrapedCopy.type,
    meta: {
      url: scrapedCopy.url,
      title: select(meta, 'title'),
      description: select(meta, 'description'),
      image: select(meta, 'images'),
      site: select(meta, 'site_name', 'site'),
      favicon: meta?.favicon
    }
  };
  cacheImages(preview);
  return preview;
}

/**
 * 
 * We will prefer oEmbed metadata if 
 * 
 * a) the URL is in the oembedList defined above
 * b) the oEmbed data is rich or video
 * 
 * Not every site has a good oEmbed implementation, even if they say it's rich, which
 * is why we only use it for a couple of sites.
 *
 * @param {*} url 
 * @param {*} meta 
 * @returns 
 */
function useOEmbed(url, meta) {
  return meta.oEmbed &&
         ["rich", "video"].includes(meta.oEmbed.type) &&
         OEMBED_LIST.some(u => url.startsWith(u));
}

function useMetadata(scraped) {
  return scraped.meta?.open_graph || scraped.meta?.twitter_card;
}

```

## Image Caching

Note the `cacheImages` call above.  I make use of [Eleventy's Image
plugin][19] to cache and format featured images, author avatars and favicons
to make sure we always have a local copy and they are sized reasonably.
Here's the implementation:

``` javascript
async function cacheImages(preview) {
  if (preview?.meta?.favicon) {
    preview.meta.favicon =
      await cachePreviewImage(preview.meta.favicon, [64], ['png']);
  }
  if (preview?.meta?.image?.url) {
    preview.meta.image.url =
      await cachePreviewImage(preview.meta.image.url, [750], ['jpeg']);
  }
}

async function cachePreviewImage(url, widths, format) {
  try {
    const stats = await Image(url, {
      widths,
      outputDir: "./_site/static/previews",
      urlPath: "/static/previews",
      formats: [format],
      cacheOptions: {
        duration: "*",
        directory: "_cache/previews",
        removeUrlQueryParams: false,
      },
    });
    return stats[format][0].url;
  } catch (e) {
    if (url.startsWith('http:') || url.startsWith('https:')) {
      console.warn(`could not process preview image ${url}, ${JSON.stringify(e)}`);
    } else {
      console.warn(`could not process non-http URL`);
    }
    return url;
  }
}
```

The whole image is downloaded and cached, after which it is resized and
deposited into my site's static folder.

## Conclusion

I'm not going to pretend that I've hacked up the perfect link unfurling
system, but I *am* going to claim that it covers a significant number of use
cases that are useful to me, including:

* Both link previews and reply contexts.
* Both IndieWeb and non-IndieWeb links.
* Judicious use of oEmbed rich data for Twitter and YouTube.

In addition, I cache the metadata and images so that we don't see broken
links if the site goes down.

Complete code can be found [on github][31].  Hopefully someone finds it
useful!

[1]: https://indieweb.org/
[2]: https://indieweb.org/like
[3]: https://indieweb.org/reply
[4]: https://indieweb.org/bookmark
[5]: https://www.11ty.dev/
[6]: https://www.netlify.com/
[7]: https://indieweb.org/link-preview
[8]: https://indieweb.org/reply-context
[9]: https://indieweb.org/note
[10]: https://indieweb.org/article
[11]: https://indieweb.org/microformats
[12]: https://indieweb.org/Twitter_Cards
[13]: https://indieweb.org/The-Open-Graph-protocol
[14]: https://indieweb.org/oEmbed
[15]: https://pypi.org/project/mf2util/
[16]: https://www.npmjs.com/package/microformats-parser
[17]: https://www.npmjs.com/package/mf2utiljs
[18]: https://www.npmjs.com/package/unfurl.js
[19]: https://www.11ty.dev/docs/plugins/image/
[20]: https://indieweb.org/featured
[21]: https://github.com/drivet/website-11ty/blob/main/configs/previews.js
[22]: https://indieweb.org/repost
[23]: https://indieweb.org/authorship-spec
[24]: https://indieweb.org/post-type-discovery
[25]: https://cheerio.js.org/
[26]: https://jquery.com/
[27]: https://mxb.dev
[28]: https://mxb.dev/blog/using-webmentions-on-static-sites/
[29]: https://mxb.dev/blog/persistent-build-folders-netlify/
[30]: https://www.11ty.dev/docs/data-global/
[31]: https://github.com/drivet/website-11ty/blob/main/configs/previews.js
[32]: https://www.11ty.dev/docs/data-computed/
