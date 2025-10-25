---
title: Eleventy For IndieWeb Enthusiasts - Basic Setup And Post Organization
blurb: 'Classifying your #indieweb posts with @eleven_ty'
date: '2022-03-09T16:46:02-05:00'
tags:
  - eleventy
  - indieweb
  - blogging
---

As [mentioned previously][1], I recently converted my website over to
[Eleventy][2].  I'm an [IndieWeb][3] participant and, moreover, my website
is fairly complex so I ended up writing (or, in some cases, pilfering) a
fair amount of code to make it all work together in an acceptable manner.  I
thought I would document the process via a series of blog posts in case
anyone else finds it of value.

None of this is intended to be an Eleventy tutorial and you'll get the most
from these posts if you already know [something about how Eleventy
works][4].

With that out of the way, we'll start with my basic setup.

## Post Format

My posts, like a lot of people's, are all written in markdown with yaml
frontmatter.  A typical [blog entry][5] looks like this:

``` markdown
---
date: 2022-03-01T13:57:07-05:00
title: 'Much Ado About (Indieweb) Bookmarks'
blurb: 'The ins and outs of owning your bookmarks on the #indieweb'
tags:
  - indieweb
mp-syndicate-to:
  - twitter
---

Like many Indieweb participants, I post...
```

There's nothing really surprising here and I suspect the format would be
familiar to [Jekyll][6] users.  I have a standard title, date and tags
attribute. The mp-syndicate-to and blurb attributes will be covered later
when I talk about syndication.

I also post [notes][7].  They look like this:

``` markdown
---
date: 2022-02-24T01:27:35Z
---

I am grateful that Wordle exists if for no other reason...
```

There's no title here, which marks it as a note instead of an article.  More
on that later.

I post [bookmarks][8], too:

``` markdown
---
date: 2022-01-16T18:10:55.755Z
bookmark-of: 'https://blog.mollywhite.net/its-not-still-the-early-days/'
---
```

As you can see, that one contains no title *and* no content.  Most of my
bookmarks (and [likes][9], and [reposts][10]) are formatted like this.  The
`bookmark-of` attribute identifies this as a bookmark post.  More on that
later.

## Permalinks

By default, Eleventy will create an output folder for every non-layout input
template, with the main content tucked away in a corresponding index.html
file, so you can access the page with URL that looks like `<content-path>/`.

I understand the rationale, but I still find it jarring to have a whole
folder for every output URL, so one of the first things I did was change
the permalink scheme.

My posts are all located in `src/posts/feed/` which has the following
`feed.json` [directory data file][11] tucked away inside:

**src/posts/feed/feed.json**:
``` json
{
  "permalink": " {{ '{{' }} page | postPermalink {{ '}}' }}",
  "layout": "post.njk"
}
```

You can see that the permalink is calculated via a postPermalink
[filter][12], defined in my .eleventy.js file as so:

**.eleventy.js**:
``` javascript
const dayjs = require('dayjs');
const utc = require('dayjs/plugin/utc')
const timezone = require('dayjs/plugin/timezone') // dependent on utc plugin
dayjs.extend(utc)
dayjs.extend(timezone)


function getSlug(fslug) {
  // test for my note files from micropub, which look like this:
  // 20200810123845.md
  if (/^\d\d\d\d\d\d\d\d\d\d\d\d\d\d(.*)/.test(fslug)) {
    // strip off the date portion, only pick up the time portion of the timestamp
    return fslug.substring(8);
  } else {
    // otherwise just return the filename
    return fslug;
  }
}

module.exports = (eleventyConfig) => {
  ...
  eleventyConfig.addFilter("postPermalink", page => {
    const date_part = dayjs(page.date).tz('America/Montreal').format('YYYY/MM/DD');
    const slug = getSlug(page.fileSlug);
    return `${date_part}/${slug}.html`;
  });
  ...
};
```

