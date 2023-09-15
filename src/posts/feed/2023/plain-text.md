---
date: 2023-09-14T20:11:18-04:00
title: "On the Durability of Plain Text"
blurb: "If you want something to last, use ASCII"
mp-syndicate-to:
- mastodon
tags: 
- blogging
---

My blog is 16 years old.  I've rotated through several blogging engines in
that time ([Bloxsom][1], YAWT 1.0/2.0, [Pelican][2] and finally
[Eleventy][3]) but they all have one thing in common: they all process blog
entries stored as plain text files.

The fact that all my blog entries are stored as plain text files on my
computer is, I believe, one of the main reasons my blog has lasted this
long.  Text files are trivial to back up.  Text files don't really crash and
rarely get corrupted.  Text files are readable and writable on any computer
manufactured since the 1980's - hell, the *1960's* if I let punch cards into
this conversation.  When you contrast this simple durability with, for
example, a [MySQL][4] data store (used by several blogging engines,
including one of the most popular ones, [WordPress][5]), with all of its
attendant version compatibility and data corruption issues, the superiority
of plain text becomes undeniable if you're trying to write something that
*lasts*.

I've [written before][6] about the longevity of paper over e-books.  Real
books obviously don't use software.  They don't have bugs (well...maybe
literal ones).  They don't require updates and they don't have a *file
format*.  You'll be able to read a paper book 100 years from now, something
I'm pretty sure you'd be hard pressed to do with a Kindle.

Plain text isn't paper, but it's as close as you're going to get to it in a
digital format.  The [standard][7] is over 60 years old and odds are good it
will still be around 100 years from now.  You obviously need software to
read it, but it's standard, simple, ubiquitous software, available on every
computer platform imaginable.  The tools to read it and, perhaps more
importantly, manipulate it, are universal.

I'm mentioning the manipulation aspect of text files because that *also*
plays a role in their longevity.  Yes, text files are easier to back up and
harder to lose then abstract rows in a database, but they are *also* very
easy to convert between one kind of text format and another.  My blog
entries are all written in [Markdown][8] but there's nothing stopping me
from converting them to [AsciDoc][9] or [Restructured Text][10] or even
[org-mode][11] if I wanted to double down on my [Emacs][12] use.

Longevity is also the reason I avoid solutions like [Obsidian][13] or [Roam
Research][14] for my notes or my GTD lists.  I don't know how these pieces
of software store your notes behind the scenes, but I do know that if they
suddenly up and disappeared, I'd be in a bind.  At the risk of beating a
dead horse, it's safer, in my mind, to have actual text files that I can
back up and move around.  For what it's worth, if anyone's curious, I've so
far settled on an [org-mode based solution][15] in this regard.

It's really a matter of how long I want my writing to last.  Do I use plain
text for literally everything?  Of course not, but the likelihood of me
using something else is directly proportional to how disposable I consider
my writing to be.  I'll use [Google Keep][16] or [Todoist][17] for things
like grocery lists, for example, but I won't use them for my blog entries,
or my GTD lists or anything meant to last longer than a week.

At the end of the day, I struggle too much with writing to lose it to
ravages of changing fashions.

[1]: https://blosxom.sourceforge.net/
[2]: https://getpelican.com/
[3]: https://www.11ty.dev/
[4]: https://www.mysql.com/
[5]: https://wordpress.com/
[6]: /2023/08/28/world-library
[7]: https://en.wikipedia.org/wiki/ASCII
[8]: https://daringfireball.net/projects/markdown/
[9]: https://asciidoc.org/
[10]: https://docutils.sourceforge.io/rst.html
[11]: https://orgmode.org/
[12]: https://www.gnu.org/software/emacs/
[13]: https://obsidian.md/
[14]: https://roamresearch.com/
[15]: https://www.orgroam.com/
[16]: 1https://keep.google.com/
[17]: https://todoist.com/
