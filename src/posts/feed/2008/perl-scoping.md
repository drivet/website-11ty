---
date: 2008-02-13 16:31:46-05:00
modified: 2015-05-16 01:53:03.401994-04:00
tags:
- technical
- perl
- programming
title: Scoping in Perl
---

I just read a very clear tutorial on Perl scoping mechanisms: 

[http://perl.plover.com/FAQs/Namespaces.html][1]

I often get bitten by scoping issues but unfortunately (or fortunately?)
it's usually easier in Perl to work around your problem than to understand
the reasons behind it.

One part I found interesting was the section labelled "The Current Package"
which explains the use of the package command in Perl.  There is a tendency
(or, at least, *I* have a tendency) to conflate the use of the Perl package
command and, say, the use of the Java package declaration because they are
often used to achieve the same effect, namely the separation of namespaces,
and also because, *by convention*, they are used in a syntactically similar
way.  Because of this, it is easy to forget that the package command in Perl
is just that: a command. You can call it from anywhere, multiple time from
the same file even, and the effect is simply to change the current package -
which in itself doesn't have a huge effect on how you write your code, just
the way to access your variables.

It's easy to forget that any syntactic or semantic similarity Perl has with,
say, Java, C or C++ is almost purely convention.  I haven't decided if this
is good or bad.

[1]: http://perl.plover.com/FAQs/Namespaces.html
