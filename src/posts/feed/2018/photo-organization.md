---
date: 2018-11-18 12:23:39-05:00
modified: 2018-11-18 12:23:39-05:00
tags:
- technical
- photography
title: How I Organize my Photos
---

I have a large collection of digital photos dating back over 15 years.  An
impressively small fraction of them are actually any good, but that's a
different conversation, probably revolving around my digital hoarding
habits.

Such a large collection deserves a particular method of organization.  Or
maybe it doesn't.  Did I mention they're mostly mediocre?  Anyway, I have
one! I thought I'd share in case anyone finds it useful (including a future
version of myself, my mind being a sieve and all).

The procedure I came up with is strongly influenced by several personal
idiosyncrasies.  Obviously, not everyone shares these traits, so your
mileage will definitely vary - though I think it's always interesting to
read about how other people do things.

 * I have a strict rule about keeping my original photos intact and under my
   control.  The masters are never stored anywhere but on my hard drive
   (which is backed up every night).
   
 * The process of curating, editing and sharing my photos is time consuming,
   and I don't want the fruits of that labour to be reside in an external
   service over which I have no control.  So while I don't mind showing or
   exporting my albums on Facebook or Instagram, I don't want these services
   to _define_ my albums.
   
 * I don't want to rely too much on any particular application.  If I ever
   decide to switch tools, I don't want that process to be too arduous.
   
 * I'm a Linux user, and I tend to prefer command line tools, though when
   you're dealing with photos it's often counterproductive to take this
   attitude to extremes.

All of my photos, every single one, are stored and organized under one
folder on my hard drive.  When I have new photos that I want to bring into
the fold, the first step is to download the contents of my camera onto my
hard disk.  Sometimes the camera is my phone and sometimes it's a real
camera.  In any case, I don't try to be clever at this point; I just do a
brainless dump of the camera contents into a temporary folder.

To make things easier for the next step, this temporary folder is usually
tucked under the folder where I keep all my organized photos.

Since I tend not to delete the contents of my camera when I do this, I'll
often end up with duplicates at this point - some (maybe even most) of the
photos I just downloaded will have already been moved to their final
destinations.  The next step, therefore, is to remove the duplicates.  For
this purpose I use a command line tool called [fdupes][1], readily available
on my Linux Mint distribution.  One line will do it:

`fdupes -rdN organized_image_folder/`

This will remove all the duplicates files from the folder I passed - which
should only come from the folder I dumped.  It doesn't matter which of the
duplicates gets deleted, since the next step will smooth that over.

The next step is to move my new photos into my organized image folder.  For
this purpose I use a command line tool called [sortphotos][2].  From the
README file:

> SortPhotos is a Python script that organizes photos into folders by date
> and/or time (year, year/month, year/month/day, or other custom formats).

The tool uses photo EXIF data to move the photos into a folder hierarchy
that you define.  I just use the default folder structure, which is just
year and month based.  An example would be "2018/10-Oct".

If there is an obvious way to sub-categorize the photos within the folder
structure, I'll do that as well.  So, for example, if I went on a trip some
month, or it was someone's birthday, I'll tuck those photos under a
sub-folder named for the event.  So you'll occasionally see a folder
hierarchy that looks like year/month/category (for example,
"2018/10-Oct/chicago").

If I don't feel that any photos are worth sharing at this point, the process
ends here.  Otherwise, the work continues.

## Selecting and Editing Photos for Sharing

As I mentioned before, because I take a lot of mediocre photos, only a small
fraction are suitable for sharing.  Even then, I often find myself editing
the selected photos, rotating and cropping and sharpening to compensate for
my lack of photography skills.

At this point I leave the command line and use an open source program called
[digikam][4].  I chose digikam because, in rough order of importance:

 * digikam treats your directory structure as sacrosanct. There is a direct,
   one-to-one correspondence between what digikam calls "albums" and the
   file folders where your images reside.  You can move photos around behind
   its back and it'll cope quite well.
   
 * It lets you add tags, keywords, flags, etc. directly in the file and the
   information will be _burned into the file_ as opposed to just updated in
   a database somewhere.  This is important if you ever decide to switch
   programs.
   
 * You can do searches by tag, keyword, etc, letting you create something
   like "smart albums" created from images which match certain criteria that
   you specify.
   
 * It has a built in photo editor, and it's very easy to save versions of
   the photo, keeping the original intact.

When I decide that some photos are worth sharing, I'll step though every one
in a particular folder (say, 2010/10-Oct/chicago) and I'll use digikam's
flagging facility to mark certain photos as "pending" in a single pass.

