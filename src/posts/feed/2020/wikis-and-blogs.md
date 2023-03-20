---
blurb: 'Digging into the differences between wikis and blogs #indieweb'
date: 2020-05-01T10:24:25.000Z
modified: 2020-05-01T10:24:25.000Z
tags:
  - blogging
  - indieweb
title: On Gardens and Streams
mp-syndicate-to:
  - twitter
---

I attended my first [IndieWebCamp session][1] last week, on the subject of
"gardens and streams", otherwise known as wikis and blogs.  Given the
current [global situation][2], the entire thing was remote; I participated
via Zoom.  It was fun!  I'm glad I got to meet everyone. 

Wikis, and how they differ from blogs, is a topic that interests me.  You
may not know it, but my domain sports a [wiki][3], powered by [MoinMoin][4].
I mostly use it to store technical notes and recipes.

I'm no historian, but it seems obvious that wikis were created mostly in
order to make certain kinds of websites easier to build and maintain.  In
the days when "webmaster" was an actual job title and when traditional
websites were maintained by an elite group of technical people, using arcane
languages like HTML, wiki powered websites:

 * were collaborative.  Anyone could create or edit a page, from the wiki
   site itself, and a page often had many authors, with many revisions.
 * tended not to use HTML directly, opting for their own text-based markup
   systems.
 * made it easy to link between pages, allowing users to create sprawling,
   hyperlinked knowledge bases - hence the *garden* metaphor.
 * made it easy to see who had edited a page, and what revisions were made.

Wikis, in other words, made websites *democratic*.

Blogs became popular around roughly the same time as wikis.  As a class of
website, they were not entirely unlike wikis, but were perhaps less well
defined in what features they were meant to offer.  About the only hard
requirement of a piece of blogging software is that it had to manage
chronologically ordered content - or *streams* of content, to use the
ongoing metaphor.  Blogs are basically just online diaries or journals,
presenting a feed of timestamped posts to the world at large.

All other blogging software features are negotiable.  Many will incorporate
WYSIWYG editors, others let you use text-based markup, while still others
make you write all your posts in HTML.  Most let you tag a post, but some
very basic ones don't.  Many have online user interfaces allowing you to
manage your content while others (like any static site generator, for
example) do not.

Notably, blogs tend to focus on a single author.  That isn't to say that
many blogs don't offer multi-author features, but I would argue that this is
not the standard use case.  Unlike a wiki, *collaboration* is not a defining
characteristic of blogging software.

## Collapsing the Differences 

[Martin Fowler's][5] main web presence consists of what he calls a
[bliki][6], a combination of a blog and a wiki.  Like a wiki, it very easily
allows you to throw up informal pieces of content using simple text based
markup.  Like a blog, it allows you to easily see chronologically ordered
content.

Interestingly, he's the only editor of his own site, so the collaborative
aspect of wikis is not a feature of his bliki.

And this leads to an interesting discussion on the differences between blogs
and wikis when you're the only one managing the site.  For many people, the
whole *point* of a wiki is that it's collaborative.  If you're the only
editor, what's the point?  Why wouldn't you just use a blog - albeit,
perhaps, a reasonably powerful one that incorporates wiki-type features like
text-based markup and easy page creation?  The differences between the two,
in that case, tend to melt away.

It's a good question.  It's an *especially* good question if, like me, you
maintain both a blog and wiki.  Why on earth would anyone do that?

## When to Plant and When to Row

As mentioned, there is at least one thing that a blog, by nature, does out
of the box that a wiki does not: it displays a reverse chronological listing
of all its posts.  Not only does a wiki not do this, it probably doesn't
make *sense* for a wiki to do this.  But why not?

And the answer to that question cuts right to the heart of the matter.  Blog
posts are, by nature, timestamped.  They have a publication date.  In a
sense, the publication date of a blog post is part of its content.  Blog
posts exist at a particular point in time.

Most blogging software will obviously allow a user edit a blog post after
it's published (to correct spelling errors, for example) but it's generally
understood, because of the very nature of blogging, that any edits to a post
after the fact should be minor.  One would never edit a blog post so much
that the publication date suddenly becomes a lie.

A wiki page isn't like that and its publication date usually isn't a very
interesting piece of information.  Wiki pages, the flowers and weeds of your
garden, exist outside time - outside the stream.  So what kind of content
fits this mould?  Paradoxically, it's two seemingly opposite types: what
I'll call the *half baked* and the *perfectly baked*.

Yes, this is my blog and I'll use baking metaphors if I want to!

Half baked pages are those which undergo numerous revisions.  Think of
personal notes, or biographical information, or a resume.  These kinds of
pages exist to be edited frequently and enthusiastically.  The publication
date for these pages are not useful, for obvious reasons.

Perfectly baked pages, on the other hand, are those that are not edited
frequently, because the information in them is "timeless" in some sense.  If
the information in a page is timeless then, obviously, the publication date
is probably not useful.  An example I often use is a recipe page.  No one
needs to know when I copied my preferred recipe for [Cranberry Orange
Buttermilk Muffins][7] to my wiki.  They just need to know how to make them
(and you *should* make them; they're delicious).  A non-food example would
be my [Certificate Management Basics][8] page, which I use to remind myself
how basic cryptography works.

This, then, is the reason I have both a blog and a wiki.  In essence, I use
my wiki for notes and as a personal knowledge base, a purpose for which it
excels.  My blog wouldn't be an appropriate place for this kind of content;
the world doesn't need to be notified when I've added a barely coherent
guide on how to use [x11vnc][9], nor does it need to know when I've recorded
a new [chili recipe][10].

## Further Exploration

So where to go from here?

Well, for one thing, I'll admit that my [wiki][3] is getting a bit long in
the tooth - it's just kind of ugly.  The IndieWebCamp session dropped some
interesting new pieces of software to try out, like [TiddlyWiki][11], so I
may end up trying some of those.  We'll see.

The session also just dropped some interesting use cases for wikis, like
second brains, commonplace books and mind palaces.

Anyway, lots of food for thought!

[1]: https://chrisaldrich.wordpress.com/2020/04/17/gardens-and-streams-wikis-blogs-and-ui-a-pop-up-indiewebcamp-session/
[2]: https://en.wikipedia.org/wiki/Coronavirus_disease_2019
[3]: https://wiki.desmondrivet.com
[4]: https://moinmo.in/
[5]: https://martinfowler.com/
[6]: https://martinfowler.com/bliki/WhatIsaBliki.html
[7]: https://wiki.desmondrivet.com/CranberryOrangeButtermilkMuffins
[8]: https://wiki.desmondrivet.com/CertificateManagementBasics
[9]: https://wiki.desmondrivet.com/VncBasics
[10]: https://wiki.desmondrivet.com/VegetarianChili
[11]: https://tiddlywiki.com/
