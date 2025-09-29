---
date: 2025-09-28T20:20:15-04:00
title: "Implementing a Reading List in Org Mode"
blurb: "Using Emacs to quit Goodreads"
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
hellscape, is slow, dated and run by a company with dubious ethics.

I ended up deactivating my Twitter account - not that difficult given that I
never really gained a following.  As it turns out, my use of Goodreads as a
*social network* never really took off either - I don't think my friend list
ever exceeded 20 people.  I used it much more as a log of my reading habits,
a record of every book I ever read or wanted to read.  That made it easier
to think about finding a replacement.

## Alternatives to Goodreads

There are several viable alternatives to Goodreads particularly if, like,
me, you aren't that invested in the social aspect of maintaining a reading
list.  In 2025, the contenders seem to include, but are not necessarily
limited to:

 * [The StoryGraph][1]
 * [LibraryThing][2]
 * [OpenLibrary][3]
 * [BookWyrm][4]

BookWyrm, in particular, is interesting as it's part of the Fediverse and
uses the ActivityPub protocol. It's therefore truly decentralized service,
like [Mastodon][5].

Here's the thing, though.  Ever since I began participating in the
[IndieWeb][6] back in 2019, I've been been trying to live up to its
principles - in particular the part about owning your data.  And while all
these services may have the best of intentions, there's no predicting the
future.  Any one of them might go out of business, or might simply evolve to
the point where you want to leave - like Twitter did.

So what can you do?

## Settling on Emacs and Org Mode

If you read my blog, you might know that I'm a fan of [keeping my data in
plain text][15].  You might also know that I'm an [Emacs][7] enthusiast who
[cobbled together a vaguely GTD like system][8] using [Org Mode][9].  Given
that a reading list is, fundamentally, nothing more than a specialized TODO
list, it makes sense to try and do something similar here.

Please note that, as usual, this will not be an Emacs or Org Mode tutorial.
If you want to learn more about these tools, please consult the fine
documentation.

So how do we go about doing this?  Plain vanilla Org mode itself gets us
about 80% of the way there.  Here's a small example file:

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
and DNF (for Did Not Finish).  Org mode automatically puts in a CLOSED line
(as seen above) when the state of a headline moves from an actionable one to
a terminal one.  You can use this to record when you finished (or did not
finish) a book.

Each title is entered as a headline with a TODO state.  Extra information
like Author and Added date is recorded via org mode's [properties][16]
feature.  I've also leveraged org mode's built in tagging system to define a
basic taxonomy for the reading list.

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
compact reading list.  Even better, you can leverage org mode's [agenda
column view][10] by typing `C-c C-x C-c` to get a nice tabular format.

## Leveraging org-books

You can type out the books in your reading list manually, but since this is
Emacs, it would be weird if there weren't a way to automate the process.

Help exists in the form of a Emacs package called [org-books][11] which
advertises itself simply as "Reading list management using org-mode".
Mostly, that means it provides a collection of elisp functions to quickly
add a book to your reading list via a URL (`org-books-add-url`) or ISBN
(`org-books-add-isbn`).

The `org-books-add-url` function, in particular, claims to support Amazon,
Goodreads and Open Library as sources.  What this means is that you can
interactively call `org-books-add-url` and paste in, for example, an [Amazon
book URL][14] when prompted.  Press enter and voila - a new entry will be
added to your book file.  It looks like this:

```text
* TOREAD Piranesi                                                       :fantasy:
:PROPERTIES:
:AUTHOR:   Susanna Clarke 
:ADDED:    [2015-07-31]
:AMAZON:   https://www.amazon.ca/Piranesi-Susanna-Clarke/dp/1635577802
:END:
```
## Formatting with an Eye Towards Export

To be honest, I found `org-books` a bit rough around the edges. While Amazon
and Goodreads URLs seemed to work, Open Library URLs would only work with
the book's JSON API endpoint, not the actual web page, which I found
annoying.

In addition, I wasn't totally happy with the way the package formatted the
book entries.  Informing my opinion was my desire to be able to easily
export my reading to HTML, for publication on my garden.

Org mode isn't primarily a CMS and while it does support HTML export out of
the box, it doesn't really come with what I would describe as an easy to use
templating system.  What this means in practice is that, without some fairly
major configuration, org-mode HTML export will result in HTML that more or
less mirrors the original content in terms of structure and semantics.

What I'm trying to say is that if you want your exported HTML to be structured
certain way, it often easiest to arrange for your org source to be structured
in a similar way.

