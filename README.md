# website-11ty

My personal website and blog, implemented with
[Eleventy](https://www.11ty.dev/).

## Posting

Posting is done by adding a markdown file with front-matter to the
src/posts/feed directory, ideally under the right year (though everything
will still work if you don't).  A date is the only mandatory field, and it
looks like this:

> date: 2025-01-19T19:35:39-05:00

Different kinds of [posts](https://indieweb.org/posts) are supported:

 * **Article**: Front matter must include a title.  Cannot be a like, photo,
   reply, etc.  This is basically the default titled post type.
 * **Note**: Title must *not* be present; that's what makes this a note.  It
   also can't be a photo, like, reply, etc.  Basically the default untitled
   post type.
 * **Photo**: A post with a photo field in the front matter.  Text content
   optional.
 * **Like** and **Repost**: A post with a `like-of` or `repost-of` field
   in the front matter.  Text content optional.
 * **Reply**: A note that's in preply to something, with a `in-reply-to`
   field in the front matter.

## Sending Webmentions

Webmentions are a way of informing someone on the Web that a post you wrote
mentions them in some way.  They're kind of like a modern(ish) day pingback
system, if you're old enough to remember those.

This blog supports the sending webmentions.  I maintain a record of all
webmentions I've ever sent out in a giant yaml file
(src/_data/wmresults.yaml).  Each time my site is deployed, I load the main
RSS feed and pick out the permalinks that do not appear in the wmresults
file.  I assume these permalinks are "new" and scrape them for URLs to ping,
using a [little utility](https://www.npmjs.com/package/send-all-webmentions
I wrote.  I save the results back into the wmresults file.

## Syndication

Syndication services are provided via [bridgy](https://brid.gy/).  You
syndicate a post to your social media account by sending a webmention to the
bridgy service.  I'm not totally sure how the post is authorized, but in
the case of mastodon I assume it has something to do with the fact that my
website is verified on my [profile](https://indieweb.social/@desmondrivet).

Bridgy extends the webmention protocol by adding a Location header to the
result, which I save in the wmresults file.  This location is the URL of the
externally created post.

### Syndication Links

A post that has successfully syndicated to a social media site will show a
link to that external post, with an icon that represent the social media
service.  Since I use bridgy and webmentions to do this, this process is
tied to the wmresults file used in the webmention system.

Basically, I'll look up the successfully sent webmentions associated with a
post, and if any of them look like a bridgy webmention endpoint, I'll check
which external service was used in the webmention, as well as the Location
header, and create a link to the external post, with an appropriate icon.

## Receiving Webmentions

I basically do [what Max Bock does](https://mxb.dev/blog/using-webmentions-on-static-sites/).

I use [webmention.io](https://webmention.io/) for this.  I download my
entire webmention list and stash it in the cache directory.  With every
build I merge the remote contents into the local ones and splice the results
onto each relevant post.

## Link Previews (Unfurling)

Without going into too much detail, this blog supports the rendering of link
previews as part of the build process.  Link previews are especially useful
in the case of likes and reposts, where there's often no content in the post
other than the URL which is being referenced.

See `src/_includes/unfurl.njk` for some insight into how this is spliced into
post output.  See `configs/preview.js` for insight into the code behind it.

Basically, I implemented a preview filter, which is backed by a cache of
preview metadata (`_cache/scraped/scraped.json`).  The cache is lazily
loaded by the preview filter, updated when a preview needs to be scraped,
and saved back when the build is done.

## Tagging

Tagging is supported, and tag pages are created automatically, which are
themselves paginated.

The process is a bit complicated since Eleventy doesn't support the
pagination of tag pages out of the box.  See `configs/collections.json` for
some insight into how it's done.  I wrote a [blog
entry](https://desmondrivet.com/2022/03/23/eleventy-pagination.html) on
this.

## Archives

Archives are supported, and are implemented as a sort of special case of
tagging.  I "tag" each post with a "year" and "year/month" label, which I
then use to render the archive pages.
