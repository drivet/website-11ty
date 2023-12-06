---
date: 2023-12-05T21:42:19-05:00
title: "Implementing GTD in Emacs With Org Mode"
blurb: "There are lots of ways to do it"
post-status: draft
mp-syndicate-to:
- mastodon
tags: 
- emacs
---

I have an on-again, off-again relationship with productivity systems.  I
usually don't bother with them but on those infrequent occasions when my
life gets a bit hectic, I will sometimes make use of one.  When that happens
I always fall back to David Allen's [Getting Things Done (GTD)][1].  I read
his book a long time ago and, at the time, found myself impressed.

The fundamental feature of the system is that it downplays the classic,
detailed, prioritized to-do list in favour of the so-called *next action*
list.  When considering a particular goal, instead of allowing yourself to
become overwhelmed with all the myriad things you know you need to do in
service of that goal, you instead focus only on the *very next thing* you
can do to move closer to your goal.  The end result is an unsorted list of
things you can do "in the moment", across all the various goals in your life
at the moment.

The result can be *also* be, as you might imagine, a very long list of next
actions.  To manage this, Allen recommends the use of *contexts* - keywords
that describe when or where it's possible to do a next action.  The idea is
that when you find yourself in one of these contexts, you're supposed to be
able to pull up all the next actions attached to it and pick one off the
list.

In the 90's, before cellphones were widespread, a classic context would have
been "phone", to which you'd attach any next action involving a phone call.
When you found yourself near a phone, you'd pull up all your phone-related
next actions and start cranking through them one by one.  Another classic
one would have been the "computer" context, back in the days when a
"computer" meant a machine sitting on a desk somewhere.  You'd often attach
that one to a next action involving an email, for example,

