title: Enum Musings in TypeScript
date: 2020-11-30 22:21:29 -0500
modified: 2020-11-30 22:21:29 -0500
tags: typescript
blurb: Typescript enums are kind of weird
mp_syndicate_to: twitter_bridgy

I'm a pretty big fan of [TypeScript][1], probably best described as a
statically typed language that transpiles to standard JavaScript.  I've
[written about it][2] before.

People who have worked with me in the past may be surprised at my
admiration, given the rancour I occasionally direct at something like the
[Google Web Toolkit (GWT)][3], a Java-to-JavaScript transpiler, which to the
casual eye seems to be in a similar vein.

The similarities, however, are superficial; TypeScript and GWT are quite
different beasts.  Something like GWT is used in web development in order to
*avoid* writing JavaScript.  TypeScript, on the other hand, is best viewed
as JavaScript extended with an optional static typing system.  The optional
part is key; in theory, any JavaScript program is also a valid TypeScript
program.  In other words, TypeScript doesn't try to hide the fact that it's
merely an enhanced version of JavaScript.

To put it another way, GWT adopts the attitude that JavaScript is
fundamentally broken and should be avoided in shame, while TypeScript adopts
the attitude that JavaScript is elegant and should be used with pride.

Granted, the latter attitude is much easier to adopt with the advent of
[EcmaScript 6 (ES6)][6], the official name of the latest version of
JavaScript.  ES6 fixes most of the shortcomings of previous versions of
JavaScript, many of which are the reasons that GWT became a thing in the
first place.

Because Java and JavaScript are so different, using GWT comes at the expense
of an "impedance mismatch".  Although JavaScript comes with arrays,
dictionaries and something resembling classes, these features are nothing
like the analogous features in Java.  Implementing a Java-to-JavaScript
transpiler like GWT means implementing basic constructs like Java classes
and specific implementations of things like ArrayLists and HashMaps - all in
JavaScript.

You don't have to pull these kinds of shenanigans with a TypeScript
transpiler because Typescript isn't pretending to be anything other than a
somewhat safer version of JavaScript.  TypeScript doesn't provide a
specialized list implementation, for example, because JavaScript (especially
ES6) already gives you perfectly good arrays.  TypeScript's class construct
maps directly to ES6's class construct which is, itself, mere syntactic
sugar over the somewhat more cumbersome ES5 class construct.

What I'm trying to say is that TypeScript really is just "JavaScript with
types" which is why my hackles go up every time I hear someone describe it
as an attempt to make JavaScript more "Java-like".  This is criminally
misleading, and misses the point entirely.

It also cuts right to the core of why I dislike TypeScript enums.

## Enums in TypeScript

TypeScript enums are similar in concept to enums from other languages, but
if all you've ever used is JavaScript, the idea might be foreign.  They
provide a convenient, type-safe way to describe a variable who's value can
only be taken from a constrained list - think of a variable to represent the
day of the week, or the month of the year.

You define an enum in TypeScript like this:

```
enum DayOfWeek {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
}
```

This is what's called a *numeric enum*; the values of Sunday through
Saturday are literally represented by the numbers 0 through 6.

The DayOfWeek enum label is usually used as a type, like this:

```
function doSomethingWithDay(day: DayOfWeek) {
   ...
}
```

This allows to call the function like this:

```
doSomethiWithDay(DayOfWeek.Monday)
```

Writing code like this makes two things clear.  For one thing, while
DayOfWeek is an enum type, it's really little more than an alias for
`number`.  For example, you can call the function like this:

```
doSomethingWithDay(10)
```

And the compiler won't complain.  Secondly, the values of an enum have a
real, tangible existence in the transiled JavaScript code, and you can
access them like this:

```
const m = DayOfWeek.Monday;
```

In other words, an enum is more than just a type; it's an actual runtime
object that can be manipulated in actual code.  In the above example, the
type of `m` would be number (and value of `m` would be 1).

Enums also come in string variants like this:

enum D {
   Stuff = 'Blah',
   Foo = 'Hello'
}

This is used in basically the same as the numeric variant, except that in a
statement like this:


```
const m = D.Stuff;
```

The type of `m` would be string (and the value of `m` would be 'Blah').

## Philosophical Discomfort

Okay, so what's the problem, exactly?  Why are TypeScript enums weird?  I
mean, it's kind of hard to imagine that JavaScript developers are unfamiliar
with the idea of a variable that can only hold one of a finite list of
values, so why does this blog entry even exist?

The objection is more philosophical than anything else, I suppose.  The
fundamental design strategy behind TypeScript is to re-use to JavaScript
concepts wherever possible.  Constructs like imports, classes, Arrays, Maps,
Sets will all be perfectly familiar to any ES6 developer - because they are
the exact same constructs.

