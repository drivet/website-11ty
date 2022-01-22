---
title: Animats and Active Vision Systems in Realistic 3D Virtual Worlds
---

My fourth year undergraduate thesis (required to graduate from
[Engineering Science][1] at [U of T][2]) was done under
[Demetri Terzopoulos][3]. This was one of the more interesting projects I've
ever worked on.

(Note that this is not quite the same as a graduate thesis. Undergraduate
theses are basically glorified research projects where you are not really
expected to come up with anything original. Thank God.)

I started thinking about my thesis near the end of my third year. I had this
idea that I could work for my thesis professor (whoever that turned out to
be) during the summer and then continue on into my fouth year. My main
method of research consisted of perusing the various homepages of the
various professors at U of T, hoping to stumble onto something worth doing.
That was when I ran into Terzopoulos' homepage and I was hooked.

The first step was to convince him to actually take me as thesis student.
This in itself was challenging, since the guy is nearly impossible to get a
hold of; he has a reputation of working odd hours. When I finally managed to
corner him in his office, the first question he asked was whether I knew how
to program. This took me by surprise; did he think I would ask to be his
student if I didn't know my way around at least one or two programming
languages? I ended up showing him work I did on the [Graphics Pipeline](gp/)
to seal the deal.

The next step was to get an [NSERC][4] grant. This turned out to be easy as
pie. There must have been more grants than applicants. I don't think anyone
who applied got turned down.

Terzopoulos's work in the past mostly revolved around simulations of fish in
marine habitats. The idea for my thesis was to transplant the vision
algorithms used by the fish into a realistic 3D urban environment and then
create a human model (called an animat) to go with it (or, at least, to get
as far as I could in this task). Creating a realistic human model from
scratch is a thesis project in itself, so we decided to try and use
something called [DI-Guy][5] from [Boston Dynamics][6]

My summer was loads of fun. I got to work in both the AI and Graphics labs
at U of T, on big SGI machines. The software I used was [SGI Performer][7]
and it turned out that one of the town simulations that came with Performer
suite was perfect as a "realistic 3D urban environment". It was just left
for me to actually get it up and running and to implement the vision system.

Anyway, if you're interested, I have the [final paper](/oldsite/thesis.ps). It was
originally written in LaTeX, but I unfortunately lost all the LaTeX code
when my hard drive crashed. I did manage to save the PostScript, which is
what you get here.

Oh, and I got a 98% :)

[1]: http://www.engsci.utoronto.ca
[2]: http://www.utoronto.ca
[3]: http://www.cs.toronto.edu/~dt/
[4]: http://www.nserc-crsng.gc.ca/
[5]: http://www.bostondynamics.com/diguy/company-overview.html
[6]: http://www.bostondynamics.com
[7]: https://en.wikipedia.org/wiki/OpenGL_Performer
