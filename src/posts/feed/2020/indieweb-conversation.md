---
blurb: 'In which I describe how to avoid solipsism on the #indieweb'
date: 2020-04-05T01:30:09.000Z
modified: 2020-04-05T01:30:09.000Z
tags:
  - indieweb
title: How to Have a Conversation on the IndieWeb
mp-syndicate-to:
  - twitter_bridgy
---

If you've read my [previous articles][1] on the [IndieWeb][2], you might be
forgiven for thinking that its members are, by and large, loners who keep to
themselves.

Consider the concept of a "like", for example.  On a site like Twitter, a
like is an *action* you perform against another person's content; you click
the heart icon next to someone's tweet, and the like counter for that tweet
goes up.  It's an implicit connection between two people - the one who did
the liking and the one who received it.

An [IndieWeb "like"][3], on the other hand, is not an action you perform on
someone's content, but rather a standalone post that you *own* and publish
to your site.  It's a reversal of the way people usually think about the
transaction, and it reflects the premium IndieWeb members place on
controlling their own content.

Notably, in the simplest version of this scheme, the person who's post was
liked - the likee, I guess? - might not even be aware that anything has
happened at all.  I mean, how would they?  The entire interaction was wholly
contained on another site.

Which is all well and good but also a little...boring? Calling the IndieWeb
a "social network" seems like a bit of stretch when you don't even know how
many people liked your post.  What's the point of it all if you don't get to
see a little heart counter going up?

The problem becomes more obvious when one realizes that the IndieWeb does
not prescribe the use of any particular blogging software or, indeed, any
particular publishing technology at all (other than the standard machinery
of the web, like HTTP, HTML and CSS), so how exactly would one be able to
even recognize that a piece of content on someone's website was a "post"
that could be "replied to" or "liked" at all?

Of course, we wouldn't get very far if the IndieWeb community didn't have
answers to these questions.

## How To Build a Decentralized Social Network

It's been said that the IndieWeb is what a social network looks like when
it's built on top of the web itself, but what exactly does this mean?

If you're a software developer, you've heard of [REST][4].  It's a software
architecture style defined in terms of distributed [web resources][15]
(uniquely identified by URLs) and a small number of predefined actions or
verbs that can be applied to those web resources - the same verbs, not
coincidentally, defined by HTTP (GET, POST, PUT, DELETE, etc).  The REST
style is used extensively to provide access to distributed web-based data
resources, often formatted as JSON.

[Roy Fielding][5] is usually credited as the person who "invented" REST, but
that's not quite true.  It would be more accurate to say that he documented
a pre-existing architecture style - namely the one used by the classic world
wide web.  The entire web is the prototypical REST system *par excellence*
in this formulation, with hyperlinked HTML documents being the prototypical
web resources.

I'm not personally accustomed to viewing the web in this fashion, because
I'm more accustomed to thinking of RESTful services as providing access to
data resources, not HTML pages, and I would argue that the distinction is
more than just semantics.  Data resources, in my mind, have a regular,
predictable structure that HTML pages lack.  Data resources represent things
like books and appointments and people and blog posts while HTML pages are
just...pages.  They are primarily meant to be rendered in a browser and
viewed by human beings and extracting any kind of structured data from them,
while not impossible, tends to be tricky.

And this puts the IndieWeb in a bit of bind.  The IndieWeb's basic unit of
communication is the post and posts live on websites and websites consist of
HTML pages and HTML pages, as I've already said, are not structured data
resources.  So, how does one (programatically) extract a post from a website
that one has never visited before?  How does one distinguish notes from
articles and likes?  These are questions that are readily answered if you're
designing a private, walled social network, because you get to design the
data resources from the ground up.  You get to decide what a post looks
like, and the web resources involved in modelling one.  It's a lot harder to
do when you have to somehow extract that information in a predictable manner
from the chaotic tag soup that forms the majority of personal websites.

## Enter Microformats

So what can you do?  What exactly *does* a post look like on the IndieWeb?
Finally we come to the answer, and it requires knowledge about a subject
called *[microformats][6]*.

Microformats are basically just specialized CSS classes added to your HTML
content in order to give consumers some idea of what that content
represents.  They are effectively the API of the IndieWeb.  Different kinds
of posts can be represented with different combinations of microformats.

A standard blog entry (an *[article][7]*, in IndieWeb parlance), might look
like this, for example:

``` html
<div class="h-entry">
    <h1 class="p-name">Awesome Title<h1>
    <div class="e-content">
        This is a pretty good blog entry
    </div>
</div>
```

The "[h-entry][8]" CSS class identifies the whole div as a generic post.
The "p-name" class identifies the h1 tag as the title of the post and the
"e-content" class represents the article content itself.  We know this is a
*blog entry* because of the title; blog entries are really nothing more then
self-contained, standalone pieces of content with titles.

(If you're curious about why microformat classes start with a certain
letter, like h or p, you should read the [standard][9].  The specifics,
while interesting, are not germane to this discussion).

