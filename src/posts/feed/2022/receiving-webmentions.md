---
date: 2022-04-25T08:37:06-04:00
title: "Receiving and Displaying Webmentions in Eleventy"
blurb: "It turns out other people have done most of the work for displaying webmentions in #Eleventy"
mp-syndicate-to:
- twitter
tags:
- eleventy
- indieweb
---

Webmentions are the way [people have conversations on the Indieweb][1], but
integrating them - or, indeed, any kind of dynamic content - into a static
site can be a little tricky.  Luckily, many people have run into this issue
before and so there wasn't much for me to do but steal.

To be clear, I'm talking about *receiving* and *displaying* webmentions
here, not *sending* them.  How I ended up sending them will be a separate
topic, as it is tied into how I manage [POSSE][9] and [backfeeding][10] on
my site.

The previous, pelican-based incarnation of my blog used a [homegrown
system][7] involving a webmention listener that handed off data to a
[redis][8] server and ultimately resulted in a new webmention being
committed to my site's repo, causing the whole thing to be rebuilt.  It
certainly worked, but it required a fair bit of maintenance.  It also failed
to take into account certain edge cases that I hadn't previously considered
(like a commentator's avatar image going missing).

This prompted me to follow [Max Bock's][2] [lead][3] and use
[webmention.io][4] to host all my webmentions.  This has a couple of
advantages:

 * The service is easy to use.  You just need to sign up with IndieAuth,
   point your webmention endpoint at the site and you're good to go.
 * By default it will cache images from the webmention so that they won't
   break, a feature that I didn't fully understand I needed when I stated
   integrating webmentions into my site.
 
My webmention integration scheme is effectively almost a complete copy of
Max Bock's, so it doesn't make sense to go into too much detail here - just
read his [article on the subject][3].  I'm basically doing the same thing:

 * I load a webmention cache using [Eleventy's global data feature][5].
 * I read all new webmentions from webmention.io and merge them into the now
   in memory cache.
 * I save the cache to disk, but keep a copy in memory.
 * I use this cache to display webmentions for the posts of my blog.
 * I [persist the cache][6] between Netlify builds

There are a couple of things that I still have to do, if I ever get around
to it:

 * Relationships between webmentions are not clear.  For example, if one
   webmention is a reply to another one in the context of one of my blog
   posts, they both get displayed as independent mentions, with no
   indication that there's a connection between the two.
 * If I mention one of my own blog posts, I'll end up displaying a
   webmention for it.  Probably not a great idea.

## Displaying Webmentions

How one displays webmentions on one's site is probably a whole topic unto
itself.

Some people don't bother differentiating and lump all types of webmentions
into one big bucket.  Others divide it up into different types.

I mostly fall into the latter category, though from a UI perspective I
haven't fully settled on how to convey the information.  In any case, for
now, I've chosen to group my webmentions into:

 * Likes
 * Reposts
 * Bookmarks
 * Leftover mentions, which includes replies but also arbitrary mentions
   that aren't so easily classified.

## More To Come
 
I mentioned before that receiving webmentions and sending webmentions were
effectively two different problems, and there are some subtleties to the
latter that really deserve their own article.  I suspect that's going to be
the next article in this series.  Stay tuned.


[1]: /2020/04/04/indieweb-conversation
[2]: https://mxb.dev/
[3]: https://mxb.dev/blog/using-webmentions-on-static-sites/
[4]: https://webmention.io
[5]: https://www.11ty.dev/docs/data-global/
[6]: https://mxb.dev/blog/persistent-build-folders-netlify/
[7]: https://github.com/drivet/webmention-git-server
[8]: https://redis.io/
[9]: https://indieweb.org/POSSE
[10]: https://indieweb.org/backfeed
