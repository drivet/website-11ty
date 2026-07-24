---
date: 2026-07-24T08:22:57-04:00
slug: static-envy-reply
in-reply-to: https://www.zylstra.org/blog/2026/07/static-website-envy/
tags: 
- indieweb
---

I run an IndieWeb (Micropub, Webmentions) enabled static
[website](https://desmondrivet.com/) and I admit it's not the easiest thing
in the world. Coding knowledge definitely helps.

My static site generator is [Eleventy](https://www.11ty.dev/).  Posts are
published by committing to the main branch.  I wrote a basic Micropub server
which piggy backs on this mechanism to publish (i.e. it "publishes" a post
by committing to the main branch).  Local comments are handled with
[staticman](https://github.com/eduardoboucas/staticman).

I receive webmentions via [webmention.io](https://webmention.io/).  All the
webmentions are pulled down and integrated with the site on every build. 

Sending webmentions is a bit more complicated.  I have an after-build step
to send out all webmentions from all posts that haven't already been sent.

All in all - complicated but fun :-)