Enums, on the other hand, do not explicitly exist in any version of
JavaScript; they are pure TypeScript constructs.  In that sense they're a
bit like [TypeScript interfaces][5], which also exist purely in the
TypeScript realm, the difference being that, unlike interfaces, which get
completely compiled away from the final, resulting JavaScript, enums very
much stay behind.  And that makes them, in my opinion, somewhat anathema to
the entire idea of TypeScript.  One of the main perks of TypeScript
development is that it is not entirely dissimilar to JavaScript development,
and the use of TypeScript enums undermines that advantage.

## Practical Considerations

Another problem is that enums are awkward for certain use cases that would
*seem* to be a natural fit.  Consider the case of a dictionary with a
limited set of strings for keys.  For example, imagine you had a JavaScript
object to represent a number of widgets for each day of the week:


```
const widgetsPerDay = {
    'Sunday': 4,
    'Monday': 10,
    'Tuesday': 3,
    'Wednesday': 6,
    'Thursday': 9,
    'Friday': 1,
    'Saturday': 5
}
```

You'd like to define a type to represent the keys of this object, and you
might think that a vanilla (numeric) enum would do the trick:


```
enum DayOfWeek {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
}
```

And you'd be right, but not without some fairly significant syntactic
acrobatics.  For one thing, the enum values are numbers, not strings, so
this doesn't work the way you might think:


```
const totalForMonday = widgetsPerDay[DayOfWeek.Monday]
console.log(totalForMonday)
```

This will print undefined, because we're actually looking up the total with
a key value of 1, i.e. the value of DayOfWeek.Monday, and there is obviously
no value attached to that key in the widgetsPerDay object.  Making this work
requires looking up the name from the enum object itself, like this:


```
const mondayKey = DayOfWeek[DayOfWeek.Monday] // mondayKey = 'Monday';
const totalForMonday = widgetsPerDay[mondayKey];
console.log(totalForMonday);
```

This prints the expected 10, but I don't believe I'm the only one to find
the need to lookup up the value of the key in the enum object somewhat
distasteful.

You can make all this work somewhat more naturally with a string enum:


```
enum DayOfWeek {
    Sunday = 'Sunday',
    Monday = 'Monday',
    Tuesday = 'Tuesday',
    Wednesday = 'Wednesday',
    Thursday = 'Thursday',
    Friday = 'Friday',
    Saturday = 'Saturday'
}
```

in which case this statement


```
const totalForMonday = widgetsPerDay[DayOfWeek.Monday]
console.log(totalForMonday)
```

will print out the expected 10.  This is, in fact, pretty close to how you
would solve this in a language like, say, Java.  You'd probably define
static string constants in a class somewhere like this:


```
public class DayOfWeek {
    public static String Sunday = "Sunday";
    public static String Monday = "Monday";
    // ...and the rest
}
```

This is also probably pretty close to how you would do this in pure
JavaScript.  In ES6, we'd probably just define a file with the key labels
defined as such:

```
export const Sunday = 'Sunday';
export const Monday = 'Monday';
// ...and the rest
```

But...it's all a bit verbose and redundant, isn't it?  You're repeating the
name of the day in the variable and its value.  One might ask why we're
bothering with this at all.  It's almost enough to make me want to skip the
enum construct entirely and just do this:


```
const totalForMonday = widgetsPerDay['Monday'];
console.log(totalForMonday);
```

Easier, yeah?  And maybe even good enough for this particular case, where
the keys are very constrained and very well-known.  That being said, if the
values of the keys were to suddenly change, you'd be in trouble.  If the
widgetsPerDay object were to suddenly look like this, for example:


```
const widgetsPerDay = {
    'Sun': 4,
    'Mon': 10,
    'Tue': 3,
    'Wed': 6,
    'Thu': 9,
    'Fri': 1,
    'Sat': 5
}
```

then this code will fail:


```
const totalForMonday = widgetsPerDay['Monday'];
console.log(totalForMonday);
```

You would have to go and change every instance of 'Monday' (that was being
used as a key to widgetsPerDay) to 'Mon' (and then do the same for all the
other days of the week).  This may or may not be a big deal depending on how
often you're using the key.  The desire to avoid this and to protect
yourself from such a possibility is what leads us to the apparent verbosity
of the other solutions.  If you were to use a string enum, all you would
have to do is redefine the values like this:


```
enum DayOfWeek {
    Sunday = 'Sun',
    Monday = 'Mon',
    Tuesday = 'Tue',
    Wednesday = 'Wed',
    Thursday = 'Thu',
    Friday = 'Fri',
    Saturday = 'Sat'
}
```

And nothing else needs to change, as long as the keys are always referenced
via the enum.  The apparent redundancy, it turns out, buys us a certain
amount of protection from change.

## Alternatives to Enums


[1]: https://www.typescriptlang.org/
[2]: /2018/01/18/static-typing
[3]: http://www.gwtproject.org/
[4]: https://www.typescriptlang.org/docs/handbook/enums.html
[5]: https://www.typescriptlang.org/docs/handbook/interfaces.html
[6]: https://en.wikipedia.org/wiki/ECMAScript#6th_Edition_%E2%80%93_ECMAScript_2015
