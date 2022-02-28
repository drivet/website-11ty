---
date: 2015-01-16T06:43:35.000Z
modified: 2015-05-16T05:53:03.218Z
tags:
  - python
  - programming
title: Coming to Terms with the Nightmare that is Python Packaging
---

I started [YAWT][1] a while ago, in python, because a) I wanted to learn a
dynamically typed programming language and b) python seemed like a
relatively easy, fun, popular, not insensible choice.  To be fair, it *is* a
fun language to use.

At the time, I gave very little thought actually packaging yawt -
i.e. making it easy for someone to actually install and use.  Mostly, I
simply didn't think I'd ever have to do this - yawt was *my* baby, and I was
the only one using it, so why bother?

My opinion about this has changed somewhat in recent months.  I still don't
think yawt is likely to be used by anyone but myself, but I find myself
wanting to learn at least a little about how the python packaging system
works - if only for myself.  I mean, it should at least be easy for *me* to
install it, right?

I expected to spend a couple of days on this.  What I was not expecting was
to the absolute clusterfuck surrounding the python packaging ecosystem.
Even something as simple as installing a package has at least a couple of
caveats around it.

So, here's what I've learned.  Hopefully someone will find it useful, even
if that someone is only a future version of myself.

## Things are Actually Not that Bad

So, first off, I should mention that things are not so bad as I made out.
The packaging system is actually fairly straightforward - if you know what
to use and what to avoid.  But this is far from obvious, given that python's
packaging system has gone through some fairly major changes in recent years,
and is associated with more than its fair share of dead ends.

So, if you're uninterested in a history lesson, here's the short version:

* use [pip][4] to install packages.
* use [distutils][2] for describing very basic metadata about the package
  you're trying to distribute.
* use [setuptools][3] for a collection of enhancements to distutils, especially
  revolving around dependency management.

Details can be found at:

[https://packaging.python.org/en/latest/current.html][8]

That's basically it.  It's pretty simple but, of course, it took a
disturbingly long time for me to figure it out.  If you're interested in the
details, read on.

## Details, Dead Ends and Detours

In the beginning, there was distutils.  Distutils is more or less used to
describe the layout of the python bundle that you want to distribute -
where the source files are located, what python packages are actually in the
bundle, etc.

Notably absent is any notion of *dependencies* i.e. which other python
packages are required for your bundle to function.  This is where
setuptools comes in.  Among other things, setuptools lets you specify
package dependencies.

Importantly, setuptools comes with a tool called [easy_install][5] which,
when used to install a package, will automatically install that package's
dependencies as well, in much the same fashion as a standard Linux package
manager.

After that, things became more complicated.  Someone forked setuptools and
called the project [distribute][6] - though when you used it in python code,
the main package was still called setuptools, which was confusing.  I'm not
entirely clear why the fork occurred, and to be honest I don't really care,
but distribute started to be preferred over plain vanilla setuptools.

At some point during all of this, someone started working on
[distutils2][7].  This was an attempt combine the best parts of distutils,
setuptools, and distribute into one system that would be bundled with a
standard python installation.

Which is all pretty confusing on its own, but then, you know, *more stuff*
happened:

* distribute was folded into setuptools.  So, as of setuptools 0.7, you get
  the distribute features bundled along with it.  The distribute package
  itself has been deprecated.

* distutils2 was killed in favour of a distutils/setuptools combination.

* someone created an alternative to easy_install, called pip, which
  essentially did the same thing, but in a more robust fashion (it could
  uninstall packages and it wouldn't screw up your system if the
  installation failed midway). So people started preferring pip.

So bottom line?  We have the holy trinity: distutils, setuptools, and pip.
Ignore everything else.  Wheee!

[1]: https://github.com/drivet/yawt
[2]: https://docs.python.org/2/distutils/
[3]: https://pythonhosted.org/setuptools/
[4]: https://pip.pypa.io/en/latest/
[5]: https://pythonhosted.org/setuptools/easy_install.html
[6]: https://pypi.python.org/pypi/distribute/0.6.49
[7]: https://pythonhosted.org/Distutils2/
[8]: https://packaging.python.org/en/latest/current.html
