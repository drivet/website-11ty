---
date: 2018-06-10 14:10:24-04:00
modified: 2018-06-10 14:10:24-04:00
tags:
- theremin
title: Why Is There No E Sharp?
---

In an attempt to at least *try* and get to know my theremin a bit better, I
caved and bought Carolina Eyck's [*The Art of Playing the Theremin*][1].  I
mean, her [instructional videos][4] on YouTube are great, but they don't
really give you a proper sense of how to move your fingers when playing a
tune.  Her book, on the other hand, does.

It's probably obvious to everyone else in the world, and I'm not sure what
exactly I was expecting, but it turns out that you have to know how to read
sheet music in order to fully benefit from the book.  Crazy, right?

Unfortunately, to put it bluntly, I know pretty much nothing about reading
sheet music - or music in general, really.  Given that I really want to
learn how to play my theremin, however, I decided to give it a go.  What
follows are some of my first impressions while learning the basics of this
entirely new (to me) domain of knowledge, from the perspective of a 40 year
old software developer starting almost entirely from zero.

## Do Re Mi

Okay, so I'm not starting *exactly* from zero.  Like most people, I was at
least partially familiar with the [Do-Re-Mi][5] song from [The Sound of
Music][6].  I understood, more or less, that the syllables were sounding out
the notes in an octave and, furthermore, that octaves "repeated" - a pitch
from one octave and the corresponding pitch from a higher octave were
perceived, in some sense, to be the same note, in what has been referred to
as "the basic miracle of music".

I even understood, in a limited way, that you could sound out the Do-Re-Mi
musical pattern on a piano by hitting a certain contiguous sequence of white
keys.  And I knew that the sounds and the keys were associated with letters
(the Do sound was a C, I knew, and the Re sound was a D).

But I didn't really give the topic much thought beyond that.  The term
"major scale" had not yet entered my brain.  I was under the hazy impression
that you could basically play any tune you wanted with only these 7 notes,
repeated indefinitely as octaves are.

To be fair, I did have a vague notion that there existed these mysterious
notes called "sharps" and "flats", and I had the sense that they were
somehow related to the black keys on a piano, but I had no idea what role
they played in a Do-Re-Mi world, so I basically just ignored them.

It's weird, I know.  I mean, what did I think the black keys were used for?
Jazz?

## Major Scales and Colour Wheels

Read enough on the topic and you eventually discover that Do-Re-Mi is an
example of something called a _major scale_ - the [C major scale][2] to be
precise.  One line stands out from the Wikipedia article on [major
scales][7]:

> The simplest major scale to write is C major, the only major scale not
> requiring sharps or flats

This tells me a couple of things:

* There are other major scales aside from C major, which means that there's
  more to music than Do-Re-Mi.  Who knew?
* The sharps and flats become important when you start constructing other
  major scales.

What's not immediately clear is _why_.  Why are sharps and flats required
for other major scales?  How exactly does one go about making a major scale?

When faced with these kinds of inquiries, you very soon come across
something called the [chromatic scale][8].  It's often depicted as a circle
like this:

<div style="clear: both; text-align: center;"> 
<img border="0"
     src="/static/img/Pitch_class_space.svg" 
     alt="Chromatic scale" />
</div>

Chromatic means "colour" in Greek, and I'm not entirely sure why it's used
in this context.  Is it supposed to evoke the image of a colour palette?
Are notes considered "colourful" to certain people?  I don't know.

It's a bit of a simplification, but the chromatic scale basically contains
all the notes that one ever hears in Western classical music.  I find it
amazing and disconcerting that the whole of Western music is built from a
repertoire of only 12 notes, but there you have it.

All other scales are necessarily a subset of the chromatic scale.  In
particular, with a _major scale_, one selects 7 notes from the chromatic
scale according to the following frequency pattern:

whole, whole, half, whole, whole, whole, half

A "half" in this context means one "hour" of the chromatic circle, so a
"whole" means two "hours".  A major scale is obtained by picking any note on
the circle and counting out the interval pattern above until you get to the
same spot again, thus obtaining an octave.  Since you can start from any
note, there are 12 major scales.

