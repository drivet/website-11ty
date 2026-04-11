---
date: 2026-03-28T09:04:40-04:00
title: "The Soapbox and the Display Case"
blurb: "My photo albums are an awkward fit for both the IndieWeb and Eleventy"
mp-syndicate-to:
- mastodon
tags: 
- indieweb
- photography
---

As a participant in the [IndieWeb][1], I post the occasional photo on my
blog.  These photos tend to be unedited and off-the-cuff, often taken from
my phone and posted with Quill via my [Micropub][2] server.

In addition to these spur-of-the-moment photos, I also take a fair number of
more deliberate shots, often when I'm visiting someplace new.  These will
usually be taken with my standalone camera as opposed to my phone, and it's
not uncommon for me to try and clean them up a bit with an image editor
before publication.  These photos are almost always organized into albums.

I've made a significant number of these albums over the years, and they
house a significant number of photos - over 1000 at some point.  This number
obviously goes up as I add more albums, but it can also go down as I'll
sometimes remove photos from albums if I feel they're not really pulling
their weight.  The number is closer to 700 photos now - still not small.

If I'm being honest, my relationship to my photos albums feels different
than my relationship to my blog.  My blog is like a chronicle of life, and
my personal soapbox.  It's designed to be updated frequently (even if I
don't update it as frequently as I should).  My photo albums are more like a
display case, aimed at family, friends, and the world, showing off a heavily
curated subset of my life in visual form.  I'm lucky if I update them more
than a couple of times a year.

I think it's this difference that informs how seamlessly - or not - photo
albums are represented on the IndieWeb.

## Photo Albums and the IndieWeb

The IndieWeb revolves around the notion of a [post][3], i.e. a
[permalinked][4], timestamped bit of content that usually ends up on a
reverse chronological [feed][8] somewhere.  Since the IndieWeb supports the
concept of [photo posts][5], in particular [multi-photo][6] posts which
house more than one photo, one might wonder how [photo *albums*][7] differ
in this regard.

The short answer is that a (multi-)photo post is an indivisible unit.  Even
when the post contains multiple photos, those photos don't have a separate
identity on the web; the only permalink is the post itself.  The photos in
an album, on the other hand, have their own identities (i.e. their own
permalinks) and the album post simply collects these individual photos onto
one (also permalinked) page.

I personally find this relationship between an album post and its individual
photos leads to awkwardness when you try to map it onto the IndieWeb
framework.  It makes sense to treat an album as a post - it has a date,
title, permalink, and intuitively feels like it belongs on a feed.  On the
other hand, the album photos themselves are *sort of* posts, in that they
have permalinks, but also sort of not in that we usually want to exclude
them from feeds to avoid clutter.  And you also need a way of expressing the
connection between the photos and the album to which they belong.

The IndieWeb community, to be fair, has ways of supporting these features.
Though it's standard practice, nothing explicitly forces you to include a
post on a feed, for example.  And the IndieWeb does (sort of) have a notion
of a [collection post][9] which exists to house other posts in a kind of
parent/child relationship.

But these are not what I would call "typical" features.  If you use
Micropub, for example, excluding posts from a feed isn't part of the
standard protocol; it requires the [visibility extension][10] to be enabled.
Publishing an album via Micropub also requires the client and server to
support collections, which also isn't standard.  Collections, in general,
seem to be a bit of a half-baked feature of the IndieWeb ecosystem.

The issues become clearer when you try to use [PhotoPostr][11], an
application which actually implements a Micropub-based album posting client.
To my knowledge it's the only app of its kind - and it only works with
Micropub servers that implement the features I described above - a small
number at best.

Here's the thing.  One of the reasons one would use Micropub for posting at
all at all is interoperability - a Micropub server is supposed to work with
an array of Micropub clients, and a Micropub client is supposed to work with
a variety of Micropub servers.  If there's only one Micropub based album
posting client for you to use, and it only works with a small fraction of
Micropub servers, then I'm not sure what Micropub is doing in this equation
at all.  In the end, PhotoPostr feels like a bespoke piece of software,
built by and for the author - which is fair enough, but it does say
something about how photo albums fit into the 

Add to this: I like grouping whole albums into collections as well.  I'll
often create a handful of photo albums after a vacation, for example, and
file them all under one "album group" for the whole trip.  How do we
implement that?  A collection of albums makes sense here but do any Micropub
frameworks support that?  What about tags?  Do the album group tags get
mixed in with the tags you may have defined for your photos?  Do the
individual albums that make up the album group show up on a feed somewhere
or are they hidden like the photos, to avoid clutter?

That's a lot of questions and they are not, in my opinion, so trivial to
answer.  Basically, I find that my album preferences do not map cleanly onto
standard IndieWeb and Micropub concepts.

## Photo Albums and Eleventy

Also problematic is the fact that the Micropub protocol is based around the
idea of publishing a single post.  Publishing a whole album of photos via
Micropub therefore involves publishing each photo one at a time.  This
doesn't work well when you use a static site generator like [Eleventy][12] -
at least not the way I use it.

My blog is currently hosted on Github.  Publishing a new post involves
committing it to the main branch, followed by a rebuild of the site.  My
Micropub server will do this for you when it receives a create POST.

So, each post is published via a single commit which triggers a new build -
which can sometimes take a minute or two.  Publishing a whole album of 30
photos one at a time using this method (as PhotoPostr does) is, as you might
imagine, a pretty awful user experience.

Not to mention, the sheer number of photos can be a problem.  Before I added
albums to my blog, it built in under 10 seconds.  Adding all my photo albums
nearly doubled the build time, even when the images were already
pre-processed and cached.  And if they weren't?  Well then you better get a
cup of coffee because it might take a while.

It got so bad that my VPS was killing the build process because it was
taking too long.  Something had to be done.

## Moving my Albums

There's no clever solution here.  I ended up moving my albums from my blog
to a [dedicated website][13] that I build and deploy from my home computer -
no automated publishing.

It's not ideal, but it's also not so bad.  For one thing, my home computer
is much faster than my VPS so the build doesn't take as long.  And the way I
have historically interacted with my photo albums was always different than
my blog.  I was always a lot more deliberate and manual with my albums, and
I never really took advantage of the Micropub protocol here, for the reasons
above.  And my blog, sans albums, builds much quicker now, for obvious
reasons, so that's a plus.

Generally speaking, I don't like spreading my content across different
sites.  At some point, my website sported not only my blog but also recipes,
bookmarks and photo albums.  But the thing is, these features don't really
overlap, and it adds complexity to an already complicated build process.
I'll admit that it does feel cleaner to split these things out.  I had
already moved my [recipes][14] out to my [garden][15], so my albums we're
bound to be next.

I guess we'll see how this goes!

[1]: https://indieweb.org/
[2]: https://indieweb.org/Micropub
[3]: https://indieweb.org/post
[4]: https://indieweb.org/permalink
[5]: https://indieweb.org/photo
[6]: https://indieweb.org/multi-photo
[7]: https://indieweb.org/photo-album
[8]: https://indieweb.org/feed
[9]: https://indieweb.org/collection
[10]: https://indieweb.org/Micropub-extensions#Visibility
[11]: https://photo.postrchild.com/
[12]: https://www.11ty.dev/
[13]: https://gallery.desmondrivet.com/
[14]: https://garden.desmondrivet.com/20250218202000-recipes.html
[15]: https://garden.desmondrivet.com