From reading this, you can see that my preferred permalink structure is
/year/month/day/slug.html (inspired by [Cool URIs don't change][13]), and
you can probably tell that I live in Montreal.  Because I host my website on
[Netlify][14], you can drop the html part of the permalink when accessing
the URL in a browser, and it becomes /year/month/day/slug.

One thing to notice, which I will likely delve into when I start talking
about my [Micropub client][23], is that when I publish notes via Micropub,
the file name is the current timestamp, to the second.  I want the slug to
be just the time portion of that timestamp, so my code here will extract
that part.  This is *very* specific to my use case and may not be of
interest to many others.

With this configuration in place, every post will end up at a URL directly
off my domain name, with no extra decorations that may end up changing over
time.  My "introduction to the IndieWeb" article, for example, is located at
[/2019/12/08/intro-to-indie-web][22] and I feel like there's a good chance
this URL would survive any reorganization of my website.

## Post Organization - Recap and Implementation

I implied above that my post feed consists of several different kinds of
posts.  I've [written about][15] the different post types recognized by the
IndieWeb community, so I won't rehash that here.  I *will*, however, rehash
the post organization scheme that I eventually settled on for my personal
site:

* **Posts**. My blog entries, notes, and standalone photos.  If you're
  interested in what's going on in my life this would be the feed to follow.
* **Blog**. Just my blog entries, because I'm old-fashioned that way.
* **Bookmarks**.  My "read later" link pile.  Whether or not I actually *read*
  these links later is a separate conversation.  Leave me alone :-)
* **Everything**.  All my posts, including everything I've already mentioned but
  also my likes, reposts, and replies.  IndieWeb enthusiants might be
  interested in this feed.

I wanted all these groups to show up as distinct, subscribable feeds on my
website.  To do this, I made use of a feature of Eleventy called
*[collections][16]*, which allows you to group content in arbitrary ways.

The first step in this process involves identifying the type of each post,
so I can put it in the right bucket.  Is it a blog entry?  A note?  A
bookmark?

One approach I've seen is to just put each type of post into its own
folder - blog entries go into an article folder, notes go into a notes
folder, etc.  This is simple and it works, but it means that you need to
remember to do this every time you create a new post.  I felt that this was
a burden that would discourage frequent posting and, in any case, I
suspected the process could be automated so I opted instead to write some
custom code to handle it.

I ended up writing a simplified (some might say "half-assed") version of the
[post discovery algorithm][17] to attach a special "postType" data attribute
to each post.  The complete post discovery algorithm is meant to work with
the entire gamut of [microformatted][18] posts seen "in the wild" and was
unnecessary in the context of my eleventy setup because we're dealing my
content, on my website, over which I have full control.

So I wrote a postType utility.  As I said, it's geared towards my own
content but my posts are formatted in a vaguely standard manner so I suspect
this code would be at least semi-useful to anyone using Eleventy:

**utils/post-type.js**:

``` javascript
const validUrl = require("valid-url");

/**
 * Returns the "value" of the data passed in.
 * 
 * - if it's an array, use the first item, otherwise just use the item passed in
 * - If there's a "value" field in the data, use that, otherwise just use the value itelf
 *
 */
function getPropValue(value) {
  return value ? getValue(Array.isArray(value) ? value[0] : value) : undefined;
}

function getValue(value) {
  return value.value || value;
}

function postType(item) {
  if (!item || !item.data) {
    return undefined;
  }

  if (item.data.type === "h-event") {
    return "event";
  }

  const props = item.data;
  const propNames = Object.keys(props);

  if (propNames.includes( 'rsvp' ) &&
     (props.rsvp.includes( 'yes' ) ||
      props.rsvp.includes( 'no' ) ||
      props.rsvp.includes( 'maybe' ) ||
      props.rsvp.includes( 'interested' ))) {
    return 'rsvp';
  }

  const impliedTypes = [
    ["repost-of", "repost"],
    ["like-of", "like"],
    ["bookmark-of", "bookmark"],
    ["in-reply-to", "reply"],
    ["video", "video"],
    ["photo", "photo"],
  ];
  
  for (let i = 0; i < impliedTypes.length; i++) {
    const [propertyName, impliedType] = impliedTypes[i];
    if (propNames.includes(propertyName) && validUrl.isUri(getPropValue(props[propertyName]))) {
      return impliedType;
    }
  }

  return item.data.title ? 'article' : 'note';
}

module.exports = {
  postType
}
```

This is a Javascript adaptation of the post discovery function from the
[mf2util python package][19], which I used in the previous version of my
site.  I simplified it for my use case, but opted to leave in the event and
rsvp support in case I ever implement that for my site.

The postType attribute will be one of event, rsvp, repost, like, bookmark,
reply, video, photo, article or note.  It's attached to each post via
Eleventy's [computed data feature][20] - I have a `feed.11tydata.js` file in
my feed folder, alongside the `feed.json` file mentioned above:

**src/posts/feed/feed.11tydata.js**:
``` javascript
const pt = require('../../../utils/post-type.js');

module.exports = {
  eleventyComputed: {
    postType: (data) => pt.postType({data}),
  }
};
```

The next step is to group the posts into my desired feeds.  To this end, I
added this to my `.eleventy.js` file:

**.eleventy.js**:
``` javascript
function postTypes(collection, postTypes) {
  return collection.filter((item) => postTypes.includes(item.data.postType));
}

function getPosts(collection) {
  return collection.getFilteredByGlob("./src/posts/feed/**/*.md").reverse();
}

module.exports = (eleventyConfig) => {
  eleventyConfig.addCollection("all", (collection) =>
    getPosts(collection)
  );

  eleventyConfig.addCollection("posts", (collection) =>
    postTypes(getPosts(collection), ["article", "note", "photo"])
  );
  
  eleventyConfig.addCollection("blog", (collection) =>
    postTypes(getPosts(collection), ["article"])
  );

  eleventyConfig.addCollection("bookmarks", (collection) =>
    postTypes(getPosts(collection), ["bookmark"])
  );
  ...
}
```

Things to note:

* The `getPosts` function returns a reverse-chronologically ordered list of
  all my posts.
* The `postTypes` function will select out all posts with a type that
  matches one of the types in the supplied list.
* Eleventy's collection API, plus the two helper functions mentioned above,
  are used to create the actual feeds that I want.

After that, you just have to hook these collections into your templates.  I
won't get into that here.

## More To Come

This website, as I mentioned before, isn't simple.  In the end, I ended up
writing code to:

* provide tag and archive pages for all my posts
* unfurl external URLs, so we can see context for likes and bookmarks
* receive and display [webmentions][21]
* send webmentions and syndicate posts

It's a lot, and I'm not even getting into the actual *web design* of the
site, which might deserve its own set of posts.

In any case, I hope someone gets something out of this.  Stay tuned to go
further down the rabbit hole.

[1]: /2022/03/06/pelican-to-eleventy
[2]: https://www.11ty.dev/
[3]: https://indieweb.org/
[4]: https://www.11ty.dev/docs/
[5]: https://indieweb.org/article
[6]: https://jekyllrb.com/
[7]: https://indieweb.org/note
[8]: https://indieweb.org/bookmark
[9]: https://indieweb.org/like
[10]: https://indieweb.org/repost
[11]: https://www.11ty.dev/docs/data-template-dir/
[12]: https://www.11ty.dev/docs/filters/
[13]: https://www.w3.org/Provider/Style/URI
[14]: https://www.netlify.com/
[15]: /2020/01/05/website-castle
[16]: https://www.11ty.dev/docs/collections/
[17]: https://indieweb.org/post-type-discovery
[18]: https://indieweb.org/microformats
[19]: https://github.com/kylewm/mf2util
[20]: https://www.11ty.dev/docs/data-computed/
[21]: https://indieweb.org/Webmention
[22]: /2019/12/08/intro-to-indie-web
[23]: https://github.com/drivet/micropub-git-server
