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

If you read my blog at all, you know that I'm a fan of keeping my data in
plain text.  You might also know that I'm an Emacs enthusiast who's cobbled
together a vaguely GTD like system using Org Mode.  Given that a reading
list is, fundamentally, nothing more than a specialized TODO list, it makes
sens to try and use the same tools here.

So how do we go about doing this?  Org mode itself gets us about 80% of the
way there.  Here's a couple of examples:

``` text
#+TODO: TOREAD READING | READ DNF

* READ The Neverending Story  :fantasy:youngadult:translation:
CLOSED: [2025-03-06]
:PROPERTIES:
:Author:   Michael Ende, Ralph Manheim
:Added:    [2015-07-24]
:Rating:   :star::star::star:
:END:

* TOREAD The Language Instinct: How the Mind Creates Language     :linguistics:
:PROPERTIES:
:Author:   Steven Pinker
:Added:    [2025-01-12]
:END:

```

There's nothing fancy here. The first line defines the TODO states that
apply to this file.  Since this file is a reading list, we have two
*actionable* states: TOREAD, READING and two *terminal* or DONE states: READ
and DNF (short for Did Not Finish).  Org mode automatically puts in a CLOSED
line (as seen above) when the state of a headline moves from an actionable
one to a terminal one.  You can use this to record when you finished a book.

Each title is entered as a headline with a TODO state.  Extra information
like Author and Added date is recorded via org mode's properties feature.
I've also leveraged org mode's built in tagging system to define a basic
taxonomy for the reading list.

To display your books using org-agenda, you can add this section in your
org-agenda-custom-commands block:

``` emacs-lisp
("r" "All Books"
 ((todo "READING")
 (todo "TOREAD")
 (todo "READ")
 (todo "DNF"))
 ((org-agenda-files (file-expand-wildcards "~/org-roam-garden/books.org"))
 (org-overriding-columns-format "%TODO %50ITEM(TITLE) %30AUTHOR %30RATING")))
```

Now you can type "r" when calling up an org agenda and you'll get a basic,
compact reading list.  Even better, you can leverage org mode's column view
in the agenda by typing `C-c C-x C-c` to get a nice tabular view.

## Leveraging org-books

You can type in the books in your reading list manually, but is there a way
to automate the process?  Since this is Emacs, it would be weird if there
weren't.

Help exists in the form of a Emacs package called org-books which advertises
itself simply as "Reading list management using org-mode".  Mostly, that
means it provides elisp functions to quickly add a book to your reading list
via a URL (org-books-add-url) or ISBN (org-books-add-isbn).

The URL functionality, in particular, claims to support Amazon, Goodreads
and Open Library as sources.  What this means practice is that you can point
your browser at an Amazon book URL like this one:

https://www.amazon.ca/Piranesi-Susanna-Clarke/dp/1635577802

...use it with `org-books-add-url`.  The function is interactive, so it'll
ask you for the URL when you call it, at which point you just paste in the
one from the browser, press enter and voila - a new entry will be added to
your book file.  It looks like this:

```text
* TOREAD Piranesi                                                       :fantasy:
:PROPERTIES:
:AUTHOR:   Susanna Clarke 
:ADDED:    [2015-07-31]
:AMAZON:   https://www.amazon.ca/Piranesi-Susanna-Clarke/dp/1635577802
:END:
```

To be honest, I found the package a bit rough around the edges. While Amazon
and Goodreads URLs seemed to work, Open Library URLs had to be the book's
JSON API endpoint, not the actual web page, which I found annoying.

In addition, I found the formatting of the book entry to be a bit awkward.
In particular, I didn't like the fact that the source URL ended up embedded
in the entry's properties; in my opinion, URLs feel more natural as real
links, with real link descriptions, in the actual content section of a book
entry.

Furthermore, Open Library has a public covers API that I wanted to leverage,
particularly for later HTML export.

## Hacking org-books and Exporting my List

In the end I decided to fork org-books and make the changes myself:

 * I capitalized the data properties, but I didn't make them all uppercase
   like they were originally.
 * I embedded the source URLS as links in the entry's content, rather than
   as properties.
 * When adding a book via Open Library, I also inserted a link to the cover
   via their covers API.
 * I added a function, `org-books-add-olb`, that accepts an Open Library web
   page URL as a data source, in a similar manner to Amazon or Goodreads.
 
Because I wanted to eventually export my reading list to HTML, I made sure
that the entries were formatted in a way that made it easier to later
produce the layout I wanted via CSS.  The end result, in org mode, looks
like this:


``` text
* READ Redshirts                   :scifi:
CLOSED: [2025-01-28]
:PROPERTIES:
:Author:   John Scalzi
:Added:    [2015-07-31]
:ISBN:     9780765316998
:Rating:   :star::star::star::star:
:END:

#+BEGIN_aside
#+ATTR_HTML: :loading lazy
[[https://covers.openlibrary.org/b/ISBN/9780765316998-M.jpg]]

[[https://openlibrary.org/books/OL25253043M/Redshirts][Open Library]] \\
[[https://openlibrary.org/api/books?bibkeys=ISBN:9780765316998&jscmd=data&format=json][Open Library Data]]
#+END_aside

I expected this book to be funny, and it was.  What I was *not* expecting
was that this book would make me emotional.  I nearly cried.
```

The content has a preamble that I wrapped in a #+BEGIN_aside/#+END_aside
pair; the exported HTML will consequently be wrapped in an `aside` tag that
can be easily styled by CSS.

The formatting of the cover image is worth noting.  It looks like this:

``` text
#+ATTR_HTML: :loading lazy
[[https://covers.openlibrary.org/b/ISBN/9780765316998-M.jpg]]
```

This results in a `loading="lazy"` attribute added to the cover `img` tag in
the exported HTML.  The effect is to only load the image when the entry is
scrolled into view.  Given that I have hundreds of books in my reading list,
the exported page loads *much* faster as a result.