A [note][10] (the IndieWeb equivalent of a tweet) looks more or less the
same as an article, but lacks a title:

``` html
<div class="h-entry">
    <div class="e-content">
        This is a pretty good note
    </div>
</div>
```

IndieWeb [reply][11] posts are basically just notes with extra information
to give the reply some context:

``` html
<div class="h-entry">
    <a class="u-in-reply-to" href="http://elsewhere.com/posts/1234">
    <div class="e-content">
        This is a pretty good reply
    </div>
</div>
```

Note the telltale "u-in-reply-to" URL, which identifies this post as a
reply, and points to the post that this reply is about.

An IndieWeb like is very similar to an IndieWeb reply, but uses the
"u-like-of" class instead of the "u-in-reply-to" class:

``` html
<div class="h-entry">
    <a class="u-like-of" href="http://elsewhere.com/posts/1234">
    <div class="e-content">
        This is a pretty good like
    </div>
</div>
```

Though likes can contain content of their own, they often don't, so they can
often be as simple as this:

``` html
<div class="h-entry">
    <a class="u-like-of" href="http://elsewhere.com/posts/1234">
</div>
```

## From Microformats to JSON

Once you embed your posts with microformats, how does a 3rd party make use
of the information?  They do it with a *microformat parser*.  Microformat
parsers function by reading the special microformat classes from an HTML
document and producing a canonical JSON representation of the extracted
data.  So, for example, this markup:

``` html
<div class="h-entry">
    <h1 class="p-name">Awesome Title<h1>
    <div class="e-content">
        This is a pretty good blog entry
    </div>
</div>
```

will produce this JSON:

``` json
{
  "items": [
    {
      "type": [
        "h-entry"
      ],
      "properties": {
        "name": [
          "Awesome Title"
        ],
        "content": [
          {
            "value": "This is a pretty good blog entry",
          }
        ]
      }
    }
  ]
}
```

according to rules defined by the standard.

Bear in mind that any HTML element without an associated microformatted
class is effectively ignored by a microformat parser, so a post that looks
like this:

``` html
<article class="h-entry">
    <div class="something-weird">
        This isn't really part of the content
    </div>
    <h1 class="p-name">Awesome Title<h1>
    <span class="wrap">Something else here</span>
    <div class="e-content">
        This is a pretty good blog entry
    </div>
    <footer>
        This is an awesome footer
    </footer>
</article>
```

will produce exactly the same JSON as the post above.

In other words, microformats are basically a way of embedding structured
data into an unstructured HTML page.  Effectively, microformats allow a
vanilla, free form personal website to double as a RESTful data service.

I'll say this again, because I think it's kind of cool: *microformats
transform a plain, vanilla, HTML-based personal website into a RESTful data
service*.

I feel like Roy Fielding would be proud.

## Counting Hearts

The process of liking a post on the IndieWeb, as I alluded to before, might
seem like a very one-sided activity.  The liker publishes a special post on
their own site which represents the "like" and that's pretty much that.  The
likee isn't really involved in the procedure.

Except, of course, that if this were actually true, the IndieWeb web
wouldn't be much of a social network.  So how exactly do you know how many
people liked your post?

The answer involves a concept called *[webmentions][12]*.  A webmention is a
way of notifying a website that another website has linked to it in some
fashion.  The basic operation is as follows:

* Bob writes an article which links to an article on Alice's website.
* Bob publishes his article, and he (or the software he uses) sends a
  webmention to an endpoint on Alice's domain, informing her that her
  article was mentioned in Bob's post.
* Alice's endpoint does something with this information (perhaps recording
  the fact in a database, for example).

The webmention protocol is remarkably spare; the payload basically contains
nothing but the source URL (doing the mentioning) and the target URL (being
mentioned).  Notably absent is any sort of information about what kind of
post is doing the mentioning.  Bob's post could have liked Alice's article,
or it could have replied to it, or it could have just randomly linked to it;
the webmention payload is silent on the subject.

Information about the post which sent the webmention must obtained by
actually *reading* the post via the source URL, parsing its microformats,
and confirming that the target URL actually appears somewhere in its
contents.  These extra steps function as form of validation; a valid
webmention *must* be backed by a real, published post or it will be
summarily rejected.  This step also allows the consumer of the source post
to classify it according to its microformats.  This is ultimately how one
implements things such as like counters and comments on an IndieWeb site.

## Here's My Card

Webmentions allow a website to know when a person has linked to your post
and a further check can determine the nature of link (was it a like? A
reply? A repost?).  Notably missing from the ongoing discussion is a crucial
piece of the puzzle, namely the identity of the person doing the linking.

Of course, what we *do* know is the website that did the linking, and if
you've read my [previous article][13] on the subject, you know that one's
domain is basically one's identity on the IndieWeb, so we're halfway there.
But if you've used a service like Twitter, you know that this isn't really
enough; one's personal domain is akin to one's Twitter handle, and although
such information is crucial, it's usually augmented with things like a name
and an avatar.  How do you provide things like that on the IndieWeb?

