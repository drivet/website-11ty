---
title: From Pelican to Eleventy
blurb: Making the switch from Pelican to @eleven_ty
date: '2022-03-06T12:08:25-05:00'
tags:
  - pelican
  - eleventy
  - blogging
---

After roughly [five years][1], I've finally decided to switch my blog over
from [Pelican][2] to [Eleventy][3].  I'd be lying if I said that I wasn't at
least partially swayed by the fact that all the cool kids seemed to be doing
it but, with that in mind, I do have some (hopefully) legitimate reasons for
making the switch.

Pelican bills itself as a static site generator, and so it is, but one that
focuses more on traditional, single user blogs.  Pelican *can*, of course,
be used as a CMS, and you *can* certainly use it to create arbitrary,
standalone pages and websites, but the features in support of this usage
seem more bolted on than organic.  There are certain assumptions built into
Pelican that stem from its original purpose as a blogging engine that are
occasionally irritating to circumvent.

For example, Pelican assumes that your website consists of a single blog
(with the attendant pages that you'd expect to go along with it, like tag,
archive, and article listings), perhaps alongside a collection of arbitrary,
standalone pages, and the template system revolves around this dichotomy.
It doesn't really support the idea of multiple, independent streams of
content - at least, not without some kind of plugin to make it work.

Add to this the fact that Pelican enforces titles on all its blog posts and
it's immediately clear that using it in an [indieweb][4] context is somewhat
of an uphill battle.  The indieweb divides personal content up into
different post types, most commonly [articles][5] (standard blog entries)
and [notes][6] (short, titleless posts reminiscent of tweets) and it's
natural to want to provide separate views for these.  I ended up hijacking
Pelican's notion of a "category" to manage it - articles were one category,
notes another - but that seemed hackish from the get go.  Of course, that
meant that I had to get rid of the categories I already had (like
"technical" and "travel"), and for a while I mitigated this by using a
"subcategories" plugin, but I eventually just got rid of them entirely since
they ended up being more trouble than they were worth.

Eleventy doesn't have these problems to quite the same extent because
Eleventy really is an arbitrary static site generator, not a blogging
engine, and is therefore less hampered by built-in assumptions about how it
will be used.  You can certainly make a blog with Eleventy, but this is less
of an explicit, baked-in feature and more of a particular way of using the
software.

To put it another way, Eleventy feels set up for easy hacking out of the box
and this makes it quite nice to use, even when you find yourself having to
program a lot of the missing features yourself.

I'll hopefully have more to say about this later when I write up some notes
on how I got my not-so-simple indieweb enabled website to play nice with
Eleventy.  For now, suffice it to say that the experience wasn't horrible.

[1]: /2016/12/12/pelican-move
[2]: https://blog.getpelican.com/
[3]: https://www.11ty.dev/
[4]: https://indieweb.org/
[5]: https://indieweb.org/article
[6]: https://indieweb.org/note