These days everyone has a smartphone, so the "phone" and "computer" contexts
are less important than they used to be (though I personally still use the
computer context for writing blog entries, since I hate doing that kind of
thing on my phone).  That doesn't mean contexts are useless as a concept,
but it does mean that you need to think a bit more about how to use them.
The key thing is to remember is that they're meant to be *opportunistic*.
You're meant to be able to pull up a particular context once you find
yourself in the corresponding situation and immediately be presented with a
series of next actions that you can do "in the moment".  My contexts these
days are often people or place oriented.  If I have a Tupperware or a book
that I need to return to my Mom, for example, I'll attach the "mom" context
to those next actions and remember to consult it before visiting her
(spoiler: I often don't remember to consult it).

In addition to a next action list, GTD also has a notion of a *project*
list.  The word "project" can mean a lot of things but when we talk of GTD,
it means nothing more than a goal that requires more than one "next action"
to accomplish.  The metaphor that often gets used in the GTD community is
that a project is a "stake in the ground", a reminder that there is a
specific, somewhat abstract goal in your life that needs addressing.
Effectively, your project list defines the "hard landscape" of your life,
around which your next actions revolve.

GTD defines two more lists:

 * Waiting
 * Someday/Maybe

The Waiting list is for things that are "outsourced or "delegated" - things
that you can't or won't do yourself.  Waiting for someone to respond to an
email, for example, is a classic Waiting list item.

The Someday/Maybe list is meant to provide a haven for any vague and hazy
to-do item that you don't really want to work on "right now".  These could
be aspirational ("Learn photography") or just stuff that you might want to
get to later on in life ("Replace ceiling fan").

## Implementing GTD in Emacs with Org Mode

As you might imagine, there are lots of ways to implement GTD, both analog
and digital.  For a while, in the early 2000's, it was fashionable in
certain circles to implement it with a notebook that you carried around with
you.  Most people, nowadays, use some kind of software.

I'm an [Emacs][2] user, or at least I try to be, and so it makes sense for
me to try and implement GTD with Emacs.  The package one usually hears about
in this regard is [Org][3], a note keeping and text outlining tool that has
evolved over the years to include task management and scheduling
functionality.

Both Emacs and Org are huge and I'm not about to give a full-fledged
tutorial on either of them (I'm a perpetual novice myself) so instead I'll
focus on what I want out of my GTD system and how I can get Org to do it.

Roughly speaking, I want my GTD system to do the following:

 * Add new items easily, probably via a templating system
 * Separately view, at a glance, my next actions, waiting, projects, and
   someday items.
 * Attach a next action to a context and be able to view each of my contexts
   separately, with the next actions attached to it.
 * Be able to easily move items from one list to another (for example, turn
   a Someday item to a Project or vice versa)
 * Be able to archive "done" items easily, so they don't clog up my lists.

Adding new items is easily accomplished with [org-capture][4].  Viewing my
next actions, waiting, projects and someday lists is easily accomplished
with [org-agenda][5].  Contexts, and viewing the next actions attached to
them, are easily implemented with Org tags and the [narrowing function][6]
functionality of org-agenda.

Moving and archiving items are similar tasks, and Org has built in
functionality to do them.

So, with all that in mind, let's consider different approaches to
implementing GTD in Org.

## The Literal Approach

TL;DR: I don't actually use this method, because I think the cons outweigh
the pros, but I find it useful nonetheless to ferret out what works and what
doesn't.

One very obvious way to implement GTD is to maintain a separate file for
each list, in roughly the same manner than one would implement with paper.
Your Next Actions, for example, could live in an ```actions.org``` file that
looks like this:

```text
* TODO do this thing
* TODO do this other thing
```


And a ```projects.org`` file that looks pretty much the same:

```text
* TODO project 1
* TODO project 2
```

The waiting and someday lists would look almost exactly the same, in their
own respective files.

GTD contexts can be represented with Org tags, tacked onto the end of the
item, like so:

```text
* TODO do this thing :mom:
* TODO do this other thing :plumber:
```

Org has built in ways of filters the agenda view based on the tags you add
to each item.

This approach has one huge, underrated advantage: it's dead simple to
implement, explain and understand.  The fact that all items have the same
TODO state and are all at the same outlining "level" means that adding new
items, moving items one list to another, and archiving items are all dead
simple as well.

What about viewing your individual lists?  It turns out this is pretty
simple as well.  Org TODO items have a property called a *category*.  If you
don't do anything special, the category is just the file from which the item
originates.  So any item in the ```actions.org``` file has an *action*
category and the same goes for items in your other files.  Defining an
agenda view based on a category is trivial in Org, so it's very
straightforward to create separate views for your next actions, projects,
and someday items.

## Linking Projects and Next Actions

Are there any disadvantages to this simple approach?  There is only one that
I can think of: there's no explicit relationship between Next Actions and
Projects.  In other words, you can't tell at a glance what the next actions
are for a project, or which project a next action belongs to.  And here is
where things start to get murky, because a GTD purist will probably tell you
that it doesn't matter.

One thing that I find interesting about GTD is that the connection between
your next actions and your projects is a lot looser than it might seem at
first glance.  Yes, a next action can be, and often is, associated with a
project - but it doesn't have to be, and it's perfectly okay to have
"freestanding" next actions that aren't attached to anything at all.  In the
same vein, a project will often have at least one next action linked to it -
but it might not, if you haven't figured out what that next action is.
Allen even has a name for projects that have no next actions - they're
called "stuck projects" and it's rather revealing that it seemingly happens
often enough that he labelled the concept.

The GTD community sometimes takes this disconnect even further - they often
suggest *no* explicit connection between next actions and projects at all.
Projects, in this formulation, simply act as a reminder that a next action
might have to be created, but once that next action is in your system you
don't (so the theory goes) actually need to know which project spawned it.

It helps to remember, at this point, that GTD was conceived in the 90's as a
paper based task management system.  Next actions were often slips of paper,
or literal items penned in a list.  In a system like that, you have to
choose where to park your next action - and it can only go in *one* place.
The common wisdom at the time dictated that you should park your next action
on the context list where it would be the most useful and that meant that it
was *not* parked under the project with which it was associated.

A paper based GTD system almost *has* to work this way and so I think, over
time, that the GTD purists ended up making a virtue of necessity and
convincing themselves that it really wasn't very useful to know the project
associated with a next action, or to know all the next actions attached to a
project.

I'm not convinced.  I think, all things being equal, it's obviously useful
to know which project a next action belongs to.  And the thing is,
software today often makes it *trivial* to park a next action on different
lists - in both a project and a context for example.  You don't have to
choose one over the other.

So how can we do this with the very, very simple system I outlined above?
It's surprisingly difficult, though one can, of course, come up with scheme.
For example, Org has a feature called [drawers][7] that let you add
arbitrary properties to TODO items.  Add a Project ID to each project item,
and a Project ID to each Next Action that belongs to a project and you have
a theoretical way of connecting the two lists.  Roughly speaking, it would
look like this:

``` text
* TODO Project 1
:PROPERTIES:
:PROJECT_ID: proj1
:END:
```

Unfortunately, theoretical is not the same as practical.  Ideally we'd like
to see a Next Action's project in the agenda view but this isn't possible
here without some special configuration - which is of course entirely
possible but annoying.  More problematic is the reverse - seeing all Next
Actions associated with a project.  There's no view (that I know of) built
into Org that'll let you do that out of the box.  You'd have to design a new
view and write the code to produce it - not the easiest of tasks.

And if you ever want to archive an entire project, or move it to the Someday
list, it would make sense to move all the associated Next Actions along with
it.  In the scheme designed above, Org certainly wouldn't do this out of the
box, and honestly I'm having trouble imagining what that feature would even
look like.

Let's see what else we can come up with.

## Projects as Hierarchy

Another, arguably more natural, approach, involves putting all of a
project's Next Actions under the project's heading, like this:

```text
* TODO Project 1
** TODO item 1
** TODO item 2
* TODO Project 2
** TODO item 3
** TODO item 4
```

This seems entirely natural. Org *is* an outlining tool, after all.
Hierarchy is its bread and butter.

We can also maintain a separate list for those next actions that are not
connected to any particular project, and it would furthermore make sense to
combine everything into one file, so all your next actions and projects are
located in just one place:

```text
* TODO item 5
* TODO item 6
* TODO Project 1
** TODO item 1
** TODO item 2
* TODO Project 2
** TODO item 3
** TODO item 4
```

What do we do with the waiting items?  These could remain in their own
```waiting.org``` file, but it's entirely possible for a particular waiting
item to be associated with a project (or not - they're like next actions
that way).  We could nestle the waiting items among the next actions in our
file but that leads to a problem: how do we distinguish next actions from
waiting items?

The answer here is to add a new TODO state, called WAIT, something Org
allows you to do:

```text
* TODO item 5
* TODO item 6
* TODO Project 1
** TODO item 1
** TODO item 2
** WAIT another item
* TODO Project 2
** TODO item 3
** WAIT email response
** TODO item 4
```

To make things even more explicit, you could use a special TODO state for
your projects as well:

```text
* TODO item 5
* TODO item 6
* PROJ Project 1
** TODO item 1
** TODO item 2
** WAIT another item
* PROJ Project 2
** TODO item 3
** WAIT email response
** TODO item 4
```

One nifty feature of Org is that it allows you to override an heading's
category using the drawer feature, meaning you can give projects their own
category, like this:

```text
* TODO item 5
* TODO item 6
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
```

*Another* nifty feature of Org is that categories are inherited, so that
TODO items under a project will inherit that projects category.  All this to
say, if you make your project's category name roughly match your project's
name, then the category can act as a suitable stand-in when viewing Next
Actions on the agenda (since categories are shown by default on the agenda
view).  This means you can easily see, at a glance, which project your Next
Action belongs to, or if it stands on its own.

Seeing all the next actions associated with a project doesn't involve using
org-agenda - you just have to look at the project section itself, and
examine the next actions under the project heading.

You could integrate your someday list into this file, but I actually like
having it in a separate file, since it can get long and rambling, and I
usually interact with it on a different cadence than my main list.  Keeping
it in its own file makes sense.

Moving top level items between the main file and the someday file is trivial
with [org-refile][8].  Archiving is also trivial, as long as one sticks to
the top level items, i.e. as long as one doesn't try to archive unfinished
projects.

## The Implementation

Here is my complete org configuration:

``` emacs-lisp
(setq org-todo-keywords
      '((sequence "TODO(t)" "WAIT(w)" "PROJ(p)" "|" "DONE(d)" "CANC(c)")
        ))

(setq org-agenda-span 90)

(setq org-agenda-files (file-expand-wildcards "~/pim/gtd/main.org"))

(setq org-agenda-custom-commands
      '(
        ("i" "Inbox" tags-todo "+TODO=\"TODO\""
         ((org-agenda-files (file-expand-wildcards "~/pim/gtd/inbox.org"))))
        ("n" "Next actions" tags-todo "+TODO=\"TODO\"")
        ("p" "Projects" tags-todo "+TODO=\"PROJ\"")
        ("w" "Waiting" tags-todo "+TODO=\"WAIT\"")
        ("s" "Someday" tags-todo "+TODO=\"TODO\"|TODO=\"PROJ\""
         ((org-agenda-files (file-expand-wildcards "~/pim/gtd/someday.org"))))
        ("o" "Actions and Projects" tags-todo "+TODO=\"TODO\"|TODO=\"PROJ\"")
        ))

