---
blurb: 'In which I zoom in slightly on the #indieweb'
date: 2020-01-05T16:06:35.000Z
modified: 2020-01-05T16:06:35.000Z
tags:
  - indieweb
title: Your Website Is Your Castle
mp-syndicate-to:
  - twitter_bridgy
---

In a [previous blog post][1], I gave a very brief introduction to the
[IndieWeb][2], hopefully giving a sense of what it is and why it matters.
In this post I'll try and zoom in a tiny bit and explain something of the
mechanics of how the IndieWeb actually works and what it means to "like" a
post or "share" a status update.

I'm deliberately trying to avoid too much detail in this post because,
frankly, there's a lot to write, and it's easy to get lost.  So I'm going to
try and give a rough idea of what an IndieWeb enabled website looks like at
a very high level, without going into the weeds.  Further posts will go into
more detail.

## The Bare Minimum: Owning Your Own Identity

They say a person's home is their castle, and this is especially true when
it comes to the IndieWeb - except, of course, on the IndieWeb, a person's
*website* is their castle.  Your website is effectively your home on the web
and needs to be under your control.

So, yes, the first thing you need to join the IndieWeb is, obviously, a
personal website but, more than that, it has to be *hosted on a domain that
you own*.  This means that a renting a space under a domain you *don't* own
(in the style of https://notmydomain.com/~myname, for example) won't do.

That being said, while hosting your own website on your own domain is a
necessary first step, it is, by itself, not sufficient.  In addition, you
need to be able to use that domain to sign into other services using
[IndieAuth][3], a login protocol based on [OAuth 2.0][4].

In general, this involves embedding your home page with various bits of
information which can be used by third parties to figure out who you are.
The process is particularly simple if you [already have a profile on an
existing social network][5] and you're comfortable leveraging that profile
for authentication purposes.  Alternatively, you can [host your own
authorization endpoint][6], though that is, obviously, more work.

Hosting a website on a domain that you own, and being able to use that
domain for sign-in purposes, is the bare minimum required for joining the
IndieWeb.  Ideally, of course, one would *also* like to publish interesting
content on said website; read on for information on what that looks like in
an IndieWeb context.

## Publishing Content

At the heart of the IndieWeb is an attempt to unify the ideas behind
personal websites, blogs and social networks, but in a manner consistent
with how the world wide web operates.

Central to this vision is the concept of a [post][7].  A post, roughly
speaking, is simply a piece of chronologically ordered content appearing on
your website, identifiable by a unique URL called a [permalink][8].  It's
hard to underestimate the conceptual importance of posts; they are the
fundamental building blocks from which IndieWeb conversations are built.
They are the main apparatus by which one recreates, in a decentralized
manner, the kinds of online interactions one has come to expect from private
social networks.

It should go without saying that publishing a post always happens on your
own website.  Your website acts much like your wall on Facebook or your
timeline on Twitter - it's your personal soapbox, your castle on the web.

On the IndieWeb, almost all forms of content are considered posts, even
things that one would normally not think of as such.  In particular, we
have, but are not limited to:

* [*Articles*][9].  These are the equivalent of classic blog entries -
  titled, long-form pieces of writing.
* [*Notes*][10].  These correspond to what other services might call tweets or
  status updates - short, titleless bits of content.
* [*Reposts*][11]. These correspond to what other services might call
  "retweets" or "shares" - posts which re-publish other posts verbatim.
* [*Replies*][12]. These are bits of content written in reply to a post on
  another website.
* [*Likes*][13].  These are posts which represent the act of liking another
  post.

Articles and notes are probably the most familiar of these, and are probably
what most people think of when they think of a "post".  They are pieces of
writing which make sense "on their own", so to speak; they are not, strictly
speaking, "about" anything other than themselves.  Reposts, replies and
likes, on the other hand, are collectively "about" other posts and they only
make sense in the context of those other posts.

The concept of a repost will be familiar to any user of Twitter (where the
feature is called a *retweet*) and its casting as a new post on *your own*
site (despite the fact that its content is almost entirely derived from
another post on another site) probably makes sense in that context.

Reply posts, in an IndieWeb context, might require some conceptual
adjustment if you're used to posting comments on standalone blogs. In the
latter case, your reply ends up being trapped on someone's site or service,
an outcome which is anathema to the notion of owning your own data.  To
avoid this, an IndieWeb reply, like all posts, is always published on *your
own* site, even though it refers to a post on some other site.

In the same vein, and perhaps requiring even more conceptual adjustment, an
IndieWeb "like" is *also* considered a kind of post.  It's a bit weird to
formulate it this way, since people tend to think of a "like" as an *action*
applied to a post on another site, rather than content living on your own.
The IndieWeb community begs to differ on this point and treats your like as
if it were just another one of your posts, albeit one who's content makes no
sense without pointing to another post (in a similar manner to a repost or a
reply).  In that sense, likes, reposts, and replies are all almost exactly
the same in terms how they're structured on an IndieWeb enabled website
(i.e. they are all posts which derive most of their content by referring to
an external post) and differ merely in the meaning they are intended to
convey.

An obvious question arises at this point: after you've liked (or reposted,
or replied to) a post, how do you communicate that fact to the website
owner?  It seems like the sort of thing they'd want to know; many websites,
for example, display a count of likes and reposts under the post itself.
The answer to this issue involves the twin concepts of [webmentions][14] and
[microformats][15], but I will save the details for another post.

