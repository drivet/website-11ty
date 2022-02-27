---
blurb: "Taming the #indieweb post type zoo"
date: 2022-02-27T17:51:48-05:00
modified: 2022-02-27T17:51:48-05:00
title: A Place For Everything
mp-syndicate-to:
  - twitter_bridgy
---

If you're a programmer of a certain age, you've either read or know about
[Design Patterns][1], a book on software architecture popular in the 90s and
early 2000's.  The book was sort of like a repository of [TV tropes][2] for
the Object Oriented Programming (OOP) world in that it provided a convenient
vocabulary for describing what were usually eminently recognizable software
engineering techniques that nonetheless often lacked punchy, easy to
remember names.

Notably, the book was more descriptive than prescriptive.  It described
preexisting techniques that were already in use in the "wild", so to speak.

The [IndieWeb][3] community has performed a similar service with online
content.  It used to be that the web was filled with blogs and websites, and
that was that.  Then came the rise of social media, and people suddenly
started posting things like status updates and tweets - frequent, short,
titleless bits of content as opposed to the titled, long-form content that
characterized traditional blogs.  And while you could, as before, comment on
those tweets with your own zingers (forming a class of content unto itself)
you weren't restricted to that alone; now you could also *like* those tweets
and *share* them with other people.

The web, in other words, had moved beyond the venerable blog post.  It
wasn't *just* that more and more online content was being walled off into
less and less services (Facebook, Twitter, Instagram).  It was that *people
were now posting different kinds of things*.

So the IndieWeb community, like the authors of Design Patterns, took a step
back and did their best to identify what was happening in the wild.  It came
up with the following categorization scheme:

* [Articles][4].  Your standard, long-form, titled blog post.
* [Notes][5].  Short, titleless posts, like tweets or status updates.
* [Photos][6].  A post where the content is a photo.  To be distinguished
  from an article that happens to contain a photo in that, with a photo
  post, the photo *is* the content.
* [Videos][7].  Take a wild guess.
* [Replies][8].  A comment on someone's blog post or tweet, but now promoted
  to an identifiable piece of content in its own right.
* [Likes][9].  The notion of a "like" on some content, but recasted as a
  standalone independent post belonging to the person doing the liking.
* [Repost][10].  Very similar to a like, but conveying the notion of
  "sharing" someone's content on your personal "post space".
* [Bookmarks][11].  Very similar to a like or repost, but indicates more of
  a "read later" semantics.

There are others.  I'm not being exhaustive.

Someone who maintains a personal website is not, obviously, under any
obligation to post all these different things.  People still maintain
traditional blogs, after all.  But it's hard to deny the convenience of a
note, for example, when you just want to say something short and sweet
without the hassle of having to write a whole *essay*.

## Organizing Is Hard

For people who *do* publish all these different things, like me, the
immediate problem becomes how to organize it all.  Organizing, in this
context, means grouping your content into feeds for easy and convenient
consumption, usually via [Atom][12], [RSS][13] or [microformats][14].

One obvious approach is to avoid organizing it at all, and just dump all
your posts into one feed.  Subscribers would get everything - every like,
every note, every photo, every article.  To be fair, there is actually
something to be said for this approach and I'd like to think that there are
at least *some* people who would be interested in everything I post online.

So, providing a fire hose of all my content *as an option* is perhaps a good
idea.  But it would be a bad idea, I think to *stop* there, as I suspect
that not everyone is interested in knowing all the gritty details of which
posts I've liked and reposted.  So where to go from there?

Moving in the opposite direction, some people on the IndieWeb ([Neil
Mather][15] for example) provide a distinct feed for every single type of
content they support.  You can browse a list of Neil's likes, for example,
or a list of his photos or replies or article, all separately.

I'll admit that this has a neat and tidy feel to it.  That being said,
certain things about the approach bother me somewhat, though it took me a
while to suss out what it was.

Do I really need separate feeds for my likes and reposts, for example?  From
a technical standpoint these two types of posts are virtually identical.
Would someone who is interested in my likes really not also be interested in
my reposts?

Or how about photos?  Should photos really be its own thing, distinct from,
say, my notes?  Would people interested in my notes really *not* be
interested in my photos?  It becomes even more silly when you realize that
photo posts are really just media-enriched notes.  Why should these go into
a separate feed?

Should articles be separate from notes?  This one is a bit trickier.  I
*can* imagine some people being interested in my articles but not my notes,
and vice-versa.  At the end of day, though, notes are really just short blog
entries without the titles...so maybe not?

Something else to consider is the fact that I post many, many more notes
than articles, so someone who just listens for new articles won't get nearly
as many updates as someone who listen for new notes.  Then again, maybe this
is a feature...

How about replies?  Replies sit somewhere between a repost and a note, so
where do those fit?

As you can see, the subject, for me at least, is not entirely
straightforward.  So what to do?

## First Attempt

My first attempt to organize my content was pretty simplistic:

* I had a "fire hose" feed with everything in it.
* I had an articles feed to house my traditional blog.
* I had a "leftover" feed to house my notes, likes, reposts, bookmarks, replies, etc.

This worked but...wasn't great.  In particular, my leftover feed was clogged
with likes, reposts and bookmarks which, on their own, were a lot less
interesting than my notes and photos.  When I redesigned my site, I took the
opportunity to reorganize my posts.

## And Everything In Its Place

The approach I finally settled on divides my content into two categories:

* Original content.  These are posts that make sense on their own.  This includes
  article, notes, and photos.
* Derivative content.  These are posts that need external context to make
  sense.  This includes likes, reposts, bookmarks and replies.

The derivative content is, in my opinion, much less interesting than the
original content.  I have a hard time imagining someone wanting to browse my
likes or reposts for example, *separately* from my notes or articles.  In
other words, if you want to browse through my likes, I suspect you're
probably okay browsing through my notes as well.

This main content divide is tempered with the following observations:

* Bookmarks are special in that they are a bit more private and dynamic than
  other types of posts.  I'll have more to say about this later.
* I have a soft spot for old school blogs, and the idea of completely
  eliminating the identity of my original blog in favour of a hybrid
  blog/notes/photos feed breaks my heart a little.  Articles, as a post
  type, are a bit special to me.

With that in mind, I came up with the following scheme:

* *Posts*. This is my original content, and includes my notes, photos, and
  articles.
* *Blog*.  This is a separate feed of just my articles, so that my original
  blog can live on.  I *could* make an argument that some people may just
  want to read my essays, but really this is all about nostalgia, pure and
  simple.
* *Links*.  These are my bookmarks.  This feed is mostly for me, since I tend
  to use bookmarks as a "read later" pile.
* *Everything*.  This is a feed of every single post.  It's currently the only
  feed where you can find my likes, reposts, and replies, but they're mixed
  in with everything else, and I'm making a bet that this is acceptable.
  
And there you have it.  I think I'm more comfortable with this
organizational scheme, but time will tell I suppose.


[1]: https://en.wikipedia.org/wiki/Design_Patterns
[2]: https://tvtropes.org/
[3]: https://indieweb.org/
[4]: https://indieweb.org/article
[5]: https://indieweb.org/note
[6]: https://indieweb.org/photo
[7]: https://indieweb.org/video
[8]: https://indieweb.org/reply
[9]: https://indieweb.org/like
[10]: https://indieweb.org/repost
[11]: https://indieweb.org/bookmark
[12]: https://en.wikipedia.org/wiki/Atom_(Web_standard)
[13]: https://en.wikipedia.org/wiki/RSS
[14]: https://indieweb.org/microformats
[15]: https://doubleloop.net/
