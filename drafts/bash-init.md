---
date: 2019-07-23 07:53:05-04:00
modified: 2019-07-23 07:53:05-04:00
status: draft
title: Notes on Bash Startup Scripts
---

Bash startup scripts are confusing, so here are some notes.

`.bashrc` is run when we start an interactive shell which is not a login
shell. So if you log in via ssh or a login terminal, this script *isn't*
run,but if you launch a terminal window from your favorte desktop
environment, it is (unless you've congured that terminal window to run as a
login shell).

So, it's important to make your .bashrc lightweight, since it's run every
time you start a shell.  Any aliases you have need to go in there as well,
because these need to be set for every shell.

But if you have lines setting up environment variables like this:

`export PATH=~/.local/bin/:$PATH`

then it's important *not* to run these in your .bashrc; your PATH variable
will grow every time you launch a terminal.

`.bash_profile` is run when we start a login shell, i.e. when we log in via
ssh or the login terminal.  If `.bash_profile` isn't there, bash will use
`.profile`, for legacy reasons.  This file would be the place to put
variable definitions, especially ones that add to PATH, for example,

When you log in from an X session, say from Cinnamon or Gnome, there is no
particular standard, but most of the time, `.profile` will be sourced, and
not `.bash_profile`.

## Bottom Line

Aliases and prompt setup go into `.bashrc` - anything that needs to be set
on a per terminal window basis.

I tend to just use `.profile` and be done with it.  It will work with an X
login and from an ssh session as well.

In any case, you should source your `.bashrc` file from this file, so you
get all your aliases, etc when you login.