## On Not Losing Your Friends

It's all well and good to talk about owning your own content and creating a
social network out of the web itself, but the fact remains that most people
are not on the IndieWeb.  How do you deal with people who are using sites
like Twitter or Facebook?  Are they forever deprived of your cutting and
insightful posts?

The IndieWeb's answer to this dilemma is not to prescribe any particular
technology but rather to coin an acronym: [POSSE][16], or Publish (on your)
Own Site, Syndicate Elsewhere.  The idea is to publish a post on your own
site *first* and then, only after it's available there, copying a version of
the post to whatever other social networks you wish.  It's more of an idea
than a specific methodology, but there you have it.

There are lots of ways to do this, and it's strongly dependent on what
software you use to publish your website.  Lots of people do it manually;
they will just copy the content to wherever it needs to go.  Some blogging
engines, like Wordpress, support automatic syndication to the social
networks of your choice.  I myself use a service called [Bridgy][17] to
syndicate my posts to Twitter.

Once your post has been published to a social network, the question then
arises of how to get notified when people interact with your *syndicated*
post.  In other words, what happens when someone likes your post *on
Twitter*?  The process of harvesting these online interactions on private
social networks for use on your own site is called [*backfeed*][18] by the
IndieWeb community and your options for doing this vary wildly depending on
the APIs that are available for the sites you use.  Most solutions I've seen
involve some variation of polling your syndicated content for changes.  In
my case, I just use Bridgy (again) since it supports backfeed as well as
syndication.

## Filling in the Details

As I said before, I deliberately left a lot of details out of this
discussion, in an attempt to keep the forest in sight and not be distracted
by the trees.  For example:

* Exactly what information do you need to embed in your home page to be able
  to use your domain for sign-in purposes?
* What exactly does a post look like?
* How do webmentions work?
* How do you integrate IndieWeb features into a [Pelican][19] based blog?

Alright, I'll admit that I'm probably one of only a small group of people
who finds that last one interesting :-)

In any case, I'm planning on answering these questions with further posts.
Stay tuned if you want more information.


[1]: /2019/12/08/intro-to-indie-web
[2]: https://indieweb.org
[3]: https://indieweb.org/IndieAuth
[4]: https://www.digitalocean.com/community/tutorials/an-introduction-to-oauth-2
[5]: https://indieweb.org/How_to_set_up_web_sign-in_on_your_own_domain
[6]: https://indieweb.org/selfauth
[7]: https://indieweb.org/posts
[8]: https://indieweb.org/permalink
[9]: https://indieweb.org/article
[10]: https://indieweb.org/note
[11]: https://indieweb.org/repost
[12]: https://indieweb.org/reply
[13]: https://indieweb.org/like
[14]: https://indieweb.org/webmention
[15]: https://indieweb.org/microformats
[16]: https://indieweb.org/POSSE
[17]: https://indieweb.org/Bridgy
[18]: https://indieweb.org/backfeed
[19]: https://blog.getpelican.com/