The answer, once again, is microformats, but this time it's not an h-entry
on which we'll focus but rather a new class called an [h-card][14].  The
h-card microformat functions roughly like an online business card; it's way
of attaching personal information, like your name, email address and photo,
to your personal domain.  A properly embedded h-card becomes especially
important when you want your personal information to appear correctly in a
context other than your own website - for example, when posting likes and
replies that you want displayed on other websites.

Embedding an h-card in your posts is not difficult:

``` html
<div class="h-entry">
    <a class="p-author h-card" href="http://example.com">Joe Bloggs</a>
    <h1 class="p-name">Awesome Title<h1>
    <div class="e-content">
        This is a pretty good blog entry
    </div>
</div>
```

As you might expect, the p-author class tells a consumer who the author of
the post is, but it's the h-card class which tells you the format of that
information.  This is a very minimal example, making use of a concept called
[implied properties][17], which allows parsers to infer certain pieces of
data from the structure of the tags.  The result of parsing the h-card, in
this case, will look like this:

``` json
{
  "items": [
    {
      "type": [
        "h-card"
      ],
      "properties": {
        "name": [
          "Joe Bloggs"
        ],
        "url": [
          "http://example.com"
        ]
      }
    }
  ]
}
```

As you can see, the url attribute is inferred from the fact that the h-card
is combined with an anchor tag, and the name attribute is inferred from the
contents of the tag.

You can choose to be more specific with your h-cards:

``` html
<div class="h-entry">
    <p class="h-card">
        <img class="u-photo" src="http://example.org/photo.png" alt="" />
        <a class="p-name u-url" href="http://example.org">Joe Bloggs</a>
        <a class="u-email" href="mailto:joebloggs@example.com">joebloggs@example.com</a>, 
    </p>
    <h1 class="p-name">Awesome Title<h1>
    <div class="e-content">
        This is a pretty good blog entry
    </div>
</div>
```

This results in the following parsed h-card:

``` json
{
  "items": [
    {
      "type": [
        "h-card"
      ],
      "properties": {
        "photo": [
          "http://example.org/photo.png"
        ],
        "name": [
          "Joe Bloggs"
        ],
        "url": [
          "http://example.org"
        ],
        "email": [
          "mailto:joebloggs@example.com"
        ],
      }
    }
  ]
}
```

You can also choose to maintain a canonical or [representative h-card][15]
which represents your *homepage*.  You do this by:

* Adding an h-card to your homepage
* Making sure that your homepage h-card contains a u-uid attribute to
  represent the page itself.
  
My representative h-card on my homepage looks like this, for example:

``` html
<section class="h-card">
    <ul>
        <li>
            <img class="u-photo" src="/me02.jpg"/>
        </li>
        <li>
            <a class="p-name u-url u-uid" 
                href="https://desmondrivet.com">Desmond Rivet</a>
        </li>
        <li>
            <a class="u-email"
                href="mailto:desmond.rivet@gmail.com">desmond.rivet@gmail.com</a>
        </li>
    </ul>
</section>
```

This will produce the following parsed JSON:

``` json
{
  "items": [
    {
      "type": [
        "h-card"
      ],
      "properties": {
        "photo": [
          "http://desmondrivet.com/me02.png"
        ],
        "name": [
          "Desmond Rivet"
        ],
        "url": [
          "http://desmondrivet.com"
        ],
        "uid": [
          "http://desmondrivet.com"
        ],
        "email": [
          "mailto:desmond.rivet@gmail.com"
        ],
      }
    }
  ]
}
```

## Onward!

At this point, I think I've covered most of what goes into a basic
IndieWeb-enabled website.

That doesn't mean, of course, that there isn't more to write, but most of
the topics from here on out will probably be a bit more specific to my
situation.

For example, this site is built using [Pelican][18], a static site generator
based on [Python][19], and there are a whole *batch* of issues that crop up
when you try to integrate certain IndieWeb features with *that* combination.
I would be remiss if I didn't write about them, but we'll see.

Anyway, thanks for reading this far!  I hope to see you around!


[1]: /tags/indieweb
[2]: https://indieweb.org/
[3]: https://indieweb.org/like
[4]: https://en.wikipedia.org/wiki/Representational_state_transfer
[5]: https://en.wikipedia.org/wiki/Roy_Fielding
[6]: https://indieweb.org/microformats
[7]: https://indieweb.org/article
[8]: https://indieweb.org/h-entry
[9]: http://microformats.org/wiki/microformats2
[10]: https://indieweb.org/note
[11]: https://indieweb.org/reply
[12]: https://indieweb.org/Webmention
[13]: https://desmondrivet.com/2020/02/12/website-identity
[14]: https://indieweb.org/h-card
[15]: https://indieweb.org/representative_h-card
[16]: https://en.wikipedia.org/wiki/Web_resource
[17]: http://microformats.org/wiki/microformats2-implied-properties
[18]: https://blog.getpelican.com/
[19]: https://www.python.org/
