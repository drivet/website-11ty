---
date: 2008-07-06T23:00:01.000Z
modified: 2015-05-16T05:53:03.040Z
tags:
  - technical
  - perl
  - programming
title: Error Handling in Perl
---

Okay, this won't be the most exciting entry in the world. There are
several different ways of logging or recording errors and warnings in
perl, and I find I easily get confused. So this entry will serve as a
very, very basic reference for me. Other people may find it useful. Or
not. 

## Core Perl

### die LIST

Taken from the Perl 5.10.0 documentation:

> Outside an eval, prints the value of LIST to STDERR and exits with
> the current value of $!  (errno). If $! is 0 , exits with the
> value of ($?>> 8) (backtick `command` status). If ($?>> 8) is 0,
> exits with 255. Inside an eval(), the error message is stuffed
> into $@ and the eval is terminated with the undefined value. This
> makes die the way to raise an exception.

Similar to warn, if LIST is empty and the $@ variable is defined (say,
from a previous eval block), then die will print out
"$@\t...propagated".

If die is used without any arguments, and $@ is empty, then the string
"Died" is printed out.

die can be called with an object for more sophisticated exception
handling capabilities, but I won't go into that here.

### warn LIST

Without going into the details, warn takes a list and prints it to
STDERR. If LIST is empty and the $@ variable is defined (say, from a
previous eval block), then warn will print out "$@\t...caught".

If warn is used without any arguments, and $@ is empty, then the
string "Warning: Something's wrong" is printed out.

## Standard Carp Module

According to the documentation, this module gives four subroutines:

*carp* - warn of errors (from perspective of caller)

*cluck* - warn of errors with stack backtrace (not exported by default)

*croak* - die of errors (from perspective of caller)

*confess* - die of errors with stack backtrace

The idea is that these routines act like warn or die, but with a
message which is more likely to be useful to a user of your module.

carp and cluck are warn alternatives and croak and confess are die
alternatives. When used with strings, they apparently behave like warn
and die. cluck is more verbose than carp, and confess is more verbose
than croak.

There's alot more here, but that's the basic idea. Also, no one seems
to use cluck all that much.

## What to use?

So when do we use carp and when do we use warn? When do we use croak
and when do we use die? I found this response on a mailing list:

> If to Carp is wrong, I don't want to be right.

> But you don't use them because they're "a little better", but
> because they're "the right tool for the job". Using croak or carp
> reports the error itself in much the same way as die or warn
> would, but they point the finger of blame at your module's
> caller. That is to say, if my code calls a subroutine in your
> module, and your subroutine in turn calls croak, then the error
> message is going to identify the bug as having occurred in MY
> code, instead of yours. The line number and file name report where
> I called your code. (See the Carp docs for the full story.)
>
> So, if you're trying to say, "you've misused my module", then carp
> and croak are the way to do it.

> If, on the other hand, you're trying to say, "something unexpected
> happened, and my code wasn't prepared to deal with it", perhaps
> warn and die will be more appropriate. It all depends, in the end,
> upon how you can best assist the person who will be unfortunate
> enough to see the error message.

## CGI scripts

If you're writing a CGI script, warn/carp and die/croak will go to
your server's error log in an often cryptic manner.  If you use
CGI::Carp, then the standard warn, die, croak, confess and carp calls
will be replaced with equivalents which write out nicely time-stamped
messages to the HTTP server error log.

But even better, if you do this:

use CGI::Carp qw(fatalsToBrowser);

then any uncaught die or croak call will be displayed in your browser.

## Summary

So there you have it. Use croak when you want to die, but want to
communicate that it's the caller's fault.  Use carp when you want to
warn, but want to communicate that it's the caller's fault. Use
CGI::Carp, especially with fatalsToBrowser, whenever you write a CGI
script, and use warn, die, croak, and carp as you did before.
