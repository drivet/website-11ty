---
date: 2018-03-30 13:25:49-04:00
modified: 2018-03-30 13:25:49-04:00
tags:
- blogging
- python
- pelican
- emacs
title: Blogging with Emacs and Pelican
---

[Pelican][1] is my blogging engine of choice these days.  Given that
[Emacs][2] is usually (though not always) my text editor of choice, it made
sense to try and streamline the process of writing blog entries for Pelican
with Emacs.  What follows is my attempt to document such an endeavour,
partly because I think it might be useful to the (undoubtedly tiny) cross
section of people who use both Emacs and Pelican, but mostly so that I have
something to refer back to when the need arises.

Note that this blog entry does not cover things like actually setting up
your Pelican blog.  It also doesn't cover my reasons for using Pelican in
the first place; for that, feel free to peruse my other [blog entry][16] on
the subject.

## Projectile and Version Control

Emacs is already pretty good at editing files, once you manage to find and
load them into buffers. _Projectile_, on the other hand, is an Emacs package
for managing the (sometimes very large) collection of files that make up
your _project_ (such as, to pick an example out of thin air, your Pelican
blog).  You can peruse the [github page][4] for more information but,
briefly, with Projectile you can easily do things like find a file, perform
a grep, or run a compile command, all scoped to the particular project
you're working on.  A good chunk of the advantages of using Emacs for
writing Pelican entries ultimately stem from using Projectile.

Projectile's notion of a "project" is flexible but version controlled
repositories (such as those managed by Git, Mercurial, Daarcs or Bazaar) are
treated as such by default.  Thus, the first step should be to put your
Pelican blog under one of these version control systems.  The usual
advantages of a version control system apply but, more importantly with
respect to Emacs integration, you get to use Projectile out of the box.

## Using Magit

I chose Git as my version control system for my blog.  I'll cop to
preferring the comparably tidy Mercurial command line interface to Git's
hodge podge of commands and options, but there are actually several reasons
to specifically use Git here:

* Git has much more mind share than any other version control system, so
  you're more likely to get help when you need it.
* [Github][10] is a hotbed of developer activity.  This, in itself,
  probably constitutes the single most important to know and use Git.
* Emacs has rather nice Git support in the form of Magit.

[Magit][5] is a full Git front end embedded in Emacs. That makes it a
complex piece of software but if you're like me and you try and keep your
repository history fairly linear, then a little bit of Magit goes a long
way.  The [Getting Started][6] page basically covers 90% of what you need to
know.

Everything starts with the Magit status buffer, brought up with the
`magit-status` command (bound to `C-x g`, as per the guide).  From the
status buffer, you can see various sections displaying various pieces of
information, among them:

* Unstaged changes
* Staged changes
* Untracked files

Using the status buffer is pretty easy if you stick to the very basics:

* Pressing `s` next to an unstaged file will stage it.
* Pressing `u` next to a staged file will unstage it.
* Pressing `k` will revert a file.
* Staging/unstaging/reverting applies to hunks as well.  You can use TAB to
  expand a file to see the hunks.
* Pressing `c` will bring up the commit buffer.  Just press `c` again to
  start the commit message, and C-c C-c to make the commit.
* Pressing `P` will bring up the push buffer, Press `p` to actually push
  your changes
* Pressing `F` will bring up the pull buffer (i.e. to pull and merge in one
  shot).
   
Git is, of course, capable of much, much more than this, and by extension so
is Magit, but I don't make use of these features on my Pelican repository,
so I haven't bothered to go that far (yet).

## Compiling and Publishing

Projectile lets you easily run a compile command for your project (C-c p c).
You can configure the default command with the
`projectile-project-compilation-cmd` variable, which I've set via a standard
Emacs `.dir-local.el` file located at the root of my blog:

``` elisp
((nil . ((projectile-project-compilation-cmd . "make"))))
```

My default compile command is a simple `make` because Pelican can easily
produce a Makefile for your blog out of the box.  Makefiles can seem rather
old school nowadays, but for something as simple as blog compilation, it's a
good fit.

Pelican's auto-generated Makefile is somewhat bloated so in my case I ended
up removing removed the cruft I wasn't using.  The end result looks like
this:

``` makefile
PY=python
PELICAN?=pelican
PELICANOPTS=

BASEDIR=$(CURDIR)
INPUTDIR=$(BASEDIR)/content
OUTPUTDIR=$(BASEDIR)/output

CONFFILE=$(BASEDIR)/pelicanconf.py
PUBLISHCONF=$(BASEDIR)/publishconf.py

SSH_HOST=yourhost.com
SSH_USER=youruser
SSH_TARGET_DIR=/home/youruser/wwwroot/pelican_blog

html:
  $(PELICAN) -s $(CONFFILE) $(INPUTDIR) -o $(OUTPUTDIR)

clean:
  [ ! -d $(OUTPUTDIR) ] || rm -rf $(OUTPUTDIR)

regenerate:
  $(PELICAN) -r $(INPUTDIR) -o $(OUTPUTDIR) -s $(CONFFILE) $(PELICANOPTS)

publish:
  $(PELICAN) $(INPUTDIR) -o $(OUTPUTDIR) -s $(PUBLISHCONF) $(PELICANOPTS)

rsync: publish
  rsync -avzhe ssh $(OUTPUTDIR)/* $(SSH_USER)@$(SSH_HOST):$(SSH_TARGET_DIR)

serve:
  cd $(OUTPUTDIR) && $(PY) -m pelican.server
```

The targets can be summarized as follows:

* `html` will generate a local copy of your blog.  This is the default
  target, and hence what gets run by Projectile by default.
* `publish` is like `html` except it uses a special publish configuration
  file.
* `rsync` depends on `publish` and copies over the generated blog to your
  server with SSH.
* `regenerate` is like `html` but keeps running and will regenerate your
  blog upon detecting that something has changed.
* `serve` will start a local HTTP server so you can see your blog.

Projectile lets you edit the compile command before running it, so you can
easily `publish` and `rsync` using this mechanism as well.

### Using a Virtualenv

A Pelican based blog such as my own is essentially a specialized Python
project and, like all my Python projects, I have the supporting libraries
tucked away in a [virtualenv][13].  Hence, the above Makefile doesn't
actually work out of the box.

Emacs comes with several virtualenv packages to help in this situation.  I
used to use [virtualenvwrapper.el][14] here but I decided to switch to
[pyvenv][15] since I get the (perhaps erroneous) impression that it works
better with Python 3.  Or maybe I just wanted to try something new.  In any
case, pyvenv is easy to use.  I set up a key sequence to easily switch to my
Pelican environment:

``` elisp
(require 'pyvenv)
(defun pyvenv-activate-pelican ()
  (interactive)
  (pyvenv-activate pelican-venv-root))
(global-set-key (kbd "C-c w v") 'pyvenv-activate-pelican)
```

## Using Markdown

My blog entries are all written in [Markdown][7], which I edit in Emacs
using [Markdown mode][8].  Markdown's basic idea is to take "natural"
markers in pure text and convert them to HTML; a piece of text flanked by
the "*" symbol, for example, will be rendered in bold.  This is an organic
fit for people like me, who like to write in pure text and who tend to use
these kinds of markers in their writing anyway.

Markdown mode provides syntax highlighting and editing capabilities but
since markdown is already pretty easy to write on its own, and my articles
are not overly structured, I haven't actually made much use of the editing
capabilities.  I just write my text out manually.

One major exception would be the management of external content, like
hyperlinks or images.  It's a bit difficult to devise an organic way to fit
these kinds of artifacts into your writing.  In markdown, there are two ways
to do it.  With option 1, you can inline the URL of the external resource
right next to the text with which it's associated, like this:

``` markdown
[Some text](http://www.example.com)
```

With option 2, you can use a reference label which has to match up to a URL
later in your article, like this:

``` markdown
[Some text][label]  
...  
...  

[label]: http://www.example.com
```

I'm using hyperlinks in the above example.  Images have a similar structure,
but the "text" is considered to be the "alternative text" for the image tag,
like this:

``` markdown
![Alt text](/img/piano.jpg)
```

or this:

``` markdown
![Alt text][label]
...  
...  

[label]: /img/piano.jpg
```

Both options involve some non-trivial syntax, which is not unexpected given
that links and images are not really "natural" textual artifacts.  I do
prefer the reference style because it makes for better flowing text and the
syntax is a bit easier to remember, but it comes at a cost: you have to
think up a label, and you have to put the actual URL somewhere else in your
document.

