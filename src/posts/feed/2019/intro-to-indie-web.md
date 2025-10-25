---
blurb: 'In which I discover that I''m retro, not passé #indieweb'
date: '2019-12-08T17:46:15.000Z'
modified: '2019-12-08T17:46:15.000Z'
tags:
  - indieweb
title: In the Beginning Was the Website
---

I don't think I've ever felt quite as old as I felt when, last year, I
discovered the [IndieWeb][5], an online community of people dedicated to
resurrecting the personal website.

This makes me feel old because I've maintained some sort of personal web
presence/site/blog since around 1998 or so, when I made my first hand-coded
HTML pages available online at U of T.  Apparently, enough time has past not
*only* for the concept of a "personal website" to have become quaint and
old-fashioned (displaced by a cluster of much more convenient social media
sites) but *also* for it to have been picked up again by an enthusiastic
band of hobbyists with a taste for the retro and a fondness for old-school
fan pages.

You hear that?  I've moved beyond passé into *retro*.  It's as if I never
stopped wearing 80's clothes but now they're cool again so it's okay.

## Why Not the Status Quo?

To be honest, until I stumbled upon the IndieWeb, it had never even occurred
to me that the personal website was something that needed reviving but, in
retrospect, it does seem obvious that I'm a bit of an outlier.  Personal
blogs (in the sense of someone running their own blogging software on their
own domain) are not something you see much nowadays; people with something
pithy to say will use Twitter or Facebook and those with longer attention
spans will use something like Medium.  You don't have to install anything
and you automatically reach a larger audience than you probably could have
on your own.

It sounds like a win all around, so why would anyone want something
different?  Why does the IndieWeb exist at all?  Aside from the obvious
pleasures of DIY and of getting your hands dirty, the answer could probably
be summarized in one word: control.

When you post a tweet or a status update (or any form of content), it ceases
to be yours the second you hit the enter key.  What happens to your content
if Twitter or Facebook shut down - or, if that seems too unlikely to
contemplate, what happens if you simply want to *leave*?  What happens if
Facebook crosses one too many ethical lines and you decide that you're
better off without it?  What happens if Medium stops being a clean, spare
blogging site and instead turns into an [annoying piece of nagware][11],
constantly pestering your readers to create an account?

To be fair, Twitter *does* offer a feature allowing you to download your
archive.  I can testify that it works; you make a request and a few minutes
later you can download a zip file with your actual content, in a text format
known as [JSON][10].

But...then what?  What is one supposed to do with that?  I mean, I'm a
software developer by trade, so I happen to know what a JSON document looks
like and how one goes about extracting information from it but can you say
that about the average person?

And, not to be paranoid, but what happens if they decide to remove the
feature?  What exactly could you do about it - except *leave*?

There are, of course other reasons to avoid these sites.  No one likes to
see ads.  Facebook is constantly tracking your online activities and selling
what it collects to nefarious organizations.  Twitter contributes to the
[echo chamber][12] effect.  But honestly? These all take a back seat to the
issue of control.  Even if there existed a completely benign, free, ad-less,
non-tracking social network, the problems I described would still exist.

## Failed Attempts

There have been attempts over the years to push sites like Facebook off
their perch.  They all failed for various reasons.

One of the first and most notable ones was [Diaspora][6].  It was touted as
a kind of decentralized Facebook that users could join by hosting their own
"pod" and running specialized software, which would communicate with other
pods in the Diaspora network to provide things like status updates.  There
was also a hosted version that you could sign up for, but that kind of
defeated the purpose of the whole endeavour.

It was an interesting experiment, but it failed to take the world by storm.
Many people point to the fact that the initial software offering was riddled
with bugs, and that setting up your own pod was a daunting task, but
ultimately I think the reason was simpler: Disapora didn't have very many
users.  Imagine going to all the trouble to launch a Disapora pod only to
discover that you had joined a ghost town?