Related to this is the fact that org-books, seemingly as a matter of policy,
formats all of its book entries exclusively as "headlines + properties",
presumably reserving the content area for extra comments about work itself.

This can be a problem when it comes time to export the entry to HTML,
because properties are treated as special in org-mode's export system.  So
that URL in the AMAZON property you see above?  Since it isn't formatted
as an org-mode link, the result in HTML is a plain, unclickable URL.  It's
actually worse than that - since propeties are exported as plain text by default,
even if you format the URL as a real link, the exported result will still
be just plain text, not HTML.  After some amount of research, I came to the conclusion
that the easiest way to get an actual link in the output was to embed the
Amazon URL as a real org-mode link, in the *content* of the entry, not the properties - and proper
org-mode links *do* get turned into HTML links during export, as you might
expect.

I also thought it would be nice to have display cover image for each book
entry, particularly in the exported HTML, and Open Library does have a
public [covers API][12] that I suspected I could leverage.  Org-books, so
far as I could tell, doesn't insert any cover images at all.  In any case,
the right place for it, assuming we wanted it to appear in the exported
HTML, would be as an image in the content, not as a property.

All this to say, org-books wasn't totally cutting it.  So what to do?

## Hacking org-books

In the end I decided to fork org-books and make [some changes][13]:

 * I capitalized the data properties, but I didn't make them all uppercase
   like they were originally.  The property names appear in the HTML as is
   so I wanted something less "in-your-face".
 * I embedded the source URLs as proper links in the entry's content, rather
   than as properties.
 * When adding a book via Open Library, I also inserted a link in the
   entry's content to the cover via their covers API.
 * I added a function, `org-books-add-olb`, that accepts an Open Library web
   page URL as a data source, in a similar manner to Amazon or Goodreads.
 
I made sure that the entries were formatted in a way that made it easier to
later produce the layout I wanted via CSS.  In the end, a typical entry
looked like this:


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
pair; when exported to HTML, this results in the enclosed content being
wrapped in an `aside` tag that can be easily styled by CSS.

The formatting of the cover image is worth noting.  It looks like this:

``` text
#+ATTR_HTML: :loading lazy
[[https://covers.openlibrary.org/b/ISBN/9780765316998-M.jpg]]
```

This results in a `loading="lazy"` attribute added to the cover `img` tag in
the exported HTML.  The effect is to only load the image when it's scrolled
into view.  Given that I have hundreds of books in my reading list, the
exported page loads *much* faster as a result.

## Example and Final Thoughts

I publish my [reading list][17] to my garden.  You can peruse it there.

Is this thing a Goodreads killer?  Will it replace a real book tracking app
for the average person?  Good God, no.  I'm the first to admit the whole
process is clunky as hell.

But apps comes with trade offs, as any IndeWeb enthusiast is aware, and one
of the things you give up when you use one is full control over your data.
The one big advantage of all this trouble is that my reading list is mine
and mine alone.  I consider that important.

Important enough to always resist the siren son of a dedicated service for
years to come?  Maybe not - that's why the IndeWeb has a prescribed way for
sharing your data with the rest of the world: [POSSE][18].  This is the
practice of publishing to your own site first and then deciding how you tell
the world about it.  It's something I do on my blog - all posts are
automatically shared to Mastodon once published.

To be honest I'm not sure how POSSE would look in this case; I'm not even
sure what service I'd share to, or how I'd figure out what to share.  Still,
it's an interesting idea worth exploring.

In the meantime, this serves my needs, and it was fun to do.

[1]: https://thestorygraph.com/
[2]: https://www.librarything.com/
[3]: https://openlibrary.org/
[4]: https://bookwyrm.social/
[5]: https://joinmastodon.org/
[6]: https://indieweb.org/
[7]: https://www.gnu.org/software/emacs/
[8]: /2023/12/05/gtd-org-mode
[9]: https://orgmode.org/
[10]: https://orgmode.org/manual/Agenda-Column-View.html
[11]: https://github.com/lepisma/org-books
[12]: https://openlibrary.org/dev/docs/api/covers
[13]: https://github.com/drivet/org-books/compare/master...drivet:org-books:customizations
[14]: https://www.amazon.ca/Piranesi-Susanna-Clarke/dp/1635577802
[15]: /2023/09/14/plain-text
[16]: https://orgmode.org/manual/Properties-and-Columns.html
[17]: https://garden.desmondrivet.com/books
[18]: https://indieweb.org/POSSE
