---
date: 2013-10-30T05:50:12.000Z
modified: 2015-05-16T05:53:02.520Z
tags:
  - emacs
title: Being an Emacs User in the Cloud Age
---

I'm a long time (15 years) [Emacs][1] user. Please don't draw too many
conclusions from this fact. I'm not, by any stretch of the imagination, an
Emacs "wizard". I don't know how to make my Emacs buffer do amazing things
with a single key stroke. I'm embarrassed to say, for example, that I've
only recently learned the keystroke for deleting am entire line.

I'm not a Lisp hacker, and even less of an Emacs Lisp hacker.  While I
wouldn't say my knowledge of Lisp is non-existent, I would still firmly
categorize it as "novice-level".

The question of why my knowledge of Emacs is somewhat primitive considering
how long I've been exposed to it is a topic for a separate conversation but,
briefly, I think it's a combination of my tendency to "plateau" quite early
when learning a new skill, and my high tolerance for pain.  When I figure
out how to do something, I tend to stick with it, even if it's not a very
optimal way of doing it. So, for example, deleting a line in a text buffer
is doable by pressing the backspace key several times, or by highlighting the
line in question with the mouse and deleting it. I never really tried to
find a faster way to do it (though now that I'm aware of it..cool!)

All that being that being said, I have developed, over the years, a fair bit
of "workflow" around my use of Emacs.

Emacs isn't a so much a text editor as it is a text editing platform - a
platform for controlling and transforming text buffers. So over the years
people have written packages which, at first blush, don't seem like the
sorts of things one would develop on top of a text *editor* but which, when
you view Emacs as a text editing *platform*, make much more sense.  So we
have [email clients][2] and [task managers][3] and [contacts databases][4]
because they all involve, however tangentially, some sort of text editing
and/or processing at some point during their use (email writing perhaps
being the more obvious example).

A while back, I "drank the Kool-Aid", so to speak, with respect to these
packages and adopted a number of them for my every day workflow.  So I
started using Gnus as my email client, and Org-Mode for my task manager, and
BBDB to maintain my contacts.  I ended up writing some Emacs Lisp to make
everything work acceptably, and I even ended up developing a somewhat
elaborate scheme for doing [GTD][5] with org-mode.  It was fun, and I
eventually got stuff to (mostly) work but it was an uphill battle. I was
seduced by the promise of a completely integrated workflow. Imagine being
able to read an email and to be able to create a TODO item based directly on
that email. I realize office software exists to do this already but the
glorious thing about Emacs is that it's a) free and b) utterly, completely,
insanely customizable - because you're the one gluing together all the bits
of functionality that you need into an integrated whole.

Of course, I never got around to writing any of it.

## Jumping on the Mobile Bandwagon

I recently bought myself a Google Nexus 4.  It's my first cell phone - not
only my first *smart* phone mind you, but my very first *cell* phone.  It's
opened my eyes to convenience of keeping everything in the cloud.  Owning a
smart phone has really highlighted one of the main weaknesses with using
Emacs for everything - you're tethered to your desktop.

I mean, what's the point of spending hours on end configuring Gnus,
meticulously integrating it with BBDB and org-mode if I just lose all that
customization when I go out?

Oh, various options do exist.  There's a project called [MobileOrg][6], for
example, which makes it possible to edit org files on your phone. I thought
this was a great idea until I started getting down to the nitty gritty of
how it was going to work:

* push org-files to your phone when you're about to leave the house

* capture items on your phone while you're out

* when you get home, pull items from your phone to an "inbox" file, to be
  integrated into your main org file system at your leisure.

The whole process seems a bit...klunky.  Not to mention the fact that I've
customized the process of capturing TODO items on my desktop; I have a
series of [remember][7] templates set up for different kinds of entries. All
that is gone when I use MobileOrg.

And with [Evernote][8], I can just write a note, and it's...everywhere. It's
alluring.

I have a similar dilemma with Gnus. One the one hand, the process of how I
read and write messages with Gnus is fairly customized. On the other hand,
what do I do when I'm out? I could set up an IMAP server, and that would
ensure that my messages are available everywhere, but what about, say, the
elaborate [scoring][9] system that I dream of setting up one day? Can I
carry *that* with me? And what about my newsgroups?

## Where Do We Go From Here?

I often find myself admiring [Richard Stallman][10], who starting writing an
entire operating system  solely on principal - because he  didn't agree with
the closed source licensing arrangements at the time.

I admit I'm less concerned about the closed/open source debate, and more
concerned about the consequences of putting my entire digital life on
someone else's computer, with no real means of getting it back. It scares
me. But it's hard to deny how convenient it is.

Can I ever imagine myself writing whole blog entries in a browser window?
Not a chance. I really like using Emacs. But, that being said, it would just
be *super peachy* if my GTD files - which are really just org-mode files in
disguise - were available from every device I own.  Not just that - I would
love it if my org-mode customizations were portable as well, so that I only
had to set things up once, in my .emacs file, and it would all just work,
web and all.

So here's a proposal. What if someone - perhaps even myself! - were to write
a web application to access their org-mode environment via a browser?  What
if said web application used Emacs as back end (running in [batch mode][11])
so that you wouldn't have to re-invent the wheel in a half-assed manner,
doing something that Emacs already does perfectly well (i.e. manipulate
org-mode files). That would solve the problem, more or less, wouldn't it?
Particularly if this hypothetical web application adhered to the principles
of responsive design. Emacs on the desktop, and this (as yet imaginary) web
application on the go.

What if this web application could manipulate your BBDB contacts as well?
Wouldn't that be cool? I'm surprised no one's done it. A Google search, at
least, turned up nothing of interest.

In all honesty, this could be just the excuse I need to try my hand at a
NodeJS project...

[1]: http://www.gnu.org/software/emacs/
[2]: http://www.gnus.org
[3]: http://orgmode.org/
[4]: http://bbdb.sourceforge.net/
[5]: http://www.davidco.com/about-gtd
[6]: https://play.google.com/store/apps/details?id=com.matburt.mobileorg&hl=en
[7]: http://www.gnu.org/software/emacs/manual/html_mono/remember.html
[8]: http://evernote.com
[9]: http://www.gnu.org/software/emacs/manual/html_node/gnus/Scoring.html
[10]: http://en.wikipedia.org/wiki/Richard_stallman
[11]: http://www.gnu.org/software/emacs/manual/html_node/elisp/Batch-Mode.html