Markdown mode helps here.  It provides an interactive command,
`markdown-insert-link` (bound to `C-c C-l`) which is a kind of "all-in-one"
interactive function which lets you enter an inline or reference link,
depending on the parameters you provide.  Reference links will be inserted
at your preferred location (controlled by the `markdown-reference-location`
variable, which I've set to `end`, i.e. the end of my document).

### Automatic Reference Labels

This works, as far as it goes, but it's not quite as convenient as it could
be.  I generally don't try to attach any semantic meaning to my link or
image labels; my usual approach is to simply use an ever increasing series
of numbers.  This lends itself well to automation; my next label is always
just the next number after the previous one.

With some shameless pilfery from [Emacs Wiki][9], I managed to cobble
together some functions to allow me to easily add referenced links and
images, incrementing the label as you add more.  Here is the code:


``` elisp
(defun my-markdown-insert-reference-image-dwim ()
  "Insert a reference image of the form ![text][label] at point.
If there is an active region, the text in the region will be used
as the alt text.  If the point is at a word, it will be used as
the alt text.  Otherwise, the alt text will be read from the
minibuffer.  The ref label will be read from the minibuffer in
both cases, with completion from the set of currently defined
references.  To create an implicit reference link, press RET to
accept the default, an empty label.  If the entered referenced
label is not defined, additionally prompt for the URL
and (optional) title.  The reference definition is placed at the
location determined by `markdown-reference-location'."
  (interactive)
  (my-markdown-insert-reference-dwim 't))

(defun my-markdown-insert-reference-link-dwim ()
  "Insert a reference link of the form [text][label] at point.
If there is an active region, the text in the region will be used
as the link text.  If the point is at a word, it will be used as
the link text.  Otherwise, the link text will be read from the
minibuffer.  The link label will be read from the minibuffer in
both cases, with completion from the set of currently defined
references.  To create an implicit reference link, press RET to
accept the default, an empty label.  If the entered referenced
label is not defined, additionally prompt for the URL
and (optional) title.  The reference definition is placed at the
location determined by `markdown-reference-location'."
  (interactive)
  (my-markdown-insert-reference-dwim 'nil))

(defun my-markdown-insert-reference-dwim (image-p)
  (let* ((defined-labels (markdown-get-defined-references))
          (bounds (or (and (markdown-use-region-p)
                          (cons (region-beginning) (region-end)))
                      (markdown-bounds-of-thing-at-point 'word)))
          (text (if bounds
                    (buffer-substring (car bounds) (cdr bounds))
                  (read-string "Text: ")))
          (label (completing-read
                  "Label (default is incremented numerical label): " defined-labels
                  nil nil nil 'markdown-reference-label-history 
                  (next-markdown-label defined-labels)))
          (ref (markdown-reference-definition
                (concat "[" (if (> (length label) 0) label text) "]")))
          (url (unless ref (read-string "URL: ")))
          (title (when (> (length url) 0)
                  (read-string "Title (optional): "))))
    (when bounds (delete-region (car bounds) (cdr bounds)))
    (if image-p
        (markdown-insert-reference-image text label url title)
      (markdown-insert-reference-link text label url title))))

(defun next-markdown-label(reflabels)
  (number-to-string (+ 1 (max-list (mapcar 'string-to-number
                        (remove-if-not 'string-integer-p reflabels))))))

(defun max-list(list)
  (if (not list) 
      0
    (max (car list) (max-list (cdr list)))))

(defun string-integer-p (string)
    (if (string-match "\\`[-+]?[0-9]+\\'" string)
        t
      nil))

(defun my-markdown-mode-keys ()
  "Modify keymaps used by `markdown-mode'."
  (local-set-key (kbd "C-c C-a r") 'my-markdown-insert-reference-link-dwim)
  (local-set-key (kbd "C-c C-i r") 'my-markdown-insert-reference-image-dwim))
(add-hook 'markdown-mode-hook 'my-markdown-mode-keys)

(setq markdown-reference-location 'end)
(setq markdown-font-lock-support-mode nil)
```

## Creating and Finding your Drafts

Projectile lets you quickly find a file once you are _in_ your project.
Projectile even lets you quickly switch to a project from whichever
arbitrary buffer you happen to be editing (even outside a project).

Out of the box, however, Projectile does _not_ let me

* Create a new blog draft entry
* Switch to a particular blog draft entry that is currently in progress

So, I wrote some code to do it.  Here it is:

``` elisp
(setq pelican-root "~/blogging/website/")
(setq content-root (concat pelican-root "content/"))
(setq blog-root (concat content-root "blog/"))
(setq blog-exts "*.md")

(defun create-pelican-draft (content-path title)
  "Create draft blogging entry at path specified."
  (interactive "sContent path: \nsTitle: ")
  (find-file (concat blog-root content-path))
  (let ((date-str (format-time-string "%Y-%m-%d %H:%M:%S"))
        (draft-template "title: %s\ndate: %s\nmodified: %s\nstatus: draft\n\n"))
    (insert (format draft-template title date-str date-str))))

(defun switch-to-pelican-drafts ()
  "Switch to the global pelican blog project, and provide a list
of drafts to choose from"
  (interactive)
  (let ((grep-find-ignored-directories '())
        (grep-find-ignored-files '()))
    (grep-compute-defaults)
    (rgrep "status: draft" blog-exts pelican-root)))

(global-set-key (kbd "C-c w d") 'create-pelican-draft)
(global-set-key (kbd "C-c w s") 'switch-to-pelican-drafts)
```

The `create-pelican-draft` prompts you for the content path (relative to
your blog root) and the title from the minibuffer.  It will create the
buffer at the content path and insert the title as metadata upon creation.
You still have to save the buffer and commit the contents.

The `switch-to-pelican-drafts` function performs a simple grep at the root
of your blog to find the drafts.  The result is a standard Emacs grep buffer
window.  You visit the drafts the same way you would visit any other grep
hit, with the `next-error` function, usually bound to `C-x <backtick>`

## Updating Timestamps

My blog entries contain metadata at the top of the file, like so:

``` markdown
title: Blogging with Emacs and Pelican  
date: 2018-03-22 10:06:38  
modified: 2018-03-22 10:06:38  
status: draft  

Pelican is my blogging engine of choice these days...
```

The format is called [MultiMarkdown][11] [Metadata][12].  It's a markdown
extension.

Pelican uses the date header to order the blog entries. To my knowledge, it
doesn't really use the last modified date header for anything in particular,
but I like having it around just in case.

Notably, Pelican does _not_ actually provide a way to manage or otherwise
insert the dates automatically; it's completely up to you to insert an
appropriate date.

To this end, I've written some code to make the date insertions easy.  I
wrote one function to refresh both the post date and the last modified date,
and another to refresh just the last modified date.  I can't think of a use
case to refresh just the creation date without the last modified date, so I
don't have a function for that.  Here's the code:

``` elisp
(defun insert-header (header value)
  "This function will find the matching header in the header
block, delete the line, and leave the point there for inserting a
new header.  If no header exists it will position at the end of
the block.  If there is no header block, it will return nil,
otherwise non-nil"
  (save-excursion
    (goto-char 0)
    (let ((end-of-headers (search-forward "\n\n" (point-max) 't))
          (header-regex (format "^[ ]\\{0,3\\}%s:" header))
          (kill-whole-line 't))
      (if end-of-headers
          (progn
            (goto-char 0)
            (if (search-forward-regexp header-regex end-of-headers 't)
                (progn
                  (beginning-of-line)
                  (kill-line))
              (goto-char (- end-of-headers 1)))
            (insert (format "%s: %s\n" header value)))
        'nil))))

(defun refresh-dates ()
  "Refresh the date and modified date with the current date"
  (interactive)
  (let ((current-date (format-time-string "%Y-%m-%d %H:%M:%S")))
    (insert-header "date" current-date)
    (insert-header "modified" current-date)))

(defun refresh-modified-date ()
  "Refresh the modified date with the current date"
  (interactive)
  (let ((current-date (format-time-string "%Y-%m-%d %H:%M:%S")))
    (insert-header "modified" current-date)))

(global-set-key (kbd "C-c w m") 'refresh-modified-date)
(global-set-key (kbd "C-c w n") 'refresh-dates)
```

The code doesn't work if the header spans more than one line, but that
doesn't really happen very often in my workflow (certainly not for date
entries), so I haven't handled it.

## Conclusion

And that's about it.  With these modifications I have a fairly nice
environment for blogging when the mood strikes.

Part of the reason for streamlining this process was that I wanted to lessen
the friction I was feeling when creating new blog entries, in an attempt to
encourage myself to write more.  We'll see if I'm successful in this regard.

If you have any suggestions feel free to drop me a line - unless, of course,
your suggestion is "don't use Emacs or Pelican" in which case you will be
politely ignored :)


[1]: https://blog.getpelican.com
[2]: https://www.gnu.org/software/emacs/
[3]: https://git-scm.com/
[4]: https://github.com/bbatsov/projectile
[5]: https://magit.vc/manual/magit.html
[6]: https://magit.vc/manual/magit.html#Getting-Started
[7]: https://daringfireball.net/projects/markdown/
[8]: https://jblevins.org/projects/markdown-mode/
[9]: https://www.emacswiki.org/
[10]: https://github.com/
[11]: http://fletcherpenney.net/multimarkdown/
[12]: http://fletcher.github.io/MultiMarkdown-4/metadata.html
[13]: https://virtualenv.pypa.io/en/stable/
[14]: https://github.com/porterjamesj/virtualenvwrapper.el
[15]: https://github.com/jorgenschaefer/pyvenv
[16]: /2016/12/12/pelican-move
