---
date: 2023-10-17T22:24:35-04:00
title: "Implementing GTD in Emacs With Org Mode"
blurb: "There are lots of ways to do it"
post-status: draft
mp-syndicate-to:
- mastodon
tags: 
- emacs
---

I have an on-again, off-again relationship with productivity systems.  I
usually don't bother with them but on those occasions when my life gets
hectic (it's not too often) I will sometimes make use of one.  When that
happens I always fall back to David Allen's Getting Things Done (GTD).  I
read his book a long time ago and, at the time, found myself impressed.

The basic, fundamental feature of the system is that it downplays the
classic, detailed, prioritized to-do list in favour of the so-called *next
action* list.  In all of your goals, instead of allowing yourself to become
overwhelmed with all the myriad things you know you need to do, you instead
focus only on the *very next thing* you can do to move closer to your goal,
and write that down.  The end result is an unsorted list of things you can
do in the moment, all in the service of the various goals you have decided
to put on your radar.

The result can be *also* be, as you might imagine, a very long list of next
actions.  To manage this, Allen recommends the use of *contexts* - keywords
that describe when or where it's possible to do a next action.  The idea is
that when you find yourself in one of these contexts, you're supposed to be
able to pull up all the next actions attached to it and pick one off the
list.

In the 90's, before cellphones were widespread, a classic context would have
been "phone", which you'd attach to any next action involving a phone call.
When you found yourself near a phone, you'd pull up all your phone-related
next actions start cranking through them one by one.  Another classic one
would have been the "computer" context, back in the days when a "computer"
meant a machine sitting on a desk somewhere.  You'd often attach that one to
a next action involving an email, for example,

These days everyone has a smartphone, so the "phone" and "computer" contexts
don't really make much sense anymore.  That doesn't mean contexts are
useless as a concept, but it does mean that you need to think a bit more
about how to use them.  The key thing is to remember is that they're meant
to be *opportunistic*.  You're meant to be able to pull up a particular
context once you find yourself in a matching situation and immediately be
presented with a series of next actions that you can do "in the moment".  My
contexts these days are often people or place oriented.  If I have a
Tupperware or a book that I need to return to my Mom, for example, I'll
attach the "mom" context to those next actions and remember to consult it
before visiting her (spoiler: I often don't remember to consult it).

In addition to a next action list, GTD also has a notion of a *project*
list.  The word "project" can mean a lot of things but when we talk of GTD,
it means nothing more than a goal that requires more than one "next action"
to accomplish.  The metaphor that often gets used in the GTD community is
that a project is a "stake in the ground", a reminder that there is a
specific, somewhat abstract goal in your life that needs addressing.
Effectively, your project list defines the "hard landscape" of your life,
around which your next actions revolve.

Maintaining separate next action and project lists forces you to think in
very concrete terms about how to accomplish a particular objective.

One thing that I find interesting about GTD is that the connection between
your next actions and your projects is a lot looser than it might seem at
first glance.  Yes, a next action can be, and often is, associated with a
project - but it doesn't have to be, and it's perfectly okay to have
"freestanding" next actions that aren't attached to anything at all.  In the
same vein, a project will often have at least one next action under it - but
it might not, if you haven't figured out what that next action is.  Allen
even has a name for projects that have no next actions - they're called
"stuck projects" and it's rather revealing that it seemingly happens often
enough that he labelled the concept.

GTD defines two more lists:

 * Waiting
 * Someday/Maybe

The Waiting list is for things that are "outsourced" - that you can't do
yourself.  Waiting for someone to respond to an email, for example, is a
classic Waiting list item.

The Someday/Maybe list is meant to provide a haven for any vague and hazy
to-do item that you don't really want to work on "right now".  These could
be aspirational ("Learn photography") or just stuff that you might want to
get to later on in life ("Replace ceiling fan").

# Implementing GTD in Emacs with Org Mode

As you might imagine, there are lots of ways to implement GTD, both analog
and digital.  For a while, in the early 2000's, it was fashionable in
certain circles to implement it with a notebook that you carried around with
you.  Most people, however, use some kind of software.

I'm an Emacs user, or at least I try to be, and so it makes sense for me to
try and implement GTD with Emacs.  The package one usually hears about in
this regard is Org, a note keeping tool that has evolved over the years
to include task management and scheduling functionality.

Both Emacs and Org are huge and I'm not about to give a full-fledged
tutorial on either of them (I'm a perpetual novice myself) so instead I'll
focus on what I want out of my GTD system and how I can get Org to do it.

Roughly speaking, I want my GTD system to do the following:

 * Add new items easily, probably via a templating system
 * Separately view, at a glance, my next actions, waiting, projects, and
   someday items.
 * Be able to easily move items from one list to another (for example,
   promote a Someday item to a Project or vice versa)
 * Be able to archive "done" items easily, so they don't clog up my lists.

Adding new items is easily accomplished with org-capture, but with some
caveats that I'll get into shortly.  Viewing my next actions, waiting,
projects and someday lists is easily accomplished with org-agenda, again
with some qualifications that I will get into shortly.

Moving and archiving items are similar tasks, and Org has built in
functionality to do them, but the process can be easier or harder depending
on how you structure your lists so it's worth considering.

So, with all that in mind, let's consider different approaches to
implementing GTD in Org.

## The Literal Approach

TLDR: I don't actually use this method, because I think the cons outweigh
the pros, but I find it useful nonetheless to ferret out what works and what
doesn't.

One very obvious way to implement GTD is to maintain a separate file for
each list.  Your Next Actions, for example, could live in an actions.org
file that looks like this:

  * TODO do this thing
  * TODO do this other thing

And a projects.org file that looks pretty much the same:

  * TODO project 1
  * TODO project 2

The waiting and someday lists would look almost exactly the same, in their
own respective files.

GTD contexts can be represented with Org tags, tacked onto the end of the
item, like so:

  * TODO do this thing :mom:
  * TODO do this other thing :plumber:

Org has built in ways of filters the agenda view based on the tags you add
to each item.

This approach has some advantages, not the least of which is that it's dead
simple to implement, explain and understand.  The fact that all items have
the same TODO state and are all at the same outlining "level" means that
adding new items, moving items one list to another, and archiving items are
all dead simple as well.

What about viewing your individual lists?  It turns out this is pretty
simple as well.  Org TODO items have a property called a *category*.  If you
don't do anything special, the category is just the file from which the item
originates.  So any item in the actions.org file has an *action* category
and the same goes for items in your other files.  Defining an agenda view
based on a category is trivial in Org, so it's very straightforward to
create separate views for your next actions, projects, and someday items.
It is also surprisingly useful to define a view that includes multiple item
types - let's say next actions and projects - because by default the Org
agenda will also display the category in its own column, allowing you to
easily distinguish between the item types.

Are there any disadvantages?  There is only one that I can think of but it's
significant: there's no explicit relationship between Next Actions and
Projects.  In other words, you can't tell at a glance what the next actions
are for a project, or which project a next action belongs to.

One can, of course, come up with scheme to compensate.  Org has a feature
called drawers that let you add arbitrary properties to TODO items.  Add a
Project ID to each project item, and a Project ID to each Next Action that
belongs to a project and you have a theoretical way of connecting the two
lists.

Unfortunately, theoretical is not the same as practical.  Ideally we'd like
to see a Next Action's project in the agenda view but this isn't possible
here without some special configuration - which is of course entirely
possible.  More problematic is the reverse - seeing all Next Actions
associated with a project.  There's no view built into Org that'll let
you do that out of the box.  You'd have to design a new view and write the
code to produce - not the easiest of tasks.

And if you ever want to archive an entire project - or move it to the
Someday list - it would make sense to move all the associated Next Actions
along with it.  Org certainly doesn't do this out of the box, and honestly
I'm having trouble imagining what that feature would even look like.

Let's see what else we can come up with.

## Projects as Hierarchy

Another, arguably more natural approach, involves putting all of a projects
Next Actions under the projects heading, like this:

  * TODO Project 1
  ** TODO item 1
  ** TODO item 2
  * TODO Project 2
  ** TODO item 3
  ** TODO item 4

We can also maintain a separate next action list for those that are not
connected to any particular project:

  * TODO item 5
  * TODO item 6

This is slightly messy since we now have Next Actions spread across
different "levels" depending on whether they belong to a project or not. In
other words, sometimes a Next action looks like this:

  * TODO blah
 
and sometimes it looks like this:

  ** TODO blah

which can make adding and templating new items problematic.  We can
therefore adjust our standalone, projectless next action list to look like
this:

  * Miscellaneous
  ** TODO item 5
  ** TODO item 6

Note that the Next Action header does not have a TODO state attached -
because it's not a project.  It's a pure header that we don't actually
want showing up in the agenda.  It exists pretty much to put standalone Next
Actions on the same "level" as project Next Actions, making them more
uniform.

At this point, it makes sens to combine the projects and next actions into
one file, so all your next actions are located in just one file:

  * Next Actions
  ** TODO item 5
  ** TODO item 6
  * TODO Project 1
  ** TODO item 1
  ** TODO item 2
  * TODO Project 2
  ** TODO item 3
  ** TODO item 4

What do we do with the waiting items?  These could remain in their own
waiting.org file, but it's entirely possible for a particular waiting item
to be associated with a project (or not - they're like next actions that
way).  We could nestle the waiting items among the next actions in our file
but that leads to a problem: how do we distinguish next actions from waiting
items?

The answer here is to add new TODO states, something Org allows you to do:

  * Next Actions
  ** TODO item 5
  ** TODO item 6
  * TODO Project 1
  ** TODO item 1
  ** TODO item 2
  ** WAIT another item
  * TODO Project 2
  ** TODO item 3
  ** WAIT email response
  ** TODO item 4

To make things even more explicit, you could use a special TODO state for
your projects too:

  * Next Actions
  ** TODO item 5
  ** TODO item 6
  * PROJ Project 1
  ** TODO item 1
  ** TODO item 2
  ** WAIT another item
  * PROJ Project 2
  ** TODO item 3
  ** WAIT email response
  ** TODO item 4

As an added bonus, all next actions in this scheme are under a heading,
which makes it easy to assign them all to a category, like this:

  * Next Actions
   :PROPERTIES:
   :CATEGORY: next
   :END:
  ** TODO item 5
  ** TODO item 6
  * PROJ Project 1
   :PROPERTIES:
   :CATEGORY: proj1
   :END:
  ** TODO item 1
  ** TODO item 2
  ** WAIT another item
  * PROJ Project 2
   :PROPERTIES:
   :CATEGORY: proj2
   :END:
  ** TODO item 3
  ** WAIT email response
  ** TODO item 4

If you make your category name roughly match your project name, then they
can act as a suitable stand-in when viewing Next Actions on the agenda.
This means you can easily see, at a glance, which project your Next Action
belongs to, or if it stands on its own.

Seeing all the next actions associated with a project doesn't involve using
org-agenda - you just have to look at the project section itself, and
examine the next actions under the project heading.

You could integrate your someday list into this master list, but I actually
like having it in a separate file, since it can get long and rambling, and I
usually interact with it on a different cadence than my projects and next
actions.  Keeping it in its own file makes sense.

If you keep the someday items in their own file, you have a choice about
they're represented.  You can leverage the fact that all the items have a
"someday" Org context derived from the file (not to be confused with a GTD
context) or you can use a special TODO state for them like this:

  * SOMEDAY learn photography
  * SOMEDAY take up skydiving

I'm partial to the latter approach since it mirrors how it's done in the
main file.  Consistency and all that.

Moving items between the main file and the someday file is relatively
straightforward, since projects and someday items are at the same Org level.
The only caveat is that the TODO state needs to be changed when changing
files (turning a project into a someday item, for example, not only involves
moving it to the someday file, but also involves changing the state from
PROJ to SOMEDAY).  This feels like something that can be easily done with
e-lisp.

Lastly, how does one archive items that are "done"?  Recall that in the
naive case, this was easy, since you could just move a done item to the
bottom of the archive file with no massaging at all.  With the new system,
next action and waiting items are always second-level.  It makes sense to
carry the entire tree-context of the items with you when you archive it -
the project header, for example.

It turns out that Org has a built in mitigation for this: the ARCHIVE
property in the heading drawer, which specifies the archive heading for
items underneath.  So you can write something like this:

  * Next Actions
   :PROPERTIES:
   :CATEGORY: next
   :ARCHIVE: %s_archive::* Next Actions
   :END:
  ** TODO item 5
  ** TODO item 6
  * PROJ Project 1
   :PROPERTIES:
   :CATEGORY: proj1 
   :ARCHIVE: %s_archive::* Project 1
   :END:
  ** TODO item 1
  ** TODO item 2
  ** WAIT another item
  * PROJ Project 2
   :PROPERTIES:
   :CATEGORY: proj2 
   :ARCHIVE: %s_archive::* Project 2
   :END:
   :END:
  ** TODO item 3
  ** WAIT email response
  ** TODO item 4
