---
date: 2012-12-15T05:51:46.000Z
modified: 2015-05-16T05:53:03.581Z
tags:
  - technical
title: Of Red Hats and Mints
---

## Part 1: A Brief History of Desmond, as it Relates to Linux

I've been using Linux since about 1998, when I started my first "real" job
at [PCI Geomatics][1].  At the time, most of PCI's workstations ran a
version of Red Hat Linux.  The standard setup revolved around FVWM and
Emacs.

I did not find Linux easy.  But it was *fun*.

I installed Linux for the first time in 2000, on a computer I built from
scratch (my first one).  It was Red Hat 6.2.  That installation was not what
you'd call "smooth", but I did eventually get it working.

Over the past 12 years, I've used 3 distributions.  So that means, on
average, I tend to stick with a distribution for about 4 years.  

The first one, as I said, was Red Hat, since it's what I had used at work.
About 4 years after that, I decided that I wanted to build a PVR, using
[MythTV][2].  The drivers for the capture card I wanted to use were not part
of the standard Linux kernel, so I had to compile them myself into a custom
kernel.  This was possible with Red Hat, but was easier with a distribution
called [Gentoo][3], whose gimmick at the time was that it was primarily a
source based distribution; packages were installed from source, and compiled
with every update.  This included the kernel - hence, making a custom kernel
with my capture card drivers was easier.

Gentoo had another advantage: the package management system was excellent,
particularly in the way it handled dependencies.  At the time, I was not
aware of yum for Red Hat - installing a package on a Red Hat system was an
often frustrating excursion to [dependency hell][4].  Gentoo more or less
eliminated that particular hell, as its package system automatically
resolved any dependencies required for any package you wanted to install.
It was awesome.

Eventually though, the constant compiling started grating on me.  Nominally,
Gentoo did all this compiling to give the user an oppourtunity to customize
the compiler flags, on the theory that it would boost system performance.
That being said, I never noticed my system going any faster as a result of a
compiler flag tweak.  Mostly, I felt that I was doing all this compiling for
no particular reason.  The large packages often took all night to
install/compile - I used to dread the XFree86 updates.

So, I started searching again.  My search led me to Debian.  Debian's
package management system, called APT, is like Gentoo's in that it will
manage all the dependencies for you.  It's unlike Gentoo's in that it's a
binary distribution.  Updates no longer took all night.  It was great.

Debian also makes it relatively easy to create a system that works decently
on older hardware; it doesn't install much by default unless you ask it to.
You have to realize that my current computer is the same one that I was
using in 2003/2004.  That makes it about 9 years old, and it shows.  To be
productive on this kind of machine, you need to cultivate an attitude of
(relative) minimalism.  This means that for the last several years, I've
been doggedly avoiding software that even *smelled* like it had anything to
do with GNOME or KDE.  With Debian, I opted to install the [Xfce][5] desktop
environment, and that was that.

I've been more or less happily using Debian Stable for the past 4 years -
since about 2008.  But even with the system stripped down to just what I
need, it's still pretty slow.  Switching from Firefox to Chrome rejuvenated
my setup to some extent but, even so, applications like Facebook and Gmail
are noticeably sluggish - especially compared to how they run on my work
computer, which is significantly better.  I like to joke that Chrome makes
my 9 year old computer feel like a 6 year old computer.

So, I decided that it was time to get a new computer.  This is a big deal.
I haven't bought a new desktop computer in 9 years.  I eventually settled on
an i7 3.4Ghz quad-core CPU, with 8GB of RAM.

## Part 2: The Minty Road, Or Why Desmond Chose Linux Mint

But now, I'm faced with some choices: stick with Debian or go with another
distribution?  

One mark against Debian revolves around issue of software updates.  Debian
Stable, as the name implies, is very stable.  It achieves this stability by
restricting the packages and the versions that get included in the
distribution.  Consequently, the software available to a Debian user via the
package management system is often somewhat out of date compared to other
distributions.

In addition, one of my main reasons for using Debian - minimalism - doesn't
really apply to my new computer.  My new system can run KDE or GNOME without
any trouble.  Hell, it could probably run *both*, if that were actually a
thing.

Another one of my main reasons for sticking with Debian, APT, is a good one,
but it turns out that APT is not unique to Debian.  Various another
distributions, like [Ubuntu][6] and [Linux Mint][7], by virtue of being
based off of Debian, also use APT as their package management system.

