---
date: 2025-09-10T08:11:14-04:00
title: "Implementing a Reading List in Org Mode"
blurb: "One way to quit Goodreads using Emacs"
post-status: draft
mp-syndicate-to:
- mastodon
tags: 
- emacs
- orgmode
- books
---

I joined Goodreads in 2012, at roughly the same time I joined Twitter, in
what might be called a more innocent age of social networking.  The idea of
a social network for book readers appealed to me at the time.

Fast forward to 2025, and the world looks different.  Twitter/X has turned
into a hellscape run by a super villain, and Goodreads, though not quite a
hellscape, is slow, dated and run by a company with a dubious ethical
status.

I also joined the IndieWeb during this time and have since been trying to
live up to its principles - in particular the part about owning your data.
And, as it turns out, my use of Goodreads as a *social network* never really
took off - I don't think my friend list ever exceeded 20 people.  I used it
much more as an archive of my reading habits, a record of every book I ever
read and every book I ever wanted to read.

All this to say that it made sense for to start thinking about jettisoning
my Goodreads account.  But what could replace it?

## Alternatives to Goodreads

There are several viable alternatives to Goodreads, particularly if, like,
me, you aren't that invested in the social aspect of maintaining a reading
list.  In 2025, the contenders seem to include, but are not necessarily
limited to:

 * StoryGraph
 * LibraryThing
 * OpenLibrary
 * BookWyrm

BookWyrm, in particular, is interesting as it's part of the Fediverse and
uses the ActivityPub protocol and is therefore truly decentralized service.

Here's the thing, though.  While these services may all have the best of
intentions, there's no predicting the future.  Any one of them might go out
of business, or might simply evolve to the point where you want to leave.
No one predicted, in 2012, that Twitter would be bought out by Elon Musk and
turned into his personal mouth piece, yet here we are.

So what can you do?

## Settling on Emacs and Org Mode

Ultimately, I decided that the only way to truly be in control of my reading
data was to, you know, *take control of my reading data* - and if you read
my blog at all, you know that what this translates into in practice is
keeping a reading list in plain text, as I do with the rest of my content.
More specifically, being an Emacs user who had already implemented a vaguely
GTD like system using the Org Mode, it made sense to try and do this in
roughly the same manner.  What is a reading list if not a giant TODO list?

## The Most Basic Org Mode Reading List Imaginable

Fundamentally, a reading list is just a specialized TODO list which means,
at the end of the day, you can simply do maintain a file like this:

``` text
#+TODO: TOREAD READING | DNF READ

* READ The Neverending Story  :fantasy:youngadult:translation:
CLOSED: [2025-03-06]

* TOREAD The Language Instinct: How the Mind Creates Language     :linguistics:
```

Some things to note here:

 * The first line defines the states that apply to this file.  Since this is
   a reading list, we have two *actionable* states: TOREAD, READING and two
   *terminal* or DONE states: READ and DNF (short for Did Not Finish).
 * I've leveraged Org mode's tag system to provide a basic taxonomy for the
   reading list.
 * Org mode automatically puts in a CLOSED line when the state of a headline
   moves from an actionable one to a terminal one.  You can use this to
   record when you finished a book.
 * This is probably simple enough to maintain by hand.

So is that all there is to it?  Are we done?  Well, not quite.

## Adding More Information

The reading I've outlined above is extremely simple - maybe a little *too*
simple.  For one thing, other than the title, it doesn't store any other
information.  I mean, who's the author?  Did you even *like* the book?

Given that this all just plain text it's easy enough to fill in this kind of
information.  Org Mode even has a feature called *properties* that feels
like a natural fit here.  Let's add it to our file:


``` text
#+TODO: TOREAD READING | DNF READ

* READ The Neverending Story  :fantasy:youngadult:translation:
CLOSED: [2025-03-06]
:PROPERTIES:
:Author:   Michael Ende, Ralph Manheim
:Added:    [2015-07-24]
:ISBN:     9780525457589
:Rating:   :star::star::star:
:END:

* TOREAD The Language Instinct: How the Mind Creates Language     :linguistics:
:PROPERTIES:
:Author:   Steven Pinker
:Added:    [2025-01-12]
:ISBN:     9780060958336
:END:

```

That's better, right?  We've added Author, ISBN, Date Added and Rating to
our books - definitely useful pieces of information.

But now we have a problem.  Are we really expected to write this all out by
hand?  Can Emacs help us out here?  Of course it can.

In our case, help is in the form of a emacs package called org-books which
allows us to quickly add book entries from on a URL (from Goodreads or
Amazon) or an ISBN.  The original package didn't use quite the format 
I wanted, so I tweaked it to my liking.

So now my basic workflow is as follows:

 * Look up the book I want to add to my list with a service like OpenLibrary
   or Goodreads.
 * Add the book using the ISBN and the `org-books-add-isbn` function.

