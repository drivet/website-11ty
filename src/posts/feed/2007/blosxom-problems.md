---
date: 2007-07-13T15:56:32.000Z
modified: 2015-05-16T05:52:59.845Z
tags:
  - blogging
  - blosxom
title: Blosxom Warts
---

At the moment I'm using [blosxom][1] to generate this blog. I'm using
blosxom for the following reasons:

- the HTML is completely under my control.

- there is no database or PHP required.

- the codebase is tiny, which means I have a better chance of fixing
  whatever problems crop up and also of integrating it into my site.

So far, it seems to work okay. My blog looks more or less the way I
want it to. More specifically, it looks like it's a seamless part of
my web site, which is exactly the effect I was after.

That being said, I'm finding that blosxom has a few warts:

- blosxom does not seem to be actively maintained. The original author
  has, from what I can tell, abandoned it, and the plugins are all
  pretty creaky. This is more of an annoyance than a showstopper
  since, as mentioned, the codebase is small, so it's usually not
  difficult to understand what's going on. Also, I am at least
  somewhat familiar with perl, to the point where I can generally fix
  whatever problem is cropping up, so long as the problem is with the
  usage of the program rather than some fundamental, application-level
  bug in the code (i.e. I would probably be able to fix an incorrect
  config item in the code, but I wouldn't be able to readily fix a bug
  in RSS generation without doing serious research into the RSS
  format).

- blosxom's configuration is built into the script. You need to change
  the script to change the configuration. Weird.

- blosxom uses its own (very simplistic) interpolation and templating
  system. I'm sure there are some who would consider this an
  advantage, but I'm not one of them. I'm a fan of simplicity, but
  this is a little *too* simple. For example, you can only interpolate
  scalar variables into the templates. This necessitates certain
  contortions when you want to do anything more complicated (see my
  note below on separation of content and presentation).

- I use the [Template Toolkit (TT)][2] to generate the pages on my site, and
  I like it, but more than that, I make heavy use of the WRAPPER directive
  to centralize the "structure" of my webpages. I'd like to use TT in this
  fashion to template my blog, but blosxom's plugin architecture makes it
  difficult to do this.

- the "seamlessness" between my blog and the rest of my site is pure
  illusion. As mentioned, I use a centralized template to hold the
  "structure" of my web pages. This is at odds with the way blosxom
  does things; it spits out a "head" chunk, followed by several
  "story" chunks, followed by a "footer" chunk. There's no real
  linkage between the different chunks except through the driver
  program. I got around this by writing a special plugin to
  dynamically "break down" my templates into something that blosxom
  could cope with, but this is a hack, at best.

- the line between content and presentation is blurred. I use a
  plugin, for example, to generate the archive tree on my blog. This
  plugin, like some nightmarish CGI program from the mid 90's,
  programatically spits out fully formed HTML markup from perl code, to
  be appended to blosxom's output via template variables. What's the
  point of having an (admittedly simplistic) templating system if you
  just bypass it completely? I know the answer, more or less:
  blosxom's templating system is too simplistic to cope with
  structured data. The approach taken to work around this limitation
  works, certainly, but it's an assault on my sense of aesthetics.

On the one hand, these seem to be fundamental problems with
blosxom. On the other hand, there doesn't seem to be much in the way
of alternatives out there, considering my design goals. And even
though I've vented considerably about blosxom's inadequacies, I *do*
like the clean, lean driver program that it offers. I like the fact
that most of the functionality in blosxom is delivered via plugins, if
not necessarily how those plugins are written. And I do like the fact
that there exists a considerable wealth of plugins for blosxom, even
if most of those are unmaintained.

One project I'm keeping track of is [Bryar][3]. This is a blosxom "inspired"
project and it aims to be a more modernized version of the software. It uses
the Template Toolkit for templating purposes (awesome!) and it corrects what
I see as a major flaw in blosxom's design, namely the incorporation of
configuration items in the blosxom script itself. Unfortunately, I'm not
sure how one goes about writing plugins for this thing. It doesn't seem to
be set up to do it in a very straightforward manner.

For all its warts, there is very little you can't do with blosxom,
given the time and inclination. I'm not yet convinced of that when it
comes to Bryar. I guess we'll find out.

[1]: http://blosxom.sourceforge.net
[2]: http://www.template-toolkit.org/
[3]: http://search.cpan.org/~dcantrell/Bryar/lib/Bryar.pm
