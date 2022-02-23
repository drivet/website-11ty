---
date: 2015-09-26T02:48:19.716Z
modified: 2015-09-26T03:03:13.315Z
tags:
  - technical
  - python
  - programming
title: Yet Another Requirements Versus Setup Post
---

In a [previous post][1] I wrote down some notes on the current state of the
python packaging ecosystem.  I felt that this was useful because most of the
information that I found online was out of date or spread out in various
places.

One area that still confuses me is the relative merits of requirements.txt
versus setup.py files.  Ironically, the
[best article I've read on the subject][2] is aimed at ruby developers.  I
think python developers should read it - it's surprising (or maybe not so
surprising) how similar the ecosystems are in this respect.  I've even made
a handy Rosetta stone, which is hopefully not completely inaccurate:

<table>
<tr><th>Ruby</th><th>Python</th></tr>
<tr><td>gem</td><td>no cute name here; python people just use "reusable component" or some such</td></tr>
<tr><td>Gemfile/Gemfile.lock</td><td>requirements.txt</td></tr>
<tr><td>.gemspec</td><td>setup.py</td></tr>
<tr><td>Rubygems</td><td>PyPI</td></tr>
<tr><td>Bundler</td><td>pip, when used with a requirements file</td></tr>
</table>

Someone wrote a [python version of the ruby article][3] but, honestly, with
no offence meant to the author, I found the original clearer.  And, since
writing things down tends to solidify things in my head, we have this
article.

I'll repeat what has been said many times before: setup.py is meant for
reusable python components and requirements.txt is meant for standalone,
deployable python applications.  With setup.py you generally only specify
your immediate dependencies, and you avoid specifying their exact versions;
usually, you specify a range of acceptable versions.  With requirements.txt,
on the other hand, you generally specify a flat list of every package
required to run the application (i.e. not just the immediate dependencies,
but their dependencies as well, recursively).  Furthermore, those packages
will generally have exact versions attached to them; the goal is to control
the exact environment into which your application will be deployed.

There's another reason to use requirements.txt files: you can specify
package sources.  For example, if you're hosting a local mirror of PyPI, the
place to specify this would be the requirements.txt file.  Although you can
perform similar shenanigans with setup.py, it's generally frowned upon;
setup.py is meant for specifying *abstract* dependencies, with no
information about the specifics of how to meet them.

## Reusable Components

So far, this is pretty simple.  Where it gets hazy is when you're actively
developing a reusable python component.  In this kind of situation, your
library is, in a sense, your application.  Sometimes, for example, your
library depends on another that hasn't yet made it to PyPI.  Or maybe it
has, but you need a more recent version that's only available from git.
And, of course, the component you're actually working on usually isn't
usually coming from PyPI - it's probably coming from a cloned git repository
sitting on your disk drive somewhere.  So what do you do?

What you do is you make yourself a requirements.txt file that looks like
this:

`requirements.txt`:

```
-e .
```

This tells pip to use the setup.py file located in the current directory,
and do an "editable" install (that's the -e option), so you don't have to
keep re-installing the package every time you make a change.  The whole
dependency tree will be installed as usual as if you ran `python setup.py
develop` (which is basically like a `python setup.py install`, but arranged
so you can easily edit the project).

So why use a requirements file instead of just running `python setup.py
develop` directly?  Because doing so lets you do something like this:

`requirements.txt`:

```
-e https://github.com/foo/bar.git#egg=bar
-e .
```

This is basically saying that you want to use the bar package from github in
preference to any bar package specified in your setup.py.  Remove the line
if you want to get the bar package from PyPI again.

Keep in mind that you're doing this, not to specify exact dependency
versions, but rather to give yourself the ability to use packages from other
sources, like git.  The setup.py file would not be an appropriate place for
this kind of information; it's supposed to contain only abstract
dependencies.  This usage of requirements files can still break your library
if someone releases an incompatible version of one of your dependencies but
it's received wisdom that finding out about this sooner rather than later is
a good thing.

Another good reason to use requirements files is to support separate
deployment environments.  For [yawt][8], for example, I have a plain
requirements.txt which pretty much does what's described here:

`requirements.txt`:

```
-e git+https://github.com/drivet/flask-whoosh.git#egg=flask-whoosh
-e .
```

And I have one for deployment - treating yawt as an app:

`requirements-deploy.txt`:

```
BeautifulSoup==3.2.1
Flask==0.10.1
Flask-Script==2.0.5
Jinja2==2.7.3
Markdown==2.5.2
MarkupSafe==0.23
PyYAML==3.11
Pygments==2.0.2
Werkzeug==0.9.6
Whoosh==2.6.0
argparse==1.2.1
blinker==1.4
facepy==1.0.6
itsdangerous==0.24
jsonpickle==0.8.0
linecache2==1.0.0
oauthlib==1.0.3
py-gfm==0.1.1
python-dateutil==2.4.1
python-frontmatter==0.2.1
pytz==2015.4
six==1.9.0
traceback2==1.4.0
tweepy==3.4.0
wsgiref==0.1.2
# these are only available on github for the time being
-e git+https://github.com/drivet/flask-whoosh.git#egg=flask-whoosh
-e git+https://github.com/drivet/yawt.git#egg=yawt
-e git+https://github.com/drivet/update-github-clone.git#egg=update-github-clone
```

But I have another requirements file for testing (used on [travis][9]) which
installs [nose][4], [mock][5], etc.:

`requirements-test.txt`:
```
-r requirements.txt
nose==1.3.4
mock==1.0.1
Flask-Testing==0.4.2
coverage==3.7.1
```

And yet another for development, which installs [rope][6], [pinocchio][7],
etc.:

`requirements-dev.txt`

```
-r requirements-test.txt
nosy==1.1.2
pinocchio==0.4.2
rope==0.10.2
ropemacs==0.7
ropemode==0.2
pylint==1.3.1
pep8==1.5.7
```

You'll note that my development requirements file just extends my testing
requirements file with some extra packages that are useful to me when I'm
coding.

And that's about it, really.  They key thing to take away is that
requirements files are basically used for two things:

 * deploying applications, in which case a flat file of exactly specified
   packages is provided.
 * specifying alternate sources for packages, in which case you mostly just
   delegate to your setup.py file.

Pretty simple once you take a deep breath and try to figure it out.


[1]: /2015/01/15/python-packaging
[2]: http://yehudakatz.com/2010/12/16/clarifying-the-roles-of-the-gemspec-and-gemfile/
[3]: https://caremad.io/2013/07/setup-vs-requirement/
[4]: https://nose.readthedocs.org/en/latest/
[5]: https://github.com/testing-cabal/mock
[6]: https://github.com/python-rope/rope
[7]: https://github.com/mkwiatkowski/pinocchio
[8]: https://github.com/drivet/yawt
[9]: https://travis-ci.org/
