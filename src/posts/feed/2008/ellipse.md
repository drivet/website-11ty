---
date: 2008-01-06 20:26:29-05:00
modified: 2015-05-16 01:52:56.237031-04:00
title: Solar System Simulations
---

When I was in high school, I used to program two dimensional solar
system simulations for fun (as you can surely tell, I was extremely
popular and desired by all the ladies far and wide).  I would put a
"sun" in the middle of the screen and I'd spin "planets" around in
simulated orbits.

There are essentially two ways you can go about programming solar
system simulations. Technique 1, which for lack of a better term I
will call the *explicit* technique, means deciding (or realizing) at
the outset that the orbits of your planets will be elliptical, and
then writing your program based on that. The explicit technique works
because there are certain exploitable patterns in elliptical planetary
orbits that can be harnessed to great advantage. Planets move faster
when they are closer to the sun, for example, and slower when they are
further away, in mathematically determined ways. One can derive
formulae for the position of the planet on the orbit versus time.

Technique 2, which I used for my simulations and which for lack of a
better term I will call the *implicit* technique, is at the same time
simpler and more complex.  You don't need to know anything about
ellipses or how fast a planet moves when it's close to the sun or
anything at all, in general, about how solar systems operate. What you
*do* need to know are Newton's laws of gravity and motion and how to
solve a first order differential equation with numerical techniques.

The implicit technique is simpler than the explicit technique in the
same way that a lump of raw pork meat is simpler than a cooked pork
sausage: the lump of raw meat is unprocessed and unrefined, while a
sausage is both processed and refined.  The equations of motion for a
planet moving in a *specifically* elliptical orbit are derived from
the more general and more generic laws of motion that come from
Newton. Work and refinement are required to produce an equation which
obviously describes movement along an ellipse, in the same way that
work is required to produce a sausage from a lump of raw meat.

The implicit technique is also, paradoxically, more complex than the
explicit technique because solving first order differential equations
is more complex than just using equations which directly describe an
elliptical orbit.

One key thing to note is that both methods produce the familiar
elliptical orbits if you set them up correctly. The thing about the
implicit method that makes it interesting is that you will get an
elliptical orbit if the conditions are right, despite the fact that
your program knows nothing at all about ellipses.  Nowhere do you ever
mention an ellipse in your program, nowhere are there any assumptions
about how the orbit is supposed to look, or even that there should be
a periodic orbit at all, but the ellipse nonetheless emerges from the
simulation; it just appears naturally out of the math.

Which technique is superior?  As always, it depends. If your only goal
is to spin out a simulation of our own solar system, you can assume
that the orbits are ellipses and your job will be easier if you use
the explicit method.  On the other hand, the implicit method is much
more open-ended; once you have Newton's laws programmed into the
simulation you can simulate a conventional solar system, but the same
program will let you run simulations of binary star systems, systems
with less or more planets than our own, systems that run normally
until a heavy comet zooms by - pretty much anything. These sorts of
fun and games are simply not possible with the explicit technique -
these configurations were not built into the equations. To use the
aforementioned analogy: once you have a lump of raw pork, you can make
a sausage if you have the time and the inclination, but you're not
limited to that. You can cut pork chops, carve out a fletch of bacon -
anything you want. If you start with a sausage, all you get is a
sausage.

The essential difference is that the implicit technique is less
presumptuous about what to expect when the simulation runs. As I
mentioned, we don't assume that the orbit will be an ellipse, or even
that there will be an orbit at all. We just enter the laws of gravity
and see what happens.  The results are often surprising. Yes, I found
that I could obtain the familiar elliptical orbit on my monitor, but I
also found that, for example, if a planet is moving too fast, you
don't get an orbit at all. Instead, the planet "slingshots" around the
sun in a trajectory shaped like a parabola - something that was not
obvious to me at all.

Since playing around with these programs I have learned two things:

* the simplistic numerical method I "invented" to solve Newton's laws
  of motion in my simulation has actually been attributed to Leonard
  Euler and is dubbed "Euler's Method".  It's not very good as the
  errors are quite significant unless you take an extremely small
  time interval.

* more importantly, the elliptical orbits and parabolic trajectories
  that appeared "naturally out of the math" were examples of what I
  later learned were called "emergent properties". These are
  properties of the system as a whole, things that are not obvious
  until you actually get your hands dirty and try things out.

Emergent properties are basically macroscopic effects of local
behavior. There are many examples of emergent properties in
nature. One example is Jupiter's Great Red Spot, that enormous, stable
yet chaotic storm that just doesn't seem to go away. Once scientists
program the proper Jovian weather model into a computer, the Great Red
Spot just emerges out of the calculations. It's not something that's
obvious from the math, and the program doesn't know anything about
Great Red Spots or storms, but it's nonetheless *there*, in the model,
in a fundamental sense.