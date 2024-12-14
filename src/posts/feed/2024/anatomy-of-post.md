---
date: 2024-12-07T12:07:47-05:00
title: "Anatomy of a Post"
blurb: "Different posts look different under the hood"
post-status: draft
mp-syndicate-to:
- mastodon
tags:
- eleventy
- blogging
---

My blog is based around Eleventy, which means I write my entries in plain
text, using Markdown and YAML front matter, a combo which I'm sure is
familiar to most users of static site generators.

I also participate in the IndieWeb, which means I post many different kinds
of content other than your standard blog posts - notes, likes, photos, etc.

I find it useful to divide my content in two main types: original content,
where the focus is on my own published material, and derivative content,
which only makes sense in the context of another, external URL.

The main original content posts are:

 * Articles
 * Notes
 * Photos
 * Multi-Photo

I would include albums and album collections here as well, but we'll deal
with those in a bit.

I've written a bit before about how I format my posts, but for the sake of
reference, an article looks like this:

``` markdown
---
date: 2022-03-01T13:57:07-05:00
title: 'Much Ado About (Indieweb) Bookmarks'
blurb: 'The ins and outs of owning your bookmarks on the #indieweb'
tags:
- indieweb
mp-syndicate-to:
- twitter
---

Like many Indieweb participants, I post...
```

And a note looks like:

``` markdown
---
date: 2022-02-24T01:27:35Z
---

I am grateful that Wordle exists if for no other reason...
```

Here's a photo post with multiple photos in it - a multi-photo, in other
words:

``` markdown
---
date: '2023-01-15T21:09:31-05:00'
mp-syndicate-to:
- twitter
photo:
- alt: Book nook contents
  value: https://media.desmondrivet.com/84/b3/2d/b8/e8de337b847a14ebac309f113fe28f1a572692c05795ca41caf435b0.jpg
- alt: Book nook started, partially built
  value: https://media.desmondrivet.com/04/5e/62/95/632d1dff8038533ba4114c0dadce31784f1a2746323e7c1b7af84c86.jpg
- alt: Miniature bookcase
  value: https://media.desmondrivet.com/2b/2a/50/dc/d18b6e980e029b94757a3e9ff500594d7ab218da940ab4f7885c5743.jpg
- alt: Book nook halfway finished
  value: https://media.desmondrivet.com/fb/c4/7c/3e/ff27aafb08dcbb24c3d08dd82ee879acbb642a968acd85f853bbb96f.jpg
- alt: Miniature book
  value: https://media.desmondrivet.com/70/a8/d8/80/25e8a621770fbe49354001c0a70e614afc9a21ecd75bb4b87e0f9c73.jpg
- alt: Miniature book with pages
  value: https://media.desmondrivet.com/26/69/87/7f/ca703fd7f6d2a8c5fa60bf69d5d0dd82b657e97a6e1141d2018c01a4.jpg
- alt: Miniature wall bookcase with many books 
  value: https://media.desmondrivet.com/e5/54/84/1a/9f396871304906351237e67c2cd5dd20b4e543a0398729cdd0a33ef8.jpg
- alt: Completed book nook on shelf with books
  value: https://media.desmondrivet.com/0c/7c/3f/a7/681cbb1930b6b6367f35ebeceeb61e16a1c620bfd73c62af76d75f67.jpg
---

I got a "book nook" for Christmas - kind of like a book end, but more elaborate.  Super fun, I want them all.
```

So how do I render these posts?  One very simple and direct way is create a
separate template for each type, but would result in a significant amount of
repetition.  A photo post and multi-photo post are virtually the same, for
example, and both of them share a lot in common with a note, in that they're
basically just notes with photos added.  An article could be consider a
longer note with a title added.

Deciding how to render them all on my blog in a relatively DRY way i.e. a
way that doesn't require me to sprinkle the same snippets of HTML among a
batch of templates of varying degrees of similarity is actually, in my
opinion, a non-trivial problem, which is worth recording here for posterity.
Most of what I write will be very specific to my workflow, but it's not
entirely inconceivable that someone might gain some inspiration for their
own blog

