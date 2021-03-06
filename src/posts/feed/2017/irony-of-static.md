---
date: 2017-02-26T02:35:00.000Z
modified: 2017-02-26T02:35:00.000Z
tags:
  - blogging
  - pelican
title: On the Irony of Using Static Site Generators
---

As I've [mentioned before][1], I've recently switched to [pelican][2] as my
blogging engine.

Pelican is a static site generator.  This means that it generates static
HTML files using templates and content as input, which can be then uploaded
(via [rsync][3], for example) to a plain vanilla web server (I use
[nginx][4]).

So far the experience has been fairly smooth.  The web server setup is much
simpler, since there's no application to run. And it's forced me to re-think
what kinds of information I want on my pages. For example, in an effort to
avoid regenerating the entire site every time I publish an article, I
created standalone index pages for tags, categories and archives rather than
display the counts on every page.

That being said, the move does come with some drawbacks, in that some common
features are a bit more difficult to implement.  One example is comments;
because the pages are static, you cannot simply tack on the comments when
the page is generated. Not efficiently, at least - it would require
regenerating a batch of HTML pages every time someone added a comment, which
isn't very scalable past a certain level of traffic.

(Interestingly, some people do exactly that - they regenerate their entire
site when a new comment is submitted.  And though I certainly don't get
nearly enough traffic on my blog to have any real concerns about the
efficiency of this procedure, it's still an assault on my sense of
aesthetics).

The main workaround is to use a third party service like [Disqus][5], or a
self-hosted solution like [Isso][6] (my preference).  Both these solutions
make up for the lack of dynamism in how the pages are generated by adding
extra dynamism to the pages themselves in the form of Javascript.

Full text search is another feature that requires a bit of work to implement
in a statically generated site.  Some people use a third party service like
Google to do it.  Others, like myself, implement it (once again) via
Javascript, using a package like [tipue search][7].

There is a certain irony in using these techniques. With [YAWT][8], I could
brag (after a fashion) that my site was usable across *all* browsers -
including text only ones like [elinks][9].  To be sure, you'd have to turn
off the CSS and you wouldn't be able to use the fancy bootstrap menus, but
things like full-text search and comments still, at the very least,
*functioned* - even with Javascript disabled.

Now that the pages of the site are generated statically, the serving of them
is much more efficient but *you now need Javascript to do simple things like
reading comments, or searching the site*.

Is this a big deal?  Objectively, of course not - I don't think I've ever
seen someone access my site with a text only browser.  But I can't shake the
feeling that I've lost something, that I've given something up.

There are things I can do, of course.  I could, for example, implement my
own full text search index (probably using something like [whoosh][10]),
updated on every compile, paired with a dynamic web application to perform
the search itself.  It wouldn't be difficult - pelican has a plugin system
that makes such things possible.  For the comments I could simply hack up a
dynamic web application to provide [CRUD][11] access to the comments
themselves, running alongside the traditional Javascript support.

Like I said, it's doable - but would I ever actually go to the trouble?
Probably not.  But I'm still allowed to feel a little sad.

[1]: /2016/12/12/pelican-move
[2]: https://blog.getpelican.com/
[3]: https://rsync.samba.org/
[4]: https://www.nginx.com/resources/wiki/
[5]: https://disqus.com/
[6]: https://posativ.org/isso/
[7]: http://www.tipue.com/search/
[8]: https://github.com/drivet/yawt
[9]: http://elinks.or.cz/
[10]: https://pypi.python.org/pypi/Whoosh/
[11]: https://en.wikipedia.org/wiki/Create,_read,_update_and_delete
