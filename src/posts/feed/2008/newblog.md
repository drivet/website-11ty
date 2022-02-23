---
date: 2008-04-07T00:00:44.000Z
modified: 2015-05-16T05:52:58.976Z
tags:
  - blogging
  - yawt
  - blosxom
  - perl
title: New Blog
---

So I got myself a new weblog, hosted on [Netfirms][1].  I'm not particularly
enjoying the Netfirms experience, but it was only $10 for a year and I get
SSH access.  They're quite skimpy with the perl modules, forcing me to
install a bunch of stuff in my home directory.

I'm using a homemade blogging program because I got tired of [blosxom's][2]
ugliness and I didn't really like what I saw of [Bryar][3].  I'm calling it
YAWT in my head - short for Yet Another Weblog Tool.  I'm hoping to extend
it at some point to handle photo galleries in a seamless and efficient
manner - my last attempt to write an online photo manager was quite
slow. We'll see.

YAWT is what you might call "blosxom-inspired", like Bryar. The
entries are regular text files and categorization is done via
directories on disk. The main program is fairly light on
features. More features can be added via plugins, in a style similar
to blosxom. The URL interpretation is almost identical to blosxom as
well.

One big difference between YAWT and blosxom is that YAWT uses the [Template
Toolkit][4] for its templating functionality.  I frankly thought blosxom's
idea of templating was a joke.  Another fairly big difference is that YAWT
does not read all the blog entries into memory once at the start of the
program.  That seemed wasteful to me. We'll see if this turns out to be such
a good idea - reading all the entries into memory allows for more flexibilty
as to the processing that happens later.

For the moment, either in core functionality or via plugins, YAWT
supports:

* Flavours, as in blosxom.  Non-blosxom users might know this idea
  better as "themes".

* RSS 1.0 and 2.0 at the category level, including, of course, the root
  category (i.e. the entire blog).

* Non-threaded comments.

* Trackbacks.

* Displayable category and archive counts.

* Date based permalinks.

* Markdown syntax for the entries


Several features are planned for phase 2:

* Tagging

* Full text search, probably via
  [Plucene](http://search.cpan.org/dist/Plucene/) though I'm not 100%
  decided yet.

* Threading and RSS support for the comments.

I will probably write more about YAWT in detail at a later date,
assuming I haven't made too many horrible design decisions and throw
the whole thing out in disgust.

[1]: http://www.netfirms.ca/
[2]: http://blosxom.sourceforge.net/
[3]: http://search.cpan.org/~dcantrell/Bryar/lib/Bryar.pm
[4]: http://template-toolkit.org/
