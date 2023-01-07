---
blurb: I cannot be the only one who thinks that git is awful
date: 2021-08-26T01:17:13.000Z
modified: 2021-08-26T01:17:13.000Z
title: Git is Terrible
mp-syndicate-to:
  - twitter_bridgy
tags:
  - programming
---

If I say that my professional life has spanned [RCS][1], [CVS][2],
[Mercurial][3] and [Git][4], this tell you something (depressing) about my
age.

Like everyone else, I'm now a git user, but I still say that Mercurial was
the most elegant of the bunch, at least in terms of its emmiently
predictable and well-designed CLI, even if the software itself gave up the
ghost years ago.

Back in the day, when version control systems were still duking it out, I
used to try and defend Mercurial to rabid git fans, mostly to no avail.  I
always felt that I was at a bit of a disadvantage in those conversation (cue
the git fans yelling "Yeah, cuz mercurial SUCKS!"), because I was always
willing to concede, despite appreciating Mercurial's virtues, that git
actually did have its strengths as well as its weaknesses, while the git
fans were rarely so magnanimous with any other VCS.

It has to be said: git is blazing fast, with a very flexible branching model
and large mindshare.  The branching model and the existence of things like
[GitHub][5] are probably, in fact, the principal strengths of git, and the
principal weaknesses of Mercurial.  It's a good thing, too, because as far as
the CLI is concerned, git is simply garbage.

## CLI Warts

The fact that there exists a site like the [git man page generator][6]
should tell you something about git's CLI.

You use ``git branch`` to list and create branches, but you use ``git
checkout`` to *switch* to a branch, unless it's a new branch, in which case
it can also be used to also create the branch if you supply it with the `-b`
option.

So far this isn't actually that bad.  The ``checkout`` command is your
moving around command - you can use it to switch to an arbitrary commit, for
example.  The branch command is your branch management command.  The `-b`
option to the checkout command does add a certain wart to the whole thing -
it would be more intuitive, in my opinion, if the branch command created
*and* switched you to the branch automatically - but so far it's nothing
horrible, even if the term "checkout" here seems a bit more general than one
might be used to.

Ah! But you *also* use ``git checkout`` when you want to *revert* a *file*,
i.e.  undo the local changes you just made to it.  I understand the
underlying conceptual link between the two use cases, but it's not the
normal way I usually think about these operations.  There is, in fact, a
``git revert`` command but, surprisingly, it doesn't actually work on files -
it's used to create commits which undo (revert) other commits.  Of course,
if you want to just revert all the local changes in your repo, you can do so
with the ``git reset`` command, which comes in hard and soft flavours, like
ice cream, and which also doesn't work on individual files, but which
provides a plethora of options, mostly revolving around whether your changes
are *staged* or not.

Oh yeah! Git also provides a staging area, which I've personally found of limited
value, especially when you consider how easy it is to amend commits.  But
your mileage may vary on that front.

And don't even get me started on ``git rebase``, the Swiss Army chainsaw of
history editing commands, which appears to be specially designed to mess up
your repo.

Don't misunderstand me.  I use git every day - sometimes even effectively! -
and I would never use Mercurial on a new project.  It's just that I
sometimes think that there are git fans who use it *because* of its
difficulties, rather than *in spite* of them - possibly the same kind of
people who enjoy competing in [Obfuscated C contests][7].  They are a
flavour of nerd to which I don't really relate.

[1]: https://en.wikipedia.org/wiki/Revision_Control_System
[2]: https://en.wikipedia.org/wiki/Concurrent_Versions_System
[3]: https://en.wikipedia.org/wiki/Mercurial
[4]: https://en.wikipedia.org/wiki/Git
[5]: https://github.com/
[6]: https://git-man-page-generator.lokaltog.net/
[7]: https://www.ioccc.org/
