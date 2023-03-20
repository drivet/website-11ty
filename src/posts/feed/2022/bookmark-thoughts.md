---
date: 2022-03-01T13:57:07-05:00
modified: 2022-03-01T13:57:07-05:00
title: 'Much Ado About (Indieweb) Bookmarks'
blurb: 'The ins and outs of owning your bookmarks on the #indieweb'
tags:
  - indieweb
mp-syndicate-to:
  - twitter
---

Like many Indieweb participants, I post [bookmarks][1], [likes][2] and
[reposts][3] to my website.

From a technical standpoint, there's virtually no difference between these
kinds of posts; they're all permalinked [h-entries][4] which house another,
usually external, URL and which often contain no other content beyond that
link.  They differ solely in the [microformat][5] which annotates the link,
indicating the *intent* of the post.

And intent make all the difference here.  A *like* is supposed to convey
that the author actually enjoys or appreciates the content to which the post
is linking.  A *repost* is supposed to convey that the author wishes, for
some reason, to share that content on their own feed.  A bookmark is
supposed to convey that the author...ugh...err...

Wait...what *is* a bookmark supposed to convey, exactly?  Other than that
the author...you know...bookmarked a link?

It's clear that the semantics of a bookmark are a bit "mushier" than those
of a like or repost.  Likes and reposts, to some extent, reveal the author's
disposition to the link; bookmarks don't really reveal anything other than
that the author thought the URL was in some way "interesting" enough (for
some definition of "interesting") to refer back to later.

According to the [wiki page][1], common uses of bookmarks include

* building a "read later" list
* building up a digital library of online content
* sharing interesting content with others

Depending on what you want to get out of your bookmarks, your relationship
to them will be different.  If the sharing aspect is important to you, then
posting them to your site is a core feature, and syndicating them to Twitter
or Reddit makes a lot of sense.  If, on the other hand, you're like me and
you're just using them to mark articles so that you can read them later,
then sharing them publicly might be less important, and you may not want to
syndicate them at all.  In that case, the bookmarks themselves might be a
lot more ephemeral than other kinds of posts, if you choose delete them
after you read them.

This all suggests, in my opinion, that bookmarks are fundamentally different
from likes and reposts.  It's much more plausible, I would argue, for
someone to want a feed of bookmarks then it is for someone to want a feed of
likes - even if, in my case, that someone is myself (hence my separate [link
feed][6]).  As I alluded to before, it's entirely possible to want to
syndicate your likes, but not your bookmarks.  I find myself much more
likely to add a tag to a bookmark post then to a like post.

I suspect what this means for me is that I may end up developing some
personalized and specialized tooling specifically around my bookmarks -
maybe an easy way to manage them on the command line, for example.  Time
will tell.


[1]: https://indieweb.org/bookmark 
[2]: https://indieweb.org/like
[3]: https://indieweb.org/repost
[4]: https://indieweb.org/h-entry
[5]: https://indieweb.org/microformats
[6]: /posts/bookmarks
