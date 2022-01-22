---
date: 2019-01-12 12:41:53-05:00
modified: 2019-01-12 12:41:53-05:00
status: draft
title: Taming Pelican Settings
---

I use Pelican to generate my website and blog.  Out of the box, it's set up
a simple blog at the root of your website, but in can do more than that with
a bit of elbow grease.

My website, for example, consists of what I suspect is an unusual amount of
pages, and my blog is hanging off of a specialized sub-category.

Pelican has a lot of settings, of not all of them appear to be used by the
core engine; many of them are there for use in templates, which the
templates themselves may ignore.  I'll concentrate on core settings in this
article, which direct the structure of your website.

## Content Structure

PATH

This one defined the root of all your content.  All other content related
path configuration will be relative to this one.  Defaults to the root of
your repository; I have mine tucked under a "content" folder instead.
Pelican defines three main types of content:

* articles
* pages
* drafts
* static files

Articles are what pelican calls blog entries, and will be collected onto
various other specialized pages, like tag, archive and category pages.
Pages are standalone pages of content that won't be collected anywhere.
Static files just get copied over to the output directory without being
modified.

Article input content is defined by

ARTICLE_PATHS
ARTICLE_EXCLUDES

Article output content is defined by:

ARTICLE_URL
ARTICLE_SAVE_AS

These two values need to be the same unless you compensate in your web
server's configuration with a rewrite rule of some sort.

You can use meta data in these settings.  For example, my ARTICLE_SAVE_AS is
defined as

ARTICLE_SAVE_AS = 'blog/{category}/{slug}.html'

so the category of the article ends up in the path.  Since I *also* have the
following setting

USE_FOLDER_AS_CATEGORY = True

this means that the path of the article is preserved in the output folder
upon compilation.

There are equivalent settings for standalone pages, drafts and static
content.

## Direct Templates

Pelican defines a "DIRECT_TEMPLATES" setting, like this:

DIRECT_TEMPLATES = ('index', 'tags', 'categories')

Each item in the tuple causes a file to be generated in the output based on
a template names after the item.  Each item is also associated with a
corresponding pair of settings like:

<template>_SAVE_AS
<template>_URL

Pelican generates a few collection type pages out of the box:

* archives
* tags
* categories