Ubuntu - based off of Debian Unstable, the version of Debian that acts as a
kind of testing ground before the software gets deposited in the Stable
repository - prides itself on being the "easy" Linux.  It installs with very
little fuss, and very little in the way of configuration, to the point where
it will actually pick a desktop environment for you.  Since it's based off
of Debian Unstable, the software tends to be pretty up to date as well.

But Ubuntu seems to have shot itself in the foot with the new, much reviled
[Unity][8] interface. I'm a bit loathe to try it.

So that leaves Linux Mint.  Linux Mint peddles itself as a kind of refuge
for Ubuntu users who don't want to use Unity.  Their repositories are based
off of Ubuntu's (and hence are twice removed from Debian's) and they offer
their own GNOME based desktops, [MATE][9] and [Cinnamon][10].  They also
offer Xfce, which makes me feels right at home.

Also making me feel right at home is Linux Mint Debian Edition (LMDE), which
is a version of Linux Mint based off of Debian Testing, a kind of middle
ground between Debian Stable and Debian Unstable.  Unlike standard Mint,
Ubuntu and plain vanilla Debian, LMDE is a rolling distribution.  You
install once and simply keep your system up to date with updates as they
come - a bit like Gentoo!  And since the repository is from Debian Testing,
the software is more up to date than a standard Debian install.  It seems
perfect.

## Part 3: In Which Desmond Installs Linux Mint Debian Edition and Feels Slightly Disconcerted

I installed Linux Mint Debian Edition and felt slightly disconcerted.

You need to understand, I've been avoiding GNOME and KDE for the better part
of 8 years.  Though I wouldn't characterize Debian or Gentoo as *minimal*, I
*would* characterize them as *obedient* - they tend not to install anything
unless you ask for it.  If you don't install a GNOME application, then GNOME
won't be on your system, simple as that.

Linux Mint, on the other hand, barely asks you any questions during the
install.  Your installation choices revolve around your choice of desktop -
I didn't actually see an option that didn't install *some* kind of desktop
environment for you.  If you choose Cinnamon, then you get Cinnamon.  If you
choose MATE, then you get MATE.  And - here's the kicker - if you choose
Xfce, like I did, you get Xfce - *plus* a whole bunch of GNOME applications
that you didn't ask for.

Like I said, I was disconcerted.  I found no less then 4 media players
available on my menu, and at least one of them ([Banshee][11])) required a
whole [Mono][12] subsystem to run.  I also found a GNOME based DVD burning
application on the same menu - even though I didn't have a DVD writer
installed at the time.  This isn't what I'm used to.

Of course, I'd be lying if I said that all this anti-minimalism wasn't
somewhat refreshing at the same time.  What finally pushed my opinion over
the edge in Mint's favour was a fully functioning Firefox/Flash combination.
As soon as the install was finished and I logged in, I could launch Firefox,
go to YouTube, and watch a video with no further effort.  Non-Linux users
won't appreciated just how nice that is.

I *can* just uninstall the stuff I don't want, after all.  I think I could
get used to this.  Is this what having a modern computer is like?

## Part 4: The Conclusion, Predictably Positioned at the End

At the moment, I like what I'm seeing of Xfce on Linux Mint - well, okay,
Xfce *plus GNOME* since the installer insisted on dumping half of GNOME on
my system as well.

I don't know if I'm going to stay with it, though.  My computer is such that
I could run a full GNOME or KDE environment - or even [Enlightenment][13]),
if I had a mind to do so.  Or Cinnamon. Or MATE.  My point is that I'm
actually in a position to choose a real desktop environment this time
around.

Though, I have to admit, it'd be kinda funny if I finally ended up using
Xfce in the end.

*I* think that would be funny :-)

[1]: http://www.pcigeomatics.com/
[2]: http://www.mythtv.org/
[3]: http://www.gentoo.org/
[4]: http://en.wikipedia.org/wiki/Dependency_hell
[5]: http://www.xfce.org/
[6]: http://www.ubuntu.com/
[7]: http://linuxmint.com/
[8]: http://en.wikipedia.org/wiki/Unity_(user_interface)
[9]: http://en.wikipedia.org/wiki/MATE_(desktop_environment)
[10]: http://en.wikipedia.org/wiki/Cinnamon_(user_interface)
[11]: http://en.wikipedia.org/wiki/Banshee_(media_player)
[12]: http://en.wikipedia.org/wiki/Mono_(software)
[13]: http://en.wikipedia.org/wiki/Enlightenment_(window_manager)