(setq org-agenda-prefix-format '((agenda . "  %-25:c%?-12t% s")
				 (timeline . "  % s")
				 (todo . "  %-12:c")
				 (tags . "  %-25:c")
				 (search . "  %-12:c")))

(setq org-agenda-tags-column -120)
(setq org-tags-column -80)

(setq org-agenda-sorting-strategy
      '((agenda habit-down time-up priority-down category-keep)
        (todo priority-down todo-state-up category-keep)
        (tags priority-down todo-state-up category-keep)
        (search category-keep)))

;; M-x org-agenda # to show the stuck projects
(setq org-stuck-projects
      '("+TODO=\"PROJ\"" ("TODO") nil "") )

(setq org-refile-use-outline-path 'file)
(setq org-outline-path-complete-in-steps 'nil)
(setq refile-targets (file-expand-wildcards "~/pim/gtd/*.org"))
(setq org-refile-targets '(( refile-targets :todo . "PROJ" )))

(setq org-capture-templates
      '(
        ("i" "Inbox" entry
         (file "~/pim/gtd/inbox.org")
         "* TODO %^{Brief Description}\nAdded: %U\n%?" :empty-lines 1 :prepend t)
          
        ("n" "Next action" entry
         (file "~/pim/gtd/main.org")
         "** TODO %^{Brief Description}\nAdded: %U\n%?" :empty-lines 1 :prepend t)

        ("w" "Waiting" entry
         (file "~/pim/gtd/main.org")
         "** WAIT %^{Brief Description}\nAdded: %U\n%?" :empty-lines 1 :prepend t)

        ("p" "Project" entry
         (file "~/pim/gtd/main.org")
         "* PROJ %^{Brief Description}\n:PROPERTIES:\n:CATEGORY: %^{Id}\n:END:\nAdded: %U\n%?" :empty-lines 1 :prepend t)
        
         ("s" "Someday" entry
         (file "~/pim/gtd/someday.org")
         "* TODO %^{Brief Description}\nAdded: %U\n%?" :empty-lines 1 :prepend t)
        ))

(define-key global-map "\C-cc" 'org-capture)
```

[1]: https://en.wikipedia.org/wiki/Getting_Things_Done
[2]: https://www.gnu.org/software/emacs/
[3]: https://orgmode.org/
[4]: https://orgmode.org/manual/Capture.html
[5]: https://orgmode.org/manual/Agenda-Views.html
[6]: https://orgmode.org/manual/Filtering_002flimiting-agenda-items.html
[7]: https://orgmode.org/manual/Drawers.html
[8]: https://orgmode.org/manual/Refile-and-Copy.html