In any case, my main strategy for dealing with these posts is to treat them
all as variations on the same theme and hence to create a single generic
post template which captures them all.  To start with, it includes:

 * an optional title (mandatory for articles)
 * optional content (mandatory for notes and articles but photos may not have
   any text) and
 * optional photos

If we're dealing with a multi-photo, then one more piece of content is
implied: a line telling you how many photos are in the post, if there's more
than 1 (for multi-photos, in other words).  A good place to put that would
be after the title, but before the content, so you would get something like
this:

 * Title
 * Photo count (for multi-photos)
 * Content
 * Photos

Although these are all optional, there is an implicit understanding that at
least some of these pieces have to be present in some combination, otherwise
you have no post at all, right?  And some parts come as a pair; you would
never have a line that said "3 photos" without showing any photos at the
end.

## Derived Posts

The other form of content I post are "derivative" posts.  The main
derivative post types are:

 * Likes
 * Reposts
 * Replies
 * Bookmarks
 
These are all posts that are "about" another URL, which I call a *referenced
URL*.  The posts make no sense without it, so it needs to be included in the
somehow.  With the exception of a reply, these types are often without
content, though there's nothing stopping you from adding some.  In addition,
these posts should all show an unfurled "link preview" or "reply context"
(the IndieWeb wiki distinguishes between these two things, but I don't find
the distinction useful, so I don't make one) for the referenced URL.

To give you some context, here's a "like" post:

``` markdown
---
date: '2024-11-09T12:53:58-05:00'
like-of: https://adactio.com/journal/21548
---
```

Super simple right?  Here's a reply post:

``` markdown
---
date: '2023-09-15T06:41:24-04:00'
in-reply-to: https://nerdculture.de/@titaniumbiscuit/111067559683462120
mp-syndicate-to:
- mastodon
---

Yeah I may have judged too quickly with regard to Obsidian. Do you use or recommend it? 
```

The difference in format between the derivative posts is quite superficial.
They all have a referenced URL, tagged with their respective identifiers
("like-of" for likes, "in-reply-to" for replies, etc.), and optional
content.

The template for these posts would seem correspondingly obvious.  They would
include an optional referenced URL, and optional content, followed by an
unfurled link preview.

Does it make sense to have a separate template for these kinds of posts?  My
first instinct was yes, since the only overlap with the "original content"
template appeared to be the content.  Upon further reflection, I realized
that notes sometimes have article links in them that would benefit from a
link preview as well.  And, though it might be unusual for a like or a reply
to have a title, there's no particular reason to enforce this as a rule.

The bottom line is that a "derivative" template can pretty much take
advantage of the "original content" template and just add the one thing that
is unique to these kinds of posts: the referenced URL.  A good place to
show it is after the title, and before the content.  With that, we have a
new unified template with:

 * Title
 * Referenced URL
 * Photo count
 * Content
 * Photos
 * Link preview

As before, all of these elements are optional, but at least some of them must
be present at any one time, and not all combinations are really valid (I'm
having a hard time imagining how a link preview would work with a photo
post, for example).

## List Context Versus Full Context

Blogs tend to have two main contexts in which posts are rendered: a *full
post*, shown on its own page, located at a permalink, and something which for
lack of a better name we shall call a *post entry*, shown in contexts where
it's part of a list, such as on a timeline or a feed.

There are some differences in how full posts are rendered versus post
entries:
 
 * Articles are shown truncated in a post entry, with a "Read more" link to
   take you to the full article
 * A multi-photo post would show thumbnails in the post entry, and the
   larger photos in the full post.
   
So, this discussion now implies *two* templates - one for posts in a list
context, aka as post entries, and one for posts in a full context.

A post entry template looks like this:

 * Title
 * Referenced link
 * Photo count
 * Truncated content (with Read more link)
 * if multi-photo show thumbnails, otherwise show one large photo
 * Link preview

For a full post template we have this:

 * Title
 * Referenced link
 * Photo count
 * Content
 * Large photos
 * Link preview
 
## Open Graph Versus Featured Images

Open Graph is a standard created by Meta (Facebook) for annotating URLs with
metadata that can used in the unfurling of said URLs in other contexts.  One
of these bits of metadata is the image associated with the post that is
displayed in such contexts.

Wordpress reportedly has a similar concept called the "featured image",
which has been copied by the IndieWeb community in the form of a u-featured
microformat.  A featured image, like Open Graph's image metadata, can be
displayed in a link preview context.

The two concepts work differently.  The Open Graph image URL is true
metadata; it's configured in a meta tag of a post's HTML.  As such, you
never see the image in the rendered post unless you explicitly put it in
there.  The IndieWeb featured image concept, on the other hand, is designed
to be embedded in the post itself, so that you have something to which you
can add the u-featured microformat.  You can, of course, turn the image off
with a good, old-fashioned "display: none" but those kinds of shenanigans,
while sometimes unavoidable, are generally frowned upon.

The featured image is easy to add to an IndieWeb post if you're dealing with
one that incorporates such an image organically, such as a photo post, or an
article with some images embedded.  In such a case, the featured image can
also be used as the Open Graph image for maximum unfurling capabilities.

I'll usually pick the first photo as the featured image in a multi-photo
post.

On the other hand, if you have an article with no natural image to display,
then the situation becomes more complicated.  Some people will actually
incorporate a featured image into their content and display it in their
posts but that feels somewhat forced and artificial to me, so I don't bother
to do it.  I do certainly add an Open Graph image if I remember to.

## Photo Album Format

I also post photo albums.  Photo albums are similar to multi-photo posts in
that they both house multiple photos, but an album differs from a
multi-photo post in the following ways:

 * Each photo in an album post has its own permalink
 * An album is more curated and less spontaneous than a photo post.  The
   photos are more processed and I'm more selective with them
 * An album tends to have more photos than a photo post, so that showing all
   the thumbnails in a list context isn't always feasible

Under the hood, as far as the post format is concerned, the two are nearly
indistinguishable, except for the following:

 * A multi-photo post tends to have no title, although there's nothing
   really enforcing that.
 * More importantly, an album post has an explicit flag, **album: true**,
   that concretely identifies the post as an album

``` markdown
---
date: '2024-10-05T20:30:30-04:00'
title: Plaza De Espana
featured:
  alt: ''
  value: >-
    https://media.desmondrivet.com/27/82/0a/cd/c834418f3eb97df08ead1fc9ee4839e43218bef19c42c728aab27fde.jpg
album: true
excludeFromPosts: true
parent: '2024-spain'
photo:
  - alt: ''
    value: >-
      https://media.desmondrivet.com/3a/41/39/00/6a65b1d3b2cea7a36d7c4b5e97deddbaea74bb3004ce61c46b93cb97.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/66/f0/05/c5/db2ed1d9fb53d937883170044fd37ce4a49e3b8ac6f6d0cddf4b3205.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/a8/49/b6/5e/0e5b9dd0d44f6602045dbc2b29a478c8abfed3b6ec26799f6ce8871f.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/9b/ce/73/e9/f001794447336d239fa507a1a57db357fef181d1dcc79375b8e50980.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/0c/57/30/34/902a01992fb105db4009d923ec25c582dcf13a63bdc8776e736d0ff6.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/0b/6a/12/4b/283a2e598bf020e67474cee6baa83826f2b640472a252d70eb514830.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/f2/c2/49/48/19b7a46adf598e35d82b35ca8572dcb31c81397913422bda35ef3ac3.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/0d/9f/fd/c8/68dc185b232fa7a466a2eb440b3b2d733ae6bb911b7e34349541bcef.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/d0/65/70/97/fac5397a015dc4a1f37b78b716df7c5b0cd75cfadbac0eab619836e5.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/f3/0e/15/77/f312e468e2776b055f04bd795bf0989e8dc0f505a4e44439ec08307d.jpg
  - alt: ''
    value: >-
      https://media.desmondrivet.com/27/82/0a/cd/c834418f3eb97df08ead1fc9ee4839e43218bef19c42c728aab27fde.jpg
---

Plaza de Espana is one of Seville's main postcard shots.  A huge, colourful building, it's hard to get it all in one photo.

```

Some things to note:

 * As mentioned, I have an explicit album flag defined.
 * I've explicitly defined a featured photo here, because the first one
   won't do.  I should probably move the featured flag into the photo list,
   but I was lazy.
 * I have a flag called "excludeFromPosts" which will remove this album from
   direct feeds.  I only want the album to appear as the child of my "Spain"
   album collection (see below), defined by the parent key.

### Rendering Photo Albums

I could, in theory, choose to render the photo album in a similar way to a
multi-photo post, but I've opted instead to do it a bit differently:

 * Because a photo album is meant to be more "curated", and may have many
   photos in it, I want to show just the featured image when it appears in a
   list context, instead of the thumbnails.
 * Since each photo has its own URL, when showing the full album post, I
   want to show just the thumbnails and let the user navigate to the photo
   URL when they click on it.

This means that my post entry template now looks like this:

 * Title
 * Referenced link
 * Photo count (shown for multi-photos and albums)
 * Truncated content
 * If album, featured image.  If multi-photo, thumbnails.  If one photo, just the photo.
 * Link preview
 
For full posts we have:

 * Title
 * Referenced link
 * Photo count (shown for multi-photos or albums)
 * Content
 * If album, thumbnails.  If not, a stack of full photos.
 * Link preview

One idiosyncrasy of an album post, when compared with a multi-photo post, is
that while we have conceptually defined a featured photo for the album, we
won't show it in the rendered post.  The reason is that all we're showing on
the post is thumbnails, and none of these are suitable for a featured image.
This is one major downside to using microformats to define a featured
image - you need to have the actual image in the post so that the
microformat has something to grab on to.

You *can* of course continue set the Open Graph image metadata to whatever
you want, and we do, in fact, set it here to the featured image so that the
permalink will unfurl correctly in most situations.

## Album Collections with Explicit Featured Images
 
A photo album collection, as you might guess, is a collection of photo
albums, i.e. a post that houses other photo albums.  It can also house other
album collections.  Such a post houses no photos *directly*, but it is still
is photo oriented, so it makes sense, in my opinion, to explicitly define a
conceptual featured image here, even if it's not "organically" part of the
post.

Format-wise, an album collection looks a lot like a really, really short
blog entry - it has a title, and a line or two of content, describing the
collection.  But there are some things which set it apart:

 * The metadata will have an eleventyNavigation key, which child photo
   albums point to as their "parent".  In theory this feature doesn't have
   to be restricted to photo albums, but that's all I'm using it for at the
   moment.
 * Crucially, the metadata will have an **album: true** flag, which firmly
   identifies this as an album collection - and notably *not* an album,
   since there are *also* no photos defined in the post directly.

An album collection will have an explicit featured image defined in the top
level metadata.

### Rendering Album Collections

When rendering an album collection in a list context, it makes sense to me
to show its featured image, like a proper photo album.  In addition, I want
to display the number of albums in the collection.  This means I enhance my
post entry template so that it looks like this:
               
 * Title
 * Referenced link
 * Photo count (for multi-photos or albums)
 * Album count
 * Truncated content
 * If one photo, just the photo.  If multi-photo, thumbnails.  If album or
   album collection, featured image.
 * Link preview

The full post for album collections will render, in some fashion, the list
of albums for that collection.  Our template is therefore augmented to
include albums, like this:

 * Title
 * Referenced link
 * Photo count (for multi-photos or albums)
 * Album count
 * Content
 * If album, thumbnails.  If not, a stack of full photos.
 * Children of the album collection, either albums or more album
   collections.  Either way, each child should have a featured image to
   display.
 * Link preview
 
Like a regular photo album post, we don't render a featured photo for full
album collection posts, since the photo is not really shown organically in
the post.  And again, like the photo album post, we *will* continue to
define the Open Graph image from the *defined* featured photo in the raw
source post, so the URL can continue to unfurl correctly.
