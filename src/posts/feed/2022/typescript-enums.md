---
title: The Philosophy of TypeScript Enums
date: '2022-04-11T20:13:11-04:00'
tags:
  - typescript
  - programming
blurb: '#TypeScript enums are kind of weird'
---

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
transpiler like GWT means implementing basic constructs like Java-style
classes and specific implementations of things like ArrayLists and
HashMaps - all in JavaScript.

You don't have to pull these kinds of shenanigans with a TypeScript
transpiler because TypeScript isn't pretending to be anything other than a
somewhat safer version of JavaScript.  TypeScript doesn't provide a
specialized list implementation, for example, because JavaScript (especially
ES6) already gives you perfectly good arrays.  TypeScript's class construct
maps directly to ES6's class construct which is, itself, little more than
syntactic sugar over the somewhat more cumbersome ES5 class construct.

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

```typescript
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

This is what's called a *numeric enum*; despite the lack of any visible
numbers, the values of Sunday through Saturday are literally represented by
the numbers 0 through 6.

The DayOfWeek enum label is usually used as a type, like this:

```typescript
function doSomethingWithDay(day: DayOfWeek) {
   ...
}
```

This allows to call the function like this:

```typescript
doSomethingWithDay(DayOfWeek.Monday)
```

Writing code like this makes two things clear.  For one thing, while
DayOfWeek is an enum type, it's really little more than an alias for
`number`.  For example, you can call the function like this:

```typescript
doSomethingWithDay(10)
```

And the compiler won't complain.  Secondly, the values of an enum have a
real, tangible existence in the transiled JavaScript code, and you can
access them like this:

```typescript
const m = DayOfWeek.Monday;
```

In other words, an enum is more than just a type; it's an actual runtime
object that can be manipulated in actual code.  In the above example, the
type of `m` would be number (and value of `m` would be 1).

Enums also come in string variants like this:

``` typescript
enum D {
   Stuff = 'Blah',
   Foo = 'Hello'
}
```

This is used in basically the same as the numeric variant, except that in a
statement like this:


```typescript
const m = D.Stuff;
```

The type of `m` would be string (and the value of `m` would be 'Blah').

## Philosophical Discomfort

Okay, so what's the problem, exactly?  Why are TypeScript enums weird?  I
mean, it's kind of hard to imagine that JavaScript developers are unfamiliar
with the idea of a variable that can only hold one of a finite list of
values, so why does this blog entry even exist?

The objection is more philosophical than anything else, I suppose.  The
fundamental design strategy behind TypeScript is to re-use existing
JavaScript concepts wherever possible.  TypeScript Constructs like imports,
classes, Arrays, Maps, Sets will all be perfectly familiar to any ES6
developer - because they are exactly the same as their JavaScript
counterparts.

Enums, on the other hand, do not explicitly exist in any version of
JavaScript; they are pure TypeScript constructs.  In that sense they're a
bit like [interfaces][5], which also exist purely in the TypeScript realm,
the difference being that, unlike interfaces, which get completely compiled
away from the final, resulting JavaScript, enums very much stay behind.  And
that makes them, in my opinion, somewhat anathema to the entire idea of
TypeScript.  One of the main perks of TypeScript development is that it is
not entirely dissimilar to JavaScript development, and the use of TypeScript
enums undermines that advantage.

## Practical Considerations

Another problem is that enums are awkward for certain use cases that would
*seem* to be a natural fit.  Consider the case of a dictionary with a
limited set of strings for keys.  For example, imagine you had a JavaScript
object to represent a number of widgets for each day of the week:


```typescript
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


```typescript
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


```typescript
const totalForMonday = widgetsPerDay[DayOfWeek.Monday]
console.log(totalForMonday)
```

This will print undefined, because we're actually looking up the total with
a key value of 1, i.e. the value of DayOfWeek.Monday, and there is obviously
no value attached to that key in the widgetsPerDay object.  Making this work
requires looking up the name from the enum object itself, like this:


```typescript
const mondayKey = DayOfWeek[DayOfWeek.Monday] // mondayKey = 'Monday';
const totalForMonday = widgetsPerDay[mondayKey];
console.log(totalForMonday);
```

This prints the expected 10, but I don't believe I'm the only one to find
the need to lookup up the value of the key in the enum object somewhat
distasteful.

You can make all this work somewhat more naturally with a string enum:


```typescript
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


```typescript
const totalForMonday = widgetsPerDay[DayOfWeek.Monday]
console.log(totalForMonday)
```

will print out the expected 10.  This is, in fact, pretty close to how you'd
write this in a language like, say, Java.  You'd probably define static
string constants in a class somewhere like this:


```java
public class DayOfWeek {
    public static String Sunday = "Sunday";
    public static String Monday = "Monday";
    // ...and the rest
}
```

This is also probably pretty close to how you would do this in pure
JavaScript.  In ES6, we'd probably just define a file with the key labels
defined as such:

```javascript
export const Sunday = 'Sunday';
export const Monday = 'Monday';
// ...and the rest
```

