---
date: 2019-03-09 20:04:45-05:00
modified: 2019-03-09 20:04:45-05:00
title: Indieweb and Pelican
---


## Different article types

* Blog entries (indie web calls these articles)

* Notes (like tweets)

* Bookmarks (like tweets carrying an article which you may or may not
  have read)

## Things to consider

* In practice, neither notes nor bookmarks have categories.

* Notes do not have tags in the same way articles have tags.  Notes may have
  hashtags, but these are different.

* Bookmarks may have tags, but it remains an open question as to whether
  these are the same as article tags
  
## Mapping Onto Pelican Concepts

Article type could be top level category.

Sub-categories (via plugin) could represent specific category of article
type.  At the moment I only see a use for blog entries here, but this
shouldn't be enforced.  Notes don't have categories because we don't specify
categories.

Pelican supports tags, but tags cut across categories.  This is not a
problem for notes, because they don't have tags, but may be a problem for
bookmarks, because we may want tags for those, and we may not want to mix
them with the blog entries.

Without a plugin, we need to accept that all post tags will mix with the
rest.  **If we are willing to write a plugin**, we can either:

* write a plugin for special bookmark tags
* write a plugin for per-category tags

Second option seems a bit more general (could be applied to photos, for
example)

Pelican's master index has everything, for all article types.  We will
generate the category (and sub category pages as per normal).  Tag pages,
too.

We may not generate a master archive page, because I can't think of a way
that this would be useful, with all the notes and bookmarks. **If we are
willing to write a plugin**, we can perhaps generate per-category archives
and only generate them for the blog category.

Pelican generates date pages, which cut across all post types.  Will we need
per-category date pages?  It's possible, but maybe not right away.  Another
possibility is that I tabbify the date pages into categories.
