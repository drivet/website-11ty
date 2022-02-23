---
date: 2018-01-19T01:46:00.000Z
modified: 2018-03-17T23:03:39.000Z
tags:
  - technical
  - programming
title: 'Static Typing, IDEs, Automated Testing: An Eternal Golden Braid'
---

I've been a programmer by trade ever since I graduated from University.
This is a fairly long time, as these things are measured.  I still consider
myself on a learning curve, but that's a separate conversation.

My career, such as it is, mostly sidestepped the whole static versus dynamic
typing debate that roiled in the early 2000's.  School, when it veered into
software territory, mostly consisted of C and Java, two statically typed
languages.  My professional life, until fairly recently, has been mostly in
Java (with a bit of C++ thrown in for good measure) and hence has almost
exclusively revolved around statically typed languages.

For those not in the know, a statically typed language is able to catch
certain kinds of errors at compile time that a dynamically typed language
cannot.  Consider the following example:

`StaticTyping.java`:

``` java
public class StaticTyping {
    public static void main(String[] args) {
        int a = 6;
        String b = "3";
        int c = a / b;
        System.out.println("This is c: " + c);
    }
}
```

This example won't compile, because you can't divide an integer by a String.
On the other hand, consider the equivalent example in Python:

`dynamic_typing.py`:

``` python
a = 6
b = "3"
print a / b
```

This will also fail, but only at runtime.  This may not seem like such a big
deal, but it becomes a bit more insidious when you modify the example
slightly:

`dynamic_typing_insidious.py`:

``` python
def func1():
    a = 6
    b = "3"
    print a / b

def func2():
    a = 6
    b = 3
    print a / b

func2()
```

This will run fine, despite the obvious error lurking in func1().  The error
would only ever come to light when you run func1().  This is what people
mean when they claim that static languages are safer.

(Proponents of dynamically typed languages will claim at this point that
automated testing helps to offset some of the risk here, and they are not
wrong.  See later in this article for a brief discussion of this).

One could say, without much exaggeration, that my current employer *loves*
static typing.  Even in the context of web applications, where one might
expect [JavaScript][1] to reign supreme, the preferred solution for a long
time was to use [GWT][2] whenever possible.  Perl, Python, and JavaScript
were used, but not to the same extent, and certainly not for anything that
could be considered an "application".  I taught myself Perl and Python at
home to give myself some balance, and I now consider myself a reasonably
seasoned Python programmer, but for me dynamic languages still have a bit of
a "hobby" feel to them.

Java is, after all, what I used every day for many years.  To say that my
work languages were all statically typed requires, therefore, some
qualification.  It is more accurate to say that they were statically typed
*in the style of Java*.

This is an important point because while there have always been other
statically typed languages with a much different feel to them, the industry,
until fairly recently, has mostly embraced Java and so this is the language
that is traditionally brought up in any static versus dynamic typing debate.
It is the *canonical* statically typed language, usually pitted against
canonical dynamically typed languages like JavaScript, Python and Ruby.

This muddies the waters somewhat, because it is well-known that Java's
relatively primitive typing system can lead to notoriously ceremonial code.
When people extol the virtues of a dynamically typed language like Python,
they are usually comparing it against a language like Java.  A language like
Python (or JavaScript) often shines in such a comparison, because it's much
less verbose, and a much easier language in which to get something up and
running in a relatively short amount of time.

## The TypeScript Experiment

There are, of course, many other statically-typed languages than Java, but
the only one with which I have any degree of familiarity would be
[TypeScript][3].  TypeScript is probably best described as [EcmaScript 6][4]
(i.e. modern JavaScript) with optional static typing added.

