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

I have an on-again, off-again relationship with productivity systems.  I'm 


I'm
not a consistent follower of one, but every so often my life will become
hectic enough to make one useful.  When that happens I always fall back to
David Allen's Getting Things Done (GTD).  I read his book a long time ago
and, at the time, found myself impressed.

The basic, fundamental feature of the system is the downplaying of the
classic, detailed, prioritized to-do list in favour of the *next action*
list.  In all of your goals, instead of allowing yourself to become
overwhelmed with all the myriad things you know you need to do, you instead
focus only on the *very next thing* you can do to move closer to your goal,
and write that down.

The result can be, as you might imagine, a very long list of next actions.
To manage this, Allen recommends the use of *contexts* - keywords that
described when it's possible to do a next action. In the 90's, for example,
before cellphones were widespread, a classic context would have been
"phone", which you'd attach to any next action involving a phone call.  The
idea is that if you ever found yourself near a phone, you'd pull up all the
next actions with that context and start cranking through your phone calls.
Another classic one would have been the "computer" context, back in the days
when a "computer" meant a machine sitting on a desk somewhere.  You'd often
attach that one to a next action involving an email, for example,

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
before visiting her.

In addition to a next action list, GTD also has a notion of a *project*
list.  The word "project" can mean a lot of things but when we talk of GTD,
it's nothing more than a goal that requires more than one "next action" to
accomplish.  The metaphor that often gets used in the GTD community is that
a project is a "stake in the ground", a reminder that there is a specific
goal in your life that needs addressing.  Effectively, your project list
defines the "hard landscape" of your life, around which your next actions
revolve.

Maintaining separate next action and project lists forces you to think in
very concrete terms about how to accomplish a particular objective.

One thing that I find interesting is that the connection between your next
actions and your projects is a lot looser than it might seem at first
glance.  Yes, a next action can be, and often is, associated with a
project - but it doesn't have to be, and it's perfectly okay to have a
"freestanding" next action that isn't attached to anything at all.  In the
same vein, a project will often have a next action under it - but it might
not, if you haven't figured out what that next action is.  Allen even has a
name for projects that have no next actions - they're called "stuck
projects" and it's rather revealing that it seemingly happens often enough
that he found it necessary to label the concept.