But...it's all a bit verbose and redundant, isn't it?  You're repeating the
name of the day in the variable and its value.  One might ask why we're
bothering with this at all.  It's almost enough to make me want to skip the
enum construct entirely and just do this:

```javascript
const totalForMonday = widgetsPerDay['Monday'];
console.log(totalForMonday);
```

Easier, yeah?  And maybe even good enough for this particular case, where
the keys are very constrained and very well-known.  That being said, if the
values of the keys were to suddenly change, you'd be in trouble.  If the
widgetsPerDay object were to suddenly look like this, for example:

```javascript
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

```javascript
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

```typescript
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
via the enum.  The apparent redundancy, as it turns out, buys us a certain
amount of future-proofing.

## Alternatives to Enums

A lot of people associate the concept of "static typing" with Java or C++
and that is a shame because the static typing in those languages is pretty
toothless compared to TypeScript (or, really, any modern, statically typed
language).  So it may surprise you to discover that you can actually reap a
lot of the benefits of enums in a language like TypeScript without ever
typing the word "enum" - albeit, sometimes with the help of a halfway decent
IDE.

For example, let's revisit the widgetsPerDay object:

```typescript
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

On a very general level, widgetsPerDay is a dictionary where the keys are
strings and the values are integers.  This is readily represented as a type
in TypeScript:

``` typescript
type WidgetsPerDay = { [day: string]: number; } 
```

We can do better, though.  The type, as defined, allows any string as a key,
which isn't technically correct; we know that only certain strings are
allowed.  So we can restrict the keys to certain strings by making a type to
represent the keys:

``` typescript
type DaysOfWeek = 
    'Sunday' | 
    'Monday' | 
    'Tuesday' | 
    'Wednesday' | 
    'Thursday' | 
    'Friday' | 
    'Saturday'
```

DaysOfWeek isn't an enum.  It's still a string type, but of a special kind;
a string that can only take on very specific values.  We can use it to
define our WidgetsPerDay type:

``` typescript
type WidgetsPerDay = { [day: DaysOfWeek]: number; } 
```

You can then write code like this:

``` typescript
const widgetsOnTuesday = widgetsPerDay['Tuesday'];
```

and if you have a relatively modern IDE like IntelliJ or VS Code, you'll get
code completion when you start typing 'Tuesday' as the key.  Generally,
these editors are smart enough to know when they're dealing with a
TypeScript type and act accordingly.

In addition, if you try and do something silly like this:

``` typescript
const widgetsPerDay: WidgetsPerDay = {
    'Sunday': 4,
    'Monday': 10,
    'Tuesday': 3,
    'Wednesday': 6,
    'Thursday': 9,
    'Friday': 1,
    'Saturday': 5
}
const widgetsOnBlatherDay = widgetPerDay['Blather'];
```

the compiler will bark at you as well.

To me, at least, this feels a lot more organic than an artificial,
TypeScript-only enum construct.  The type statements will get compiled out
of the resulting JavaScript, and all that's left is code that probably looks
similar to code you would have written in JavaScript anyway.

TypeScript even lets us do clever things like defining a type from the
values in an array:

``` typescript
const daysOfWeek = [
    'Sunday',
    'Monday',
    'Tuesday',
    'Wednesday', 
    'Thursday',
    'Friday',
    'Saturday'] as const;
type DaysOfWeek = typeof daysOfWeek[number];
```

Of course, you still technically have a problem if you want to change the
values of the keys.  If you want the days of the week to look like this
instead, for example

``` typescript
type DaysOfWeek = 
    'Sun' | 
    'Mon' | 
    'Tue' | 
    'Wed' | 
    'Thu' | 
    'Fri' | 
    'Sat'
```

then you're still stuck with changing every instance of "Tuesday" to "Tue" -
*unless*, of course, you are using one of the aforementioned modern,
TypeScript-aware IDEs, which can do it for you.

## Conclusion

Part of the reason I like TypeScript is that, for the most part, it doesn't
add an extra layer on top of JavaScript, except for the typing.  So when I
see something like enums being added to the language as a completely new
*runtime* concept, I get suspicious.

Instead, you can create something that acts very much like an enum using
nothing but static typing tricks, and if you use a halfway decent IDE,
you'll get all of the benefits without any of the weird residual, runtime
artifacts.

I guess the moral of the story is that you should use an IDE? Especially if
you're using a statically typed language.  I have [written about][2] that
subject before, but enums, for me, just drives home the point.  I know a lot
of software developers are too young to remember the static versus dynamic
typing debates, but static typing really does seem to have the edge here.

For me, at least. Your mileage may vary.  I'm not one to tell you can't put
pineapple on your pizza :-)

[1]: https://www.typescriptlang.org/
[2]: /2018/01/18/static-typing
[3]: http://www.gwtproject.org/
[4]: https://www.typescriptlang.org/docs/handbook/enums.html
[5]: https://www.typescriptlang.org/docs/handbook/interfaces.html
[6]: https://en.wikipedia.org/wiki/ECMAScript#6th_Edition_%E2%80%93_ECMAScript_2015