People are often skeptical of TypeScript when they first come across it
because a) it was developed and is currently maintained by Microsoft and b)
their only basis for comparison (I'm guessing) are other transpiled systems
like [CoffeeScript][5] and GWT, both of which have their issues.  Using
CoffeeScript, for example, requires learning an entirely new (dynamically
typed) language which consists of almost pure syntactic sugar over
[EcmaScript 5][6] - a lot of which is obviated with the introduction of
EcmaScript 6.  GWT, on the other hand, being Java, comes with its attendant
verbosity and ceremony, and makes it difficult to integrate 3rd party
JavaScript libraries.

TypeScript is different.  TypeScript is a superset of JavaScript; you can
write standard EcmaScript 5 code and it will be recognized as valid
TypeScript code.  You can also write more or less standard EcmaScript 6 code
and transpile it to standard EcmaScript 5 code (a la [Babel][6.5]).  Perhaps
most importantly, however, you can optionally add static typing to your code
and the transpiler will enforce it.

This last part is key.  The static typing in TypeScript is *optional* and
*inferred*; unlike Java, it's not an all-or-nothing proposition.  You don't
have to add a type to every variable to make use of it, and this makes it
fundamentally less verbose than Java.

I think that a language like TypeScript drastically changes the calculus of
the static versus dynamic debate.  When your only alternative to JavaScript
is something like Java, many people, understandably, prefer to stick with
JavaScript.  But when you have something like TypeScript available, where
the language is familiar and the typing is unobtrusive, the choice is much
harder.

## In Defence of IDEs

It's safe to say that I'm a fan of TypeScript.  Using it comes with very
little downside.  In theory, the upside, as with any statically typed
language, is that the transpiler would be able to catch certain kinds of
bugs up front that, for a dynamically typed language, would only ever be
caught at runtime.

Honestly, though?  In my experience, the bug-catching aspects of statically
typed languages are overblown.  To be sure, TypeScript has caught a handful
of bugs in my own code, but I wouldn't say that it's caught enough for this
to be the sole reason to use it.  Although there is very little downside to
using TypeScript, it's not *zero* - you still need to download and set up
the compiler, the code you write is no longer directly usable is a browser,
and you have to use a new [linting tool][7].

So why use Typescript? For me, there are two main reasons:

1. Type annotations are an excellent form of documentation
1. IDEs suddenly become much more useful

The first is perhaps less controversial than the second.  It's useful to
know, by looking at the code, if a variable is supposed to be a number or a
string, or a collection thereof.  It's *especially* useful in conjunction
with libraries like [lodash][8] and [rxjs][9], which often transform
collections and Observables from one form to another in ways that are not
entirely obvious at first glance.  I'll often find myself adding an explicit
type declaration to an intermediate variable even when type inference would
normally figure it out, just for the clarity it provides.  Of course, you
can do the same thing with a comment, but the type declaration has the added
advantage of being compiler enforceable while remaining just as readable.

I say that the second point is controversial because I know that there are a
large number of dynamic language advocates who take pride in their habit of
writing code without an Integrated Development Environment (IDE).  Java
programmers almost never do that since writing Java without an IDE is
torture.  Often this leads people to conclude that an IDE is merely a crutch
for covering up flaws in your language of choice; there would be no need for
an IDE, so the argument goes, if Java were less verbose.

While I see the point, I think it's a bit simplistic.  It's true that part
of the reason one uses an IDE in conjunction with Java is to facilitate the
cranking out of code constructs that are tedious to write by hand -
constructs which are unnecessary in other languages. But it is *also* true
that an IDE is useful for much more:
 
 * automatic import management
 * jump to function definition
 * auto completion
 * refactoring tools, like method and variable renaming
 * on-the-fly detection of compile errors and warnings.

You can, of course, learn to live without any of this, but why would you
want to?  All of the above is staggeringly useful, especially as the size of
your project grows.

More to the point, all of the above is greatly facilitated by the use of a
statically typed language, because it's possible to deduce more about your
code up front than is possible with a dynamically typed one.

## On the Use of Test Suites

While it is, of course, silly and wrong to suggest that a strong focus on
automated testing and the use of a statically typed language are mutually
exclusive, the practice does tend to be associated more with dynamically
typed languages.  The received wisdom is that the test coverage makes up for
the lack of compile time error checking; in a sense, your test suite
fulfills the role of a statically typed compiler, catching basic issues in
advance.  When a dynamic language advocate is confronted with the assertion
that a static language catches certain kinds bugs that a dynamic language
cannot, they will often just wave their hand, shout "unit tests!", and move
on.

Alright, I'm exaggerating. But only slightly.

I understand the sentiment, but I feel it rests on an incorrect assumption.
Many people think that the main purpose of an automated testing suite is to
catch bugs.  While this is certainly a useful side effect, I find that a
test suite's main use lies in its role as *documentation*.

A test suite, at its best, documents developer expectations, and when your
code base gets to be a certain size, this is invaluable.  Debugging in
unfamiliar code is often a two-pronged challenge because not only are you
trying to find a defect, you're often, at the same time, trying to figure
out *what the right answer is supposed to look like*.  You are, in other
words, trying to figure out what the developer *meant to do*.  An automated
test suite, when written the right way, can help immensely with this,
wherever you fall on the dynamic/static divide.

## Conclusions

Ultimately, I guess what I'm trying to say is the following:

* Static typing is a win, as long as it's the right kind of static typing.
  Otherwise all bets are off.

* Automated test suites are a win, whether you use a static language or
  dynamic language, as long as they are written primarily for the purpose of
  documenting developer expectations.

* IDEs are a win, just because they're so darn useful.  And static typing
  makes IDEs work better.


[1]: https://en.wikipedia.org/wiki/JavaScript
[2]: http://www.gwtproject.org/
[3]: https://www.typescriptlang.org/
[4]: https://en.wikipedia.org/wiki/ECMAScript#6th_Edition_-_ECMAScript_2015
[5]: http://coffeescript.org/
[6]: https://en.wikipedia.org/wiki/ECMAScript#5th_Edition
[6.5]: https://babeljs.io/
[7]: https://palantir.github.io/tslint/
[8]: https://lodash.com/
[9]: http://reactivex.io/rxjs/
