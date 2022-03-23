---
title: "Taming Eleventy Tags: Or How I Learned To Tolerate Double Pagination"
date: 2022-03-23T15:33:00-04:00
mp-syndicate-to:
- twitter_bridgy
tags:
- blogging
- eleventy
---

[Pagination][1] in [Eleventy][2] is bit strange.  Out of the box, Eleventy
comes with support for something the developers *call* "pagination", but it
might not be the sort of pagination you're used to.

As you probably already know, Eleventy is a static site generator.  It
generates output HTML files from input template files.  In the simplest
case, one HTML file is generated for each (non-[layout][3]) input template.

The feature known as *pagination* in Eleventy is basically a way of
getting around that limitation, allowing you to generate multiple
output files from one template file.  If you squint, you can see how
this maps to the traditional idea of pagination (i.e. we want to break
down a long list of items into smaller sized pages, writing only one
template for the page) and although this is, indeed, a common way to
use pagination in Eleventy, it turns out that the feature is much more
generic than you might think - and, paradoxically, less useful than
you might think.

## Enabling Vanilla Pagination

Imagine you have a [collection][4] called "posts" with 50 items in it.  You
have a template to display them all:

{% raw %}
``` markdown
<ul>
{%- for item in collections.posts %}
  <li>{{ item }}</li>
{% endfor -%}
</ul>
```
{% endraw %}

This will generate one output HTML for all 50 items. This is rather
long so you decide you want to break it up into pages - let's say 10
items per page, for a total of 5 pages.  It turns out this is really
easy to do in Eleventy.  You just add a pagination attribute to your
front matter and change your template a bit.

{% raw %}
``` markdown
---
pagination:
  data: collections.posts
  size: 10
permalink: "page-{{ pagination.pageNumber }}.html"
---
<ul>
{%- for item in pagination.items %}
  <li>{{item }} </li>
{% endfor -%}
</ul>
```
{% endraw %}

The pagination attribute, as specified above, tells Eleventy to break
up the items in collection.posts into chunks of size 10, and to
generate an output page for each chunk.  A pagination object will be
created for each page allowing you to access the pagination specific
attributes.

In this case, from the permalink attributes, you can guess that the output
files will be called `page-0.html`, `page-1.html`, etc.

## The Tagging Problem

So far, so good.  This works for any collection you can *explicitly* name in
the data attribute.  But what happens if you don't know the collection name
ahead of time?

This can happen, for example, if you've implemented custom taxonomy on your
blog - a fancy way of saying that you've tagged your blog entries with
arbitrary labels.  You probably don't know all your tags ahead of time, and
even if you did you certainly don't want to explicitly write a template for
each tag.

In fact, with Eleventy, the issue goes even deeper than that.
Pagination aside, if you've implemented a general tagging system for
your blog, how do you even go about generating a page of posts for
each tag without writing a template per tag?  It's not entirely
obvious.

It turns out that the solution to this problem is...pagination! [But
probably not the kind you're used to][5].  When I said that pagination in
Eleventy is a general-purpose way of cranking out multiple output files from
one template, I wasn't kidding.  And in Eleventy, you can paginate through
an *object*.  What does that even mean and how does it help us?

As you may know, objects, in JavaScript, are just collections of
attribute/value pairs.  For the purposes of pagination, Eleventy lets you
treat the object as a collection of keys and produces output pages
accordingly.  If you set your chunk size to 1, then you're effectively
creating one page per attribute.

So, if you have an object that maps your tags (as attributes) to the
collection of posts corresponding to that tag (as values) you can use the
object pagination feature to produce a page per tag without you having to
know all your tags in advance.

{% raw %}
``` markdown
---
pagination:
  data: collections.postsByTag
  size: 1
  alias: tag
permalink: /tags/{{ tag }}/
---
<h1>Tagged "{{ tag }}"</h1>

<ul>
{% set taglist = collections[ tag ] %}
{% for post in taglist | reverse %}
  <li><a href="{{ post.url | url }}">{{ post.data.title }}</a></li>
{% endfor %}
</ul>

```
{% endraw %}

This actually works, and it's pretty clever, but there's a snag.

## The Double Pagination Problem

"Aha!", I hear you cry!  "But what if I want to *paginate each tag page*?"

And, indeed, this is a non-trivial problem to solve with Eleventy.
Your tag page template is already using the pagination feature to
generate the individual tag pages themselves.  There's no obvious way
to tell it that the those tag pages should *also* be paginated into
chunks of a certain size.

It turns out that the way out of this problem is to basically
side-step the vanilla pagination feature of Eleventy and do it
yourself:

``` javascript
const _ = require('lodash');

function classify(collection, tagExtractor) {
  const classified = {};
  collection.forEach((item) => {
      const tags = tagExtractor(item);
      if (tags.length === 0) {
          return;
      }

      tags.forEach(tag => {
        if (!classified[tag]) {
          classified[tag] = [];
        }
        classified[tag].push(item);
      });
  });
  return classified;
}

/**
 *
 * Return object map of tag -> posts
 */
function indexByTag(collection) {
  return classify(collection, item => item.data.tags || []);
}

/**
 *
 * Take a map of tags to items, return flat list of paged item objects like this:
 * { tagName, pageNumber, items }
 *
 * page starts from 0
 */
function flatPaginate(indexedCollection, size) {
  const pages = [];
  for(let tagName of Object.keys(indexedCollection)) {
    const pagedItems = _.chunk(indexedCollection[tagName], size);
    for(let pageNumber = 0; pageNumber <  pagedItems.length; pageNumber++) {
      const page = {
        name: tagName,
        page: pageNumber,
        total: pagedItems.length,
        items: pagedItems[pageNumber]
      };
      pages.push(page);
    }
  }
  return pages;
}

eleventyConfig.addCollection('tagPages', (collection) =>
    flatPaginate(indexByTag(collection), 10)
);

```

There's a lot going on here, so I'll try and explain.  First we take our
collection and produce a map of tags to posts - this is what `indexByTag`
does.  The `indexByTag` function uses a somewhat generic `classify` function
because I also use this code to produce pages for my archives (archiving is
kind of like tagging except with years, months and days instead of tags).

Then we take this map of tags to posts and run it through `flatPaginate`
along with a page size (in this case 10).  Using the lodash `chunk`
function, which seems specially designed for this use case, we produce a
flat list of pseudo pagination objects that give us everything we need to
render a paginated tag page.

The pagination object looks like this:

``` json
{
    name: '<tag name>',
    page: <page number>,
    total: <total number of pages for the tag>,
    items: [<array of items for the tag and page number>]
}
```

We can render the tag pages with a template like this:

{% raw %}
``` markdown
---
pagination:
  data: collections.tagPages
  size: 1
  alias: page
permalink: /{{ page.name }}-{{page.page}}.html
---
<h1>Tagged {{ page.name }} (Page {{ page.page }})</h1>

<ul>
{% set taglist = page.items %}
{% for post in taglist | reverse %}
  <li><a href="{{ post.url | url }}">{{ post.data.title }}</a></li>
{% endfor %}
</ul>

```
{% endraw %}

Effectively, we're enumerating a flat list of tag pages across two
dimensions - the tag itself, and the page number - and rendering each
combination in a new output page.

Complicated, right?  This is what I meant when I said that Eleventy
pagination is both more and less useful than you might imagine.  It's
flexible in that it lets us do what we need to do, but implementing some common
use cases is a bit tortuous.  Still...it works, and it makes sense once you
understand it - but you *do* have to understand it.

One silver lining is that I can adapt this general approach to produce my
archive pages as well - you just have to treat years, months and days as if
they were tags.  Here's the full (though simplified and, honestly, kind of
ugly) code:

