---
date: 2026-01-10T21:20:53-05:00
title: "Moving My Bookmarks to My Garden"
blurb: "My bookmarks were always a bit dicey"
post-status: draft
mp-syndicate-to:
- mastodon
tags:
- indieweb
- emacs
- orgmode
---

I've previously written about the [idiosyncrasies of IndieWeb bookmarks][1],
and how I consider them to be qualitatively different from other
[IndieWeb][7] posts.  The differences boil down to my relationship with
them.  For me, they are primarily a collection of references, not a way I
interact with the world.  Unlike notes or articles, sharing them isn't a
priority; I don't particularly care about shouting my bookmarks from the
digital soapbox that is my blog.

Other people obviously differ here, and I'm vaguely aware of the notion of
social bookmarking, and now defunct services like [delicious][2], but that's
just not how I use them.  My bookmark posts always felt a bit out of place
on my website, much more subject to whims of deletion and reorganization
than other forms of content.

I've been wanting to revamp them for a while.  But how?

## Emacs...Again

In the end I decided, unsurprisingly (if you know me), to leverage Emacs,
Org mode and my [digital garden][3] to manage my bookmarks, much as I did
with my [reading list][4].  It turns out that using Org mode to maintain a
pile of links is quite common and there are a number of articles on the
subject.

Most of the information seems to revolve around [org-cliplink][5], a useful
little package that takes a URL on the clipboard, parses out the HTML to
extract the title, and inserts it as an org hyperlink into your current
buffer.  It's actually quite straightforward to build a simple Org mode
based bookmarking system around this functionality, taking advantage of Org
mode's built-in tagging functionality to organize them.

One thing that I gave up in the name of expediency is [unfurling the shared
link, as I do on my blog][6].  My unfurling system uses a bespoke cocktail
of Open Graph data, favicons and Microformats that I feel works pretty well
across a spectrum of IndieWeb and non-IndieWeb sites.  For whatever reason,
I wasn't able to find any Emacs packages for extracting this metadata and
since I wasn't willing to write any of it myself at this time, I simply did
without.

One approach that I briefly considered was extracting my unfurling code into
a standalone REST service and using an Emacs JSON package to interact with
it, but while I haven't completely dismissed that possibility for the
future, it felt like too much work for what I was trying to accomplish,
which was a simple replacement for managing my bookmarks.  In any case,
integrating icons and images into an Org mode file in a way that looks
reasonable while editing the text but which *also* exports to HTML that
doesn't look like garbage is no picnic, as [creating my reading list][8]
taught me.

All this to say is that, in the end, I created a [very simple bookmarks
page][9] on my digital garden, and removed the corresponding links on my
blog.  I *also* removed the Lifestream feed since I felt it was becoming a
bit redundant and I *also* felt a better argument could be made for
including all my posts by default once the bookmarks were gone.

Will this last?  Who can say?  To be fair, it's not like I was making much
use of the bookmarks on my blog when they were there.  Maybe this will teach
me to be more aware of what I record.  Time will tell, I suppose.

[1]: https://desmondrivet.com/2022/03/01/bookmark-thoughts.html
[2]: https://indieweb.org/Delicious
[3]: https://garden.desmondrivet.com/
[4]: https://garden.desmondrivet.com/books.html
[5]: https://github.com/rexim/org-cliplink
[6]: https://desmondrivet.com/2022/04/04/eleventy-previews.html
[7]: https://indieweb.org/
[8]: https://desmondrivet.com/2025/09/28/reading-list-in-emacs
[9]: https://garden.desmondrivet.com/links.html