After that I'll do a second pass, using digikam's filtering facility to only
show me the photos I've flagged.  For each one I'll ask:

 * Do I really want it in my final album?
 
 * Do I need to edit it?
 
If I don't want it in my final album, I'll just remove flag it and move on
(note that I don't delete the photo).  If I decide I want it and it needs
editing, then I'll launch the built in editor and save a version of the
photo that I'm happy with, leaving the original alone.  Once I have a photo
I'm happy with and willing the share (either the original or an edited
version of it) I'll remove the flag and add a tag in the following format:

`album:<name_of_album>`

I prefix the album name with "album:" so that it doesn't interfere with any
other tags I might decide to add.

Another possibility is to simply stash the winners into their own sub-folder
instead of tagging them.  I opted not to do this because a) my folder
structure was already pretty nested as it was and I was leery of adding yet
another layer and b) I wanted the option to put the same photo into multiple
albums.

I also didn't want to simply move the selected photos into a completely
separate location, because I like the idea of the winners living in the same
folder as the losers - all my photos in one place.

## Processing Photos With Exiftool

As I alluded to before, one of the nice things about digikam is that it
modifies the image metadata directly in standard ways when you add a tag.
This allows your files to be manipulated after the fact with a command line
tool that can read this metadata.  The canonical tool for this purpose is
[exiftool][5].

For example, if you want to know which photos belong to which albums, you
can use this command:

`exiftool -T -r -directory -filename -keywords -if '$keywords =~ /album:/i' <image root>`

This will trawl through your entire photo collection, picking the photos
that have a keyword with "album:" somewhere in it, and print out a three
column table consisting of the folder, the file, and the keywords.  It might
take a long time to run, but at the end of it you'll have a complete summary
of your album information.

Another similar example, if you happen to know the specific album you're
looking for:

`exiftool -T -r -directory -filename -keywords -if '$keywords =~ /album:2018_chicago/i' <image root>`

If you want to create a folder with all the photos from a particular album
copied into it, you can use this command:

`exiftool -r -o .  '-directory=/home/dcr/media/images/chicago' -if
'$keywords =~ /album:2018_chicago/i' <folder with photos>`

The command is a bit weird.  You need the `-o` because otherwise the files
would be moved, not copied.  However, the `.` after the `-o` is just a dummy
directory, because the real directory is supplied in the `-directory` part.
Although exiftool is an impressive feat of engineering, I'll admit the
interface is a bit arcane.

Why am I bothering with all this?  I'm bothering with it because I like the
idea of being able to extract my curated photos and re-create my albums if I
ever lose access to my photo sharing service.

## Sharing Photos

After I've marked which photos are going into an album, the only step left
is to share them.  My photo sharing software of choice is [piwigo][3],
partly because I have a bias towards self-hosted software, but also because
piwigo lets people download the entire, unmodified image if they want.

As it turns out, digikam has built in support for piwigo (I found this out
much later; the decision to use piwigo and digikam were independent of each
other).  I use digikam to filter out all the photos except the ones that are
in the album I'm trying to share, which I will then upload right from the
interface.

Another way to do this is to use exiftool to copy the photos in the album
you're trying to share into a temporary folder, which you can then upload to
piwigo manually.

Of course, if you already store your photos as shareable albums directly on
your hard drive, then you can just upload the photos to piwigo straight
away.

## Final Thoughts

That's pretty much it!

A friend of mine once accused me of over engineering this whole thing .  She
may be right...or I may have just lacked documentation!  I guess time will
tell.

I'll admit that I find elegance in the concept of tagging my curated photos.
The photos remain sorted by date, and I don't have this artificial
distinction between winners and losers.

That being said, another friend of mine is brutal with his photos.  He
deletes any and all photos that don't make it into his albums.  For him,
it's _all_ about distinguishing between winners and losers.  There's a
certain harsh elegance to that approach as well.  I'll even cop to a bit of
envy - my photo collection is uncomfortably large.

But it makes me uncomfortable in other ways.  What if there's a photo that I
change my mind about?  It hasn't happened yet, but what if it does?

Anyway, perhaps my digital hoarding habits will change as I get older.  For
the moment, this approach appears to be workable.


[1]: https://linux.die.net/man/1/fdupes
[2]: https://github.com/andrewning/sortphotos
[3]: https://piwigo.org/
[4]: https://www.digikam.org/
[5]: https://www.sno.phy.queensu.ca/~phil/exiftool/