More subtly, the issue is also one of interoperability.  The IndieWeb
community's blanket term for sites like Facebook and Twitter is *[silo][13]*
and the metaphor is apt.  People take it for granted that Facebook users
don't get to follow you on Twitter unless they're also Twitter users.
Sending a message to someone on Facebook doesn't work unless that person is
also a Facebook user.  You don't get to comment on Medium articles unless
you create yourself a Medium account.

Despite being decentralized and free, Diaspora is also a kind of silo.
Generally speaking, when you post something to Diaspora, the only people who
can see it are other Diaspora users.  Your posts do not show up, for
example, on someone's Twitter feed.

Can you imagine if something as basic as email worked like that?  Can you
imagine if Gmail users were restricted to sending emails to other Gmail
users?  The fact that they are not so restricted is directly attributed to
the fact that the protocols involved in sending emails are open standards.
Even if you opt for a Gmail account, with your emails ultimately under
Google's control, the idea that you couldn't use that account to send emails
to someone who uses, say, [ProtonMail][7], is absurd.

So...why can't social networking work like that?  Good question.

## Enter the IndieWeb

The IndieWeb is an attempt to bring the benefits of social networking (such
as sharing articles, status updates, likes and reposts) to the web at large,
using open protocols.  Whereas a site like Twitter or Facebook is a kind of
walled garden, with online social interactions restricted to the confines of
the site, the IndieWeb tries to create a social network *out of the world
wide web itself*.

The Diaspora project, of course, tried to do more or less the same thing,
with its vision of a social network consisting of a decentralized collection
of web "pods" communicating among themselves via the Diaspora software
suite.  As noted, it failed because, at the end of the day, it tried to be
an out-and-out replacement for Facebook, which was an unrealistic goal;
Facebook has billions of users, and they aren't budging anytime soon.

The IndieWeb is a somewhat different beast.  You don't need to run a
specialized "pod" to become part of the IndieWeb.  You just need a standard,
vanilla website - albeit one that conforms to certain (straightforward)
patterns.  After that, sharing a status update (for example) consists of
posting content to your own site, where anyone can read it, using whatever
mechanism is most convenient for you.

This is an important point to grasp, so I'll reiterate.  A personal website
belonging to the IndieWeb doesn't need to run any particular suite of
software, and doesn't need to be hosted on any particular service.  It's a
standard website, written with standard HTML and CSS, using whatever tools
you want.

One of the key differences between the IndieWeb and similar projects like
Disapora is that the former prescribes a methodology or pattern for
[syndicating your content to sites like Twitter][8] and for [harvesting
online interactions to those syndicated copies][9] for use on your own site.
In other words, the IndieWeb is not trying to dislodge the existing social
networking giants, but is rather trying to cooperate with them.  This is a
crucial feature, and is the main reason why adoption numbers don't really
matter as much to the IndieWeb as they did to other attempts at open social
networking.  Even if the entire IndieWeb consisted of a single personal
website, it would still "work", in the sense that the owner of the website
would be in control of their own content and would still be able to share
that content with their friends on (say) Twitter.

## Details to Come

If you've read this far, I'm assuming you found the subject interesting.
And yet, there are many unanswered questions.

In concrete terms, what does it take for a website to become part of the
IndieWeb?  What does the IndieWeb even *look like*?  How exactly do you
"like" something or leave comments on another website without signing up for
some sort of centralized service?  To put it simply, how does this all
*work*?

The IndieWeb community has answers for all of this, which I intend to cover
in further blog entries.  If you are interested, I urge you to stay tuned.


[1]: https://en.wikipedia.org/wiki/Randall_Munroe
[2]: https://www.xkcd.com/
[3]: https://imgs.xkcd.com/comics/the_simpsons.png 
[4]: https://imgs.xkcd.com/comics/movie_ages.png 
[5]: https://indieweb.org/
[6]: https://diasporafoundation.org/
[7]: https://protonmail.com/
[8]: https://indieweb.org/POSSE
[9]: https://indieweb.org/backfeed
[10]: https://www.json.org/json-en.html
[11]: https://indieweb.org/Medium#Nagware
[12]: https://en.wikipedia.org/wiki/Echo_chamber_%28media%29
[13]: https://indieweb.org/silo
