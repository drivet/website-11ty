---
title: The Computer as a Tool
---

> If the designers of X-windows built cars, there would be no 
fewer than five steering wheels hidden about the cockpit, 
none of which followed the same principles -- but you'd be 
able to shift gears with your car stereo.  Useful feature, that.

-- From the programming notebooks of a heretic, 1990.

As I said on a previous page, I'm a hacker wannabe.  I am studying to be a
Linux guru, or trying to, anyway :-) I consider Linux to be a good, stable,
flexible operating system and I don't think MS-Windows is a particularly
good platform to program on.

At the same time, as an end user, I have a grudging respect for Microsoft
Corp. I like MS-Word. I like its toolbars. I like its spell checker. I like
Windows 95/98/NT as an environment for launching nifty programs. I like how
I can click on an Excel file and have the program start up automatically. I
like the ease with which I can create a new and reasonably complex program
with Visual C++.  I like all the devices that exist *Windows ready*. I like
the way I can navigate my hard disk with the Windows Explorer. And, above
all, I *dislike* people who dismiss all this ease of use as a sign of
*inferiority*, and then hold up Linux as their ideal system.

Do I like Linux? The answer depend on the particular task at hand.  If I'm
programming a difficult assignment with lots of files, and the makefile is
already written for me, then I love it. It tells me everything I need to
know. It lets me find memory overwrites with an ease that I'm hard-pressed
to find anywhere else. And the GNU C++ compiler is really *good*. I like the
fact that I can kill errant processes with a command at the prompt. I hate
the fact that I have write a makefile for all but the simplest of programs,
but I'm willing to overlook this for the benefits I receive. And I also know
that this discussion means absolutely bubkus to the average end-user when
s/he finds out that s/he has to start his/her favorite application by
*typing it at the command line*.

Herein lies the greatest weakness of Linux, and the reason why a bare bones
Linux/Xfree86 system will *never* be enough to bring down the so-called
MS-Windows monopoly. It's not because of Microsoft's alleged
anti-competitive practices. Everybody knows the answer: Linux is hell on the
end user (me included). Never mind that there's hardly any end-user software
available for it. Never mind that you'd be hard pressed to play the latest
games on it. I'm talking about the design of the underlying system. It took
me a week to figure out how to consistently launch a wallpaper at
startup. At one point I would log in to the computer and it would rudely log
me back out, for no apparent reason.

(It turns out that I had to launch the image display program (xv) from my
.xinitrc file as a background process. I had been launching it as a
foreground process, which one usually only does with window managers.
Launching a program in the foreground is apparently a signal to the system
that says something along the lines of "when this program exits, log me
out". This is so that the system will log you out when you quit your window
manager. Since I had rigged the xv call to quit as soon as the image was
displayed, and since it was in the foreground, the computer was logging me
out before it could even bring up an xterm. Silly me)

Am I griping about mere superficialities? Maybe, but these superficialities
are enough to bring many inexperienced users to their knees.  It's true that
X-Windows and Linux are, in general, *extremely* configurable, so that in
the hands of an experienced user is can be a very useful tool. But very few
of these features are built into the system. Many window managers, for
example, will let you configure popup menus that appear whenever you click
on the screen, so that you can add items to launch your favorite
programs. But first you have to learn that you *can* configure the menu
(it's not usually immediately obvious), then you have to learn that the
configuration options are usually stored in a file called .rc stored in
your home directory. Then you have to learn how to *read* the rc file. And
finally you can add your menu item. All that to launch a program from a
popup menu. And with MS-Windows you can just drag an icon onto the desktop.

I think the problem many so-called hackers have with MS-Windows is that it
isn't "hackable" (i.e. it's source code is not publicly available) and it's
configurabilty options aren't as flexible as X-Windows.  I personally don't
have a big problem with this, and I suspect most users don't either. Some
people have never gotten it into their heads that computers are just *tools*
and that the easier a tool is to use, the more it will be used. I don't care
how my toaster works. I just care that it does. Do I care that it can do a
million little things that I will likely never take advantage of? At the
same time I know that there are *some* people who are intensely interested
in toasters and that a majority of these people become toaster
engineers. More power to them. Just don't ask the <em>end-user</em> to
become a toaster engineer. Why should computers be any different?

Is Linux more stable than MS-Windows? Sure. Is Linux faster than
MS-Windows? Almost definitely. These are very valid reasons to admire
Linux. You'd be surprised how well it operates on an old Intel
386. But all this proves is that Linux is faster and more stable than
Windows. It doesn't prove that it's *better*. I could be wrong about this,
but I think most end-users prefer an easy-to-use desktop environment which
may not take full advantage of your hardware to an environment that's
difficult to use but which squeezes every last ounce of speed out of your
hardware. I think that most end-users prefer an environment that crashes now
and then (though not nearly as much as some people claim) in exchange for
draggable icons. At least, this seems to be the case. It's not like
Microsoft is the only choice out there.  You can buy a Mac. And you can buy
Linux from RedHat or Debian. But most people still buy Microsoft's products.

Are they fooled into this? Is it a big ruse put on by Microsoft marketing
experts? Is the power of Microsoft that great? I doubt it. It took Microsoft
*seven years* to produce a version of Windows that was palatable to
end-users. And how come nobody was fooled when Internet Explorer first came
out? It took a couple of releases before IE could even come close to
rivaling Netscape. Furthermore, in those markets where a nice GUI doesn't
count for much, where raw, industrial strength power is gold (I'm talking
mostly about web server technology) Microsoft usually gets its butt
kicked. Do you think an "abuse of market power" could bring down Apache?

Having said all that, I personally believe that Linux has the potential to
become more popular than Windows in about five years or so, *if* steps are
taken to make it easier to use. For the longest time there was no economical
desktop analog for X-Windows on a PC. With the advent of open source
desktops like the GNOME and KDE, I think this about to change.  Like most
Linux products, they will be insanely customizable. But, from what I can
tell, I don't think the configuration scripts will be the only means of
customizing your interface. At the very least, I think somebody will write a
GUI front end to the scripts. If they don't, they will have missed the point
of the entire venture.

I've been rambling on for a while, so let me summarize. I like Linux. I
really do. I like the flexibility it gives me.  At the same time I
understand why some other people might be annoyed with it. So I also
understand why Microsoft does so well. I don't begrudge them their
success. They don't scare me. There's *always* a choice. They're monopoly
was earned through a nifty looking desktop.  Nothing else.