In particular, if you start from C, and you follow the pattern, you get C,
D, E, F, G, A, B, and C again.  You've just written out the C major scale,
the simplest one because it skips all the sharp notes in between and thus
can be played using only the white keys on a piano.  As the above quote
mentions, it's the only major scale with this feature; all the other major
scales have at least one sharp in them.  [D major][9], for example, consists
of the notes D, E, F#, G, A, B, C# and D again.

## Why is There No E Sharp?

As someone mulling these things over for the first time in his life, I'm
immediately struck by the weirdness in chromatic labeling.  Why is there a
note (C#) in between C and D, that feels like it was added as an
afterthought?  Conversely, why is there no note between E and F?  Or to put
it another way, why is C# not simply called a D?  It's the next letter,
after all.  Or, if you insist on having sharps in between the notes, then
why is F not called an E#?

All things considered, why not just label the notes 1 through 12, or A
through L?

I mean, it's all very well and good to say that the C major scale skips all
the sharp notes, but *that's* only true because someone decided, seemingly
arbitrarily, that a major scale followed a certain interval pattern (W, W,
H, W, W, W, H) *and*, furthermore, decided that there was no sharp in
between E and F.

What's so special about a C major scale that we will seemingly bend over
backwards in order to be able to label its notes with simple, unadorned
letters - to the point where every *other* note in the chromatic scale is
labeled as a kind of "correction" to these notes?  I honestly don't know.  I
get the impression that asking these kinds of questions is akin to asking
why a yard has 36 inches.  There's an answer, of course, but it's not
particularly germane to the task at hand, which is to be able to play a
tune.

## Idle Speculation

Of course, I can make what I hope are educated guesses.

If you look at the interval pattern for a major scale (W, W, H, W, W, W, H)
you can see that it can be broken down into two patterns of intervals (W, W,
H) separated by a whole interval.

If you consider just C major, for example, you get the first group of notes
C, D, E, F and the second group of notes G, A, B, C, separated by a whole
interval.  For each group of four notes, the ratio of the heighest note to
the lowest note is about 4:3.

I suppose that the ancient Greeks simply thought that these two notes
sounded nice together in that ratio.  If you consider that the
quintessential [lyre][3] has 4 strings, you can imagine an ancient musician
making sure the first and last strings on his instrument contained those two
notes.

When deciding how to tune the two middle strings, I can imagine this
hypothetical musician deciding to do something fairly simple - space them
both one whole interval apart, starting with the first string.  Doing so, of
course, means that the distance between the third and fourth strings is only
a half interval, but such is life.  Label the strings and you have, roughly,
half a major scale.

Is this accurate?  I have no idea, but it seems plausible to me.  One starts
with some fixed constraints (for example, the constraint that one always
needs two strings in a 4:3 ratio), and you fill in the rest around those
constraints.

Of course this *doesn't* explain why the notes in C major scale, in
particular, all get simple letters.  Maybe middle C is just a really easy
note to play?

## Will This Help?

It says something about my personality that I find all this music theory
somewhat more interesting than actually practising my theremin.  It's a
character flaw; I'm more bookish than practical.

So all this theory, while fascinating, does not help me play an instrument.

Not sure how to get around this, except to just put the research down and
start playing, I guess.   We'll see how it goes.


[1]: http://www.carolinaeyck.com/method/
[2]: https://en.wikipedia.org/wiki/C_major
[3]: https://en.wikipedia.org/wiki/Lyre
[4]: https://www.youtube.com/channel/UCYkSWMBi1pZUqjs2OngjUyA
[5]: https://en.wikipedia.org/wiki/Do-Re-Mi
[6]: https://en.wikipedia.org/wiki/The_Sound_of_Music
[7]: https://en.wikipedia.org/wiki/Major_scale
[8]: https://en.wikipedia.org/wiki/Chromatic_scale
[9]: https://en.wikipedia.org/wiki/D_major
