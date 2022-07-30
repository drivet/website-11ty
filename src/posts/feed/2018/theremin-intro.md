---
date: 2018-05-02T02:45:07.000Z
modified: 2018-05-02T02:45:07.000Z
tags:
  - theremin
title: On Pulling Musical Notes Out of Thin Air
image: /static/img/etherwave_theremin.jpg
---

I am an unapologetic [Downton Abbey][3] fan.  The series is full of
memorable scenes, but one in particular has stuck with me.  Daisy, one of
the scullery maids, is asked if she turned on the electric lights in one of
the rooms and she replies "No. I daren't".

It seems like such a minor, throwaway line, but I feel like it succinctly
captures how the uninitiated must have felt about electricity back then.
Daisy is downright _afraid_ of it.  Steam and fire are very direct and
literal sources of energy, but electricity is much more abstract.  You never
see the electricity moving or burning, even as the motor spins or the lamp
shines.

It must have seemed rather ghostly.  Spooky, even.

And the idea that one could make music from this stuff - that one could
build a device to turn this abstract, invisible force into something audible
and pleasing to the ear - must have been downright _unnerving_.

## Music from the Void

People often assume that electronic musical instruments were invented in the
1960's with the advent of the [synthesizer][2], but that's not quite not
true.  The world's first electronic musical instrument actually predates the
synthesize by a good 40 years.  It's called the _[theremin][1]_ and it
certainly qualifies as one of the most unusual instruments ever invented.
To my knowledge it is the only musical instrument that one plays with no
physical contact.

Even if you've never heard of a theremin, you've probably heard one being
played without realizing it.  You know that high-pitched twang from a
typical 1950's sci-fi movie?  That's a theremin in action.  Because it was
typically used for these kinds of soundtracks, people came to associate it
with eerie, spooky situations (which I feel is somewhat appropriate, given
the eerie, spooky nature of electricity itself).

The instrument consists of two metal rods (usually called "antenna", though
they operate on different principles).  One of the rods is usually pointing
in the air, and the other is usually curved in a loop.  One controls the
pitch by moving one's hand to and from the first rod; the closer one's hand,
the higher the pitch.  The second antenna controls the volume in the same
way; moving your hand closer lowers the volume.  A typical setup looks like
this:

<img src="/static/img/etherwave_theremin.jpg" 
     alt="Complete Open Theremin Board" class="u-featured entry__photo image" />

Given the lack of anything physical to manipulate, they are notoriously
difficult to play.  They are also the butt of many jokes.  For example:

<div style="clear: both; text-align: center;">
    <iframe width="560" height="315"
    src="https://www.youtube.com/embed/_YYABE0R3uA" frameborder="0"
    allow="autoplay; encrypted-media" allowfullscreen></iframe>
</div>

In the right hands, however, they are very, very cool.  Don't believe me?
Consider exhibit A. She starts playing the theremin around the one minute
mark but you should watch the whole thing :

<div style="clear: both; text-align: center;">
    <iframe width="560" height="315"
    src="https://www.youtube.com/embed/ajM4vYCZMZk" frameborder="0"
    allow="autoplay; encrypted-media" allowfullscreen></iframe>
</div>

I'll be honest, this video, more than any other, makes me want to learn to
play.

## Basic Principles

From a cultural and musical standpoint, theremins are fascinating devices.
They are also fascinating from an engineering standpoint.

Internally, for each antenna, one constructs an [electronic oscillator][5]
out of [capacitors][6] and [inductors][7].  The frequency of the oscillator
is a function of the particular capacitors and inductors used in the
circuit.  Each antenna forms one half of a capacitor, connected directly
into its oscillator.  The player's hand forms the other half; when their
hand moves to and fro, the capacitance of the antenna changes, thus changing
the frequency of the oscillator.  Though this frequency is not in the
audible range, theremins use a process called [_heterodyning_][4], which
basically consists of shifting the frequency onto the audible spectrum so
that one may actually hear a note.

In other words, a theremin works because the player effectively forms a part
of the circuit - one half of a capacitor. I don't know why, but I find that
idea pleasing to think about.

## Onward and Upward

Why am I writing about theremins?  Well, one answer is simply that I like
theremins, and that this is _my_ blog and I'll write about what I darn well
please.

_Another_ answer, closer to the mark, is that they're great fodder if you're
looking to tinker with hobby electronics.  Stay tuned for more information
on how far I got on _that_ score.


[1]: https://en.wikipedia.org/wiki/Theremin
[2]: https://en.wikipedia.org/wiki/Synthesizer
[3]: https://en.wikipedia.org/wiki/Downton_Abbey
[4]: https://en.wikipedia.org/wiki/Heterodyne
[5]: https://en.wikipedia.org/wiki/Electronic_oscillator
[6]: https://en.wikipedia.org/wiki/Capacitor
[7]: https://en.wikipedia.org/wiki/Inductor