Finally, GTD defines one more list: the Someday/Maybe list.  This is meant
to provide a home for any vague and hazy to-do item that you don't really
want to work on "right now".  These could be aspirational ("Learn
photography") or just stuff that you might want to get to later on in life
("Replace ceiling fan").

# Implementing GTD in Emacs with Org Mode

Let's recap.  GTD mandates the following lists:

 * Next Actions
 * Projects
 * Someday/Maybe

Furthermore:

 * Items can be tagged with a *context*, and you need to be able to pull up
   all items with the same context.
 * Next Actions often belong to Projects, but they don't have to.
 * Projects often have Next Actions, but they don't have to.

As you might imagine, there are lots of ways to implement, both analog and
digital.  For a while, in the early 2000's, it was in vogue in certain
circles to implement it with pencil and paper.  Most people, however, use
some kind of software.

I'm an Emacs user, or at least I try to be, and so it makes sense to try and
implement GTD with Emacs.  The package one usually hears about in this
regard is org mode, an outlining tool that has evolved over the years to
include task management and scheduling functionality.

Both Emacs and Org mode are huge and I'm not about to give a full fledged
tutorial on how to use them (I'm a perpetual novice myself) so instead I'll
focus on what I want out of my GTD system and how I can get Org Mode to do
it.

Roughly speaking, I want my GTD system to do the following:

 * Add new items easily, probably via a templating system
 * Separately view, at a glance, my next actions, projects, and someday
   items.
 * Be able to easily move items from one list to another (for example,
   promote a Someday item to a Project or vice versa)
 * Be able to archive "done" items easily, so they don't clog up my lists.

Adding new items is easily accomplished with org-capture, but with some
caveats that I'll get into shortly.  Viewing my next actions, projects and
someday lists is easily accomplished with org-agenda, again with sme
qualifications that I will get into shortly.

Moving and archiving items are similar tasks, and org mode has built in
functionality to do them, but the process can be easier or harder depending
on how you structure your lists so it's worth considering.

So, with all that in mind, let's consider different approaches to
implementing GTD in Org Mode.

## The Literal Approach

TLDR: I don't actually use this method, because I think the cons outweigh
the pros, but I find it useful nonetheless to ferret out what works and what
doesn't.

One very obvious way to implement to GTD is to maintain a separate file for
each list.  Your Next Actions, for example, could live in an actions.org
file that looks like this:

* TODO do this thing
* TODO do this other thing

And a projects.org file that looks pretty much the same:

* TODO project 1
* TODO project 2

And finally a someday.org file with exactly the same format:

* TODO Learn Spanish
* TODO Renovate the kitchen

This approach has some advantages, not the least of which is that it's dead
simple to explain and understand.  The fact that all items have the same
TODO state and are all at the same outlining "level" means that adding new
items, moving items one list to another, and archiving items are all dead
simple as well.

What about viewing your individual lists?  It turns out this is pretty
simple as well.  Org Mode TODO items have a property called a *category*.
If you don't do anything special, the category is just the file from which
the item originates.  So any item in the actions.org file has an *action*
category (and the same goes for projects and someday items).  Defining an
agenda view based on a category is trivial in org mode, so it's very
straightforward to create separate views for your next actions, projects,
and someday items.  You can *also* define a view that includes just your
next actions and projects, and this is a surprisingly useful view since by
default the org agenda will also display the category in its own column,
allowing you to easily distinguish between the actions and the projects.

Are there any disadvantages?  Only one that I can think of but it's
significant: there's no explicit relationship between Next Actions and
Projects.  In other words, you can't tell at a glance what the next actions
are for a project, or which project a next action belongs to.

One can, of course, come up with scheme to compensate.  Org mode has a
feature called drawers that let you add arbitrary properties to TODO items.
Add a Project ID to each project item, and a Project ID to each Next Action
that belongs to a project and you have a theoretical way of connecting the
two lists.

Unfortunately, theoretical is not the same as practical.  Ideally we'd like
to see a Next Action's project in the agenda view but this isn't possible
here without some special configuration - which is of course entirely
possible.  More problematic is the reverse - seeing all Next Actions
associated with a project.  There's no view built into org mode that'll let
you do that out of the box.  You'd have to design a new view and write the
code to produce - not the easiest of tasks.

And if you ever want to archive an entire project - or move it to the
Someday list - it would make sense to move all the associated Next Actions
along with it.  Org mode certainly doesn't do this out of the box, and
honestly I'm having trouble imagining what that feature would even look
like.

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
therefore adjust our next action list to look like this:

* Next Actions
** TODO item 5
** TODO item 6

Note that the Next Action header does not have a TODO state attached -
because it's not a list item.  It's a pure header that we don't actually
want showing up in the agenda.  It exists pretty much to put standalone Next
Actions on the same "level" as project Next Actions, making them more
uniform.

I said that this was two lists, and up till now I've put the different lists
in different files.  But that would mean that your Next Actions are spread
across two files, which is perhaps less than ideal.  We can combine the
projects and next actions into one file:

* Next Actions
** TODO item 5
** TODO item 6
* TODO Project 1
** TODO item 1
** TODO item 2
* TODO Project 2
** TODO item 3
** TODO item 4

You could add your Someday list here too, but I actually like having it in a
different file.  My someday list can get long and rambling, and I interact
with it on a different cadence than my projects and next actions, so keeping
it distinct makes sense.

As an added bonus, all next actions in this scheme are under a heading,
which makes it easy to assign them all to a category, like this:

* Next Actions
 :PROPERTIES:
 :CATEGORY: next
 :END:
** TODO item 5
** TODO item 6
* TODO Project 1
 :PROPERTIES:
 :CATEGORY: proj2
 :END:
** TODO item 1
** TODO item 2
* TODO Project 2
 :PROPERTIES:
 :CATEGORY: proj2
 :END:
** TODO item 3
** TODO item 4

if you make your category roughly match your project, then they can act as a
suitable stand-in when viewing Next Actions on the agenda.  This means you
can easily see, at a glance which project your Next Action belongs to, or if
it stands on its own.
