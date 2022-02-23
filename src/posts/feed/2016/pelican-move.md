---
date: 2016-12-13T03:06:00.000Z
tags:
  - blogging
  - pelican
title: Moving to Pelican
---

After a good run, I've decided to retire [YAWT][1] for the time being.  I
found the workflow I set up to be a bit complicated, and I found myself
wanting to concentrate more on writing rather than the nitty-gritty of
publishing.  I also got a bit tired of the lone wolf thing and I wanted to
try using piece of software that other people actually use.

But I still liked the basics of my workflow.  I still liked writing blog
entries with a standard text editor (Emacs in my case).  I still liked the
idea of keeping my site under source control, as plain files.  In other
words, I did not want to switch to [Wordpress][2].

[Jekyll][3] was a contender for a little while, as it is very popular (27859
stars on github as of this writing), but it uses ruby and I had heard that
the [templating system][4] wasn't particularly powerful, so I passed.

Eventually I decided to try [pelican][5], since it uses python and the
[Jinja2][6] templating engine - both of which are used by YAWT, so I figured
it wouldn't be too much of a leap.

Pelican, like jekyll and several other similar pieces of software, is a
static site generator.  The idea is to compile your site into HTML files
that you can just rsync or otherwise copy to your webserver of choice.  It's
an old idea; at some point in the deep past (many, many, many months ago, as
these things are measured in the tech world) I used [WML][7] to compile my
own website so it's not an idea with which I'm completely unfamiliar.

It does, however, go against the design of YAWT, which generated the pages
dynamically.  Dynamic site generators (which comprises most of the blog
software world) make dynamic features like full text search and comment
tracking easier.  While such features are not impossible with a static site
generator, they are often a bit more difficult to set up (and, perhaps
ironically, require a bit more dynamism in the webpage itself, i.e. more
Javascript).  Of course, I can't honestly say that I was really taking full
advantage of the dynamic nature of YAWT on my blog; though I had implemented
full text search, I did not implement a commenting system, and there was no
way to add new blog entries except by committing them to a github
repository.  So I figured the missing features wouldn't be *too* glaring in
pelican (though, as of this writing, I still haven't added full text
search).

I did, however, end up writing two pelican plugins to cover other bits of
missing functionality that I was used to having in YAWT and that weren't
incompatible with a static site generator.  The [first one][8] generated
article summaries that tried to honour the configured word count but which
always ended on a paragraph break.  The [second one][9] generated my
[preferred permalink scheme][10] in parallel to the standard category based
scheme.

I also wasn't overly thrilled with the available themes from
[http://www.pelicanthemes.com/][11].  I ended up more or less settling for
[pelican-bootstrap3][12], but with some fairly significant tweaks.  The
result is a theme that I'm calling [pelican-website][13], which is geared
towards blogs that are just one part of a larger context, i.e. websites with
perhaps a bit more standalone pages than average.

Anyway, we'll see how this goes.  I may end up moving back to YAWT.  Or I
may end up writing more plugins for pelican.  Who knows?

[1]: https://github.com/drivet/yawt
[2]: https://wordpress.com/create/
[3]: https://jekyllrb.com/
[4]: http://shopify.github.io/liquid/
[5]: http://blog.getpelican.com/
[6]: http://jinja.pocoo.org/docs/dev/
[7]: http://thewml.org/
[8]: https://github.com/drivet/paragraphed-summary
[9]: https://github.com/drivet/pelican-cool-uri
[10]: https://www.w3.org/Provider/Style/URI.html
[11]: http://www.pelicanthemes.com
[12]: https://github.com/DandyDev/pelican-bootstrap3
[13]: https://github.com/drivet/pelican-website