``` javascript
const _ = require('lodash');

function getYear(date) {
  return date.getFullYear().toString();
}

function getMonth(date) {
  const month = date.getMonth() + 1;
  return (month < 10 ? '0' : '') + month;
}

function getDay(date) {
  const day = date.getDate();
  return (day < 10 ? '0' : '') + day;
}

function classify(collection, tagExtractor) {
  const classified = {};
  collection.forEach((item) => {
      const tags = tagExtractor(item);
      if (tags.length === 0) {
          return;
      }

      tags.forEach(tag => {
        if (!classified[tag]) {
          classified[tag] = [];
        }
        classified[tag].push(item);
      });
  });
  return classified;
}

/**
 *
 * Return object map of tag -> posts
 */
function indexByTag(collection) {
  return classify(collection, item => item.data.tags || []);
}

function indexByDates(collection) {
  return classify(collection, item => [
    getYear(item.date),
    getYear(item.date) + '/' + getMonth(item.date),
    getYear(item.date) + '/' + getMonth(item.date) + '/' + getDay(item.date)
  ]);
}

/**
 *
 * Take a map of tags to items, return flat list of paged item objects like this:
 * { name, page, items }
 *
 * page starts from 0
 */
function flatPaginate(indexedCollection, size) {
  const pages = [];
  for(let tagName of Object.keys(indexedCollection)) {
    const pagedItems = _.chunk(indexedCollection[tagName], size);
    for(let pageNumber = 0; pageNumber <  pagedItems.length; pageNumber++) {
    const page = {
        name: tagName,
        page: pageNumber,
        total: pagedItems.length,
        items: pagedItems[pageNumber]
    };
    pages.push(page);
    }
  }
  return pages;
}

eleventyConfig.addCollection('tagPages', (collection) =>
  flatPaginate(indexByTag(collection), 10)
);

eleventyConfig.addCollection('archivePages', (collection) =>
  flatPaginate(indexByDates(collection), 10)
);

```

Its not the nicest code, and there are lots of ways to clean it up, but this
works for now.

## Musings and Comparisons

On the one hand, Pelican tagging and pagination was a lot easier to
configure than Eleventy tagging and pagination.  On the other hand,
the code detailed in this article forms the core of a relatively
complete and flexible tagging and archiving system.

Under Pelican, for example, I basically had to give up tagging my
notes, because I didn't want my note tags to pollute my blog tags, and
there was no real way to keep them apart.  Using the code in this
article as a foundation, I can implement separate tagging for notes,
articles, bookmarks - basically any indieweb post type.  Pelican,
though easier to configure, did not have that kind of flexibility, so
I guess I would have to say that Eleventy wins here, even if I had to
write a fair bit of code to get it to work.

Anyway, if you've read this far, I'm impressed.  Stay tuned.  There's
much more to come.


[1]: https://www.11ty.dev/docs/pagination/
[2]: https://www.11ty.dev/docs/
[3]: https://www.11ty.dev/docs/layouts/
[4]: https://www.11ty.dev/docs/collections/
[5]: https://www.11ty.dev/docs/quicktips/tag-pages/
