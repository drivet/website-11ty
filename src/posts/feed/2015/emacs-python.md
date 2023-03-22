---
date: 2015-08-20T05:48:47.000Z
modified: 2015-08-21T05:53:02.000Z
tags:
  - emacs
  - python
  - programming
title: Emacs and Python
---

I use Emacs as a python IDE.  The web already offers a [significant][13]
[number][14] of resources on the subject but I've made a couple of
specialized tweaks that I feel may be worth sharing.  So here we go.

Why am I doing this?  Well, the simple answer is that I like using Emacs.  I
do most of my writing in it.  I use it as my task manager and TODO list.  I
have, in the past, used it as my email client.  So it makes sense that I
would use it as my python development environment.  Your mileage may vary.

## Immediate goals

I am mainly a Java developer and Eclipse user at work.  Eclipse makes Java
programming somewhat tolerable.  I use the following features heavily:

 * automatic import management
 * jump to definition
 * auto completion
 * refactoring tools, like method and variable renaming
 * on-the-fly detection of compile errors and warnings.

A python IDE, for me, would have to have roughly the same feature set,
insofar as the dynamic nature of the language makes it possible.  It turns
out that this is possible in Emacs, with a little bit of elbow grease.  This
discussion focuses on Emacs 24, and assumes you have ELPA set up.

## Installing Rope

The first thing to do is to install [Rope][1], a python refactoring library
and, since it isn't Emacs specific, the *second* thing to do is to integrate
it with Emacs via a package called [ropemacs][2].  According to the github
page, ropemacs is an "Emacs mode that uses rope library to provide features
like python refactorings and code-assists".  Both packages are written in
python and can be installed easily via [pip][15].

Though these libraries are usually available via the package management
system of your favourite Linux distribution, and though I'm normally an
advocate of using a Linux distribution's packaging system wherever possible,
the versions on PyPI, installed via pip, are much more up-to-date and are much
more likely to work.  Plus, using pip makes it easier to use
[virtualenv][4], if that's your thing (which it should be, unless you're
using [docker][5], which is on my list of things to try out).

"But wait!", I hear you cry.  "Ropemacs is an Emacs mode, and it's written
in *python*?  What sorcery is this?".  No sorcery; ropemacs works via
another package called Pymacs, which provides an interface between Emacs and
a python process, allowing one to write Emacs extensions in python.

Pymacs has two pieces to it: an Emacs lisp piece and a python piece.  The
lisp piece is easy to install if you have ELPA set up.  The python piece,
however, is surprisingly difficult to install, mostly because of its archaic
build system.  I ended up installing it globally (so I wouldn't have to do
this for every virtualenv I manage) as follows:

``` bash
pip install -e "git+https://github.com/pinard/Pymacs.git#egg=Pymacs"
cd $VIRTUAL_ENV/src/pymacs
make
cp /root/src/pymacs/Pymacs.py /usr/local/lib/python2.7/dist-packages
```

Replace the `/usr/local/lib/python2.7/dist-packages` part by whatever your
equivalent is.

Finally, make sure you have this somewhere in your init.el file:

``` elisp
(require 'pymacs)
(pymacs-load "ropemacs" "rope-")
(setq ropemacs-enable-autoimport t)
(defun pymacs-reload-rope () 
    "Reload rope"
    (interactive)
    (pymacs-terminate-services )
    (pymacs-load "ropemacs" "rope-"))
```

## Dealing with virtualenvs

Most of my python projects are tucked away inside virtualenvs.  Using a
virtualenv means I can pip install any python package I want without
worrying about whether it's polluting my other projects.  It's basically a
way of isolating a project's dependencies.  You can install it, once again,
using pip.

Most of the time I don't use virtualenv directly, but rather a set of
extensions that operate a layer above called [virtualenvwrapper][7].  It's
also installable via pip.  With virtualenvwrapper, you get tools to easily
create, delete and list existing virtualenvs, as well a hook system that
lets you perform certain tasks during certain times.  I will have more to
say about that feature later.

Before you can use a virtualenv, you have to *activate* it.  The standard
way to do this with the virtualenvwrapper toolkit is to use the `workon`
alias from the command line.  Sometimes, though, you *already* have a python
process running (maybe it's an interactive environment), and you want to
activate the virtualenv from *within* the python process.  In such a case,
the standard virtualenvwrapper package includes a script called
`activate_this.py` which you can run from the python process as follows:

``` python
activate_this = '/path/to/env/bin/activate_this.py'
execfile(activate_this, dict(__file__=activate_this))
```

This will load the virtualenv located at /path/to/env into the currently
running python process.  Note that there is no equivalent way to
*deactivate* a virtualenv in this manner.

## Emacs and virtualenvs

Using virtualenvs complicates the Emacs setup, but there are ways to
mitigate this.  My solution of choice is to use [virtualenvwrapper.el][6],
which lets you switch between virtualenvs from within Emacs.  It basically
replicates, in Emacs, many of the features of virtualenvwrapper.

The virtualenvwrapper.el package handles a number of things for you; for
example, it will prepend the current virtualenv's bin directory to the PATH
environment variable so you can run any virtualenv local executable directly
from Emacs (for example, pep8, pylint, or nosetests).  It also comes with a
hook system, in the same spirit as its namesake.  Specifically - this will
be important later - it comes with a venv-postactivate-hook function and a
venv-predeactivate-hook function which get run just after a virtualenv is
activated and just before it's deactivated, respectively.

Unfortunately, virtualenvwrapper.el and ropemacs don't really play nice
together; as mentioned before, ropemacs works via Pymacs, and Pymacs works
by launching a single python process from within Emacs.  That python
process, with its attendant libraries, is usually *not* the one associated
to the virtualenv that Emacs is currently pointing to, which means that rope
features like "jump to definition" don't work properly when you have a
virtualenv activated.  You *could* launch your Emacs editor on the command
line once you've activated your virtualenv, but this is inconvenient if you
use Emacs for non-python work and you already have an instance running
before you start editing your python files.

Another possibility that comes to mind is to use the `activate_this.py`
method described before to activate the current virtualenv inside the Pymacs
process.  This *almost* works but, as noted before, there appears to be no
easy way, out of the box, to *deactivate* the virtualenv, which poses a
problem when you want to switch to another one.

The solution I came up with is a bit involved, but it's also fairly general.
It consists of two parts.

First, I use the postmkvirtualenv hook (run by virtualenvwrapper) to copy a
specialized `activate_this.py` script into the newly created virtualenv.  In
addition, I also copy, to the same location, a special `deactivate_this.py`
script which undoes the work of the previous script.  The hook looks like
this:

    `postmkvirtualenv`:

``` bash
#!/bin/bash
# This hook is sourced after a new virtualenv is activated.
cp $WORKON_HOME/custom_activate_this.py "${VIRTUAL_ENV}"/bin/activate_this.py
cp $WORKON_HOME/custom_deactivate_this.py "${VIRTUAL_ENV}"/bin/deactivate_this.py
```

Secondly, I run the `activate_this.py` script from the
venv-postactivate-hook in Emacs to prep the Pymacs python process, and I use
the venv-predeactivate-hook to run the `deactivate_this.py` script, undoing
the changes when I'm done.  The requisite code in my init.el file looks like
this:

``` elisp
(defun pymacs-activate-virtualenv () 
    (when (boundp 'python-shell-virtualenv-path)
        (let ((activate-file
                (concat python-shell-virtualenv-path "bin/activate_this.py"))) 
            (when (file-exists-p activate-file)
                (pymacs-exec (concat "execfile('" activate-file 
                            "', dict(__file__='" activate-file "'))"))))))

(defun pymacs-deactivate-virtualenv()
    (when (boundp 'python-shell-virtualenv-path)
        (let ((deactivate-file
                    (concat python-shell-virtualenv-path "bin/deactivate_this.py")))
            (when (file-exists-p deactivate-file)
                (pymacs-exec (concat "execfile('" deactivate-file 
                            "', dict(__file__='" deactivate-file "'))"))))))
                            
(add-hook 'venv-postactivate-hook 
        (lambda () (pymacs-activate-virtualenv)))

(add-hook 'venv-predeactivate-hook 
        (lambda () (pymacs-deactivate-virtualenv)))
```

The specialized `custom_activate_this.py` script is exactly the same as the
standard one, except that it remembers the paths that it changed, so that
the `custom_deactivate_this.py` can do its work later on.  They look like
this:

`custom_activate_this.py`:

``` python
try:
    __file__
except NameError:
    raise AssertionError(
        "You must run this like execfile('path/to/custom_activate_this.py',
        dict(__file__='path/to/activate_this.py'))")
import sys
import os

old_os_path = os.environ['PATH']
os.environ['PATH'] = os.path.dirname(os.path.abspath(__file__)) + \
                        os.pathsep + old_os_path
base = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if sys.platform == 'win32':
    site_packages = os.path.join(base, 'Lib', 'site-packages')
else:
    site_packages = os.path.join(base, 'lib', 'python%s' % sys.version[:3], \
                                'site-packages')
prev_sys_path = list(sys.path)
import site
site.addsitedir(site_packages)
sys.real_prefix = sys.prefix
sys.prefix = base
# Move the added items to the front of the path:
new_sys_path = []
for item in list(sys.path):
    if item not in prev_sys_path:
        new_sys_path.append(item)
        sys.path.remove(item)
sys.path[:0] = new_sys_path

# THIS IS NEW
# SAVE OLD VALUES FOR DEACTIVATE
sys.prev_sys_path = prev_sys_path
sys.old_os_path = old_os_path
```

`custom_deactivate_this.py`:

``` python
try:
    __file__
except NameError:
    raise AssertionError(
        "You must run this like execfile('path/to/custom_deactivate_this.py',
        dict(__file__='path/to/activate_this.py'))")
import sys
import os

if hasattr(sys, 'real_prefix'):
    sys.prefix = sys.real_prefix
    sys.path = sys.prev_sys_path
    os.environ['PATH'] = sys.old_os_path

    delattr(sys, 'real_prefix')
    delattr(sys, 'prev_sys_path')
    delattr(sys, 'old_os_path')
```

## Activating a virtualenv upon opening a file

It's useful to be able to automatically activate a virtualenv by opening any
file in the project.  I didn't do anything special here; i just followed the
advice given in the virtualenvwrapper.el README file, and I used a
.dir-locals.el in the relevant projects.  Put a .dir-local.el file in your
projects root directory, with the following contents:

``` elisp
((python-mode . ((project-venv-name . "myproject-env"))))
```

And make sure you have this somewhere in your init.el file:

``` elisp
(add-hook 'python-mode-hook (lambda ()
    (hack-local-variables)
        (when (boundp 'project-venv-name)
            (venv-workon project-venv-name))))
```

## Flymake, pylint, pep8

I use [flymake][8], coupled with [pylint][9] and [pep8][10], for on-the-fly
detection of errors and deviations from pep8 style conventions.

Getting the packages is easy.  Install flymake in Emacs via ELPA.  Install
pylint and pep8 via pip.

Flymake works by running a syntax checker tool against the current buffer,
and then parsing the output (which needs to be of a particular form) to mark
errors directly in said buffer.  Note that as of this writing, flymake is
unable to run more than one syntax checker at the same time, which means
that we can't run both pep8 and pylint together without jumping through some
hoops.  The "hoops", in this case, consist of combining the two syntax
checkers into a single script that can be run on its own.  It's not much to
look at, but it works:

`pycheckers`:

``` bash
epylint $1 -enable=C,R,I,W,E,F
pep8 $1
```

The `epylint` call is a variation on the standard pylint script which
formats its output in a flymake friendly manner.  It comes with the pylint
package.

Next, add the following to your init.el file:

``` elisp
(require 'flymake-cursor)
(defun my-flymake-mode-hook ()
    (local-set-key (kbd "C-.") 'flymake-goto-next-error)
    (local-set-key (kbd "C-,") 'flymake-goto-prev-error))
(add-hook 'flymake-mode-hook 'my-flymake-mode-hook)

(when (load "flymake" t)
    (defun flymake-pylint-init ()
        (let* ((temp-file (flymake-init-create-temp-buffer-copy
                    'flymake-create-temp-inplace))
                (local-file (file-relative-name
                    temp-file
                    (file-name-directory buffer-file-name))))
                (list "~/sandbox/bin/pycheckers" (list local-file))))
        
            (add-to-list 'flymake-allowed-file-name-masks
                '("\\.py\\'" flymake-pylint-init)))


;; enable flymake when loading python files
(defun my-python-mode-hook ()
    (flymake-mode t))
(add-hook 'python-mode-hook 'my-python-mode-hook)
```

Flymake-cursor is a package which displays the flymake message in the
minibuffer rather than a window.  You don't have to use it if you prefer the
window.

## Autocomplete and yasnippet

Ropemacs, via rope, has direct support for auto-completion.  Just explore the
menu.

I've opted instead to integrate the rope completion system into
[auto-complete][12], a general-purpose Emacs completion framework.  The
auto-complete package will provide suggestions for you as you type.

In addition, I've integrated [yasnippet][12], a templating system for Emacs.
Python, admittedly, is pretty terse, so the argument in favour of a
templating system is less strong here than for a language like, let's say,
Javascript, but I still find it occasionally useful.

Anyway, I added this to my init.el:

``` elisp
(require 'yasnippet)
(yas-global-mode 1)
(define-key yas-minor-mode-map (kbd "<tab>") nil)
(define-key yas-minor-mode-map (kbd "TAB") nil)
(define-key yas-minor-mode-map (kbd "C-SPC") 'yas-expand)

;; set up auto-complete
(require 'auto-complete)
(require 'auto-complete-config)
(ac-config-default)
(global-auto-complete-mode t)
(ac-flyspell-workaround)

(ac-ropemacs-initialize)

(add-hook 'python-mode-hook
        (lambda ()
        (add-to-list 'ac-sources 'ac-source-ropemacs)
        (add-to-list 'ac-sources 'ac-source-yasnippet)))
```

Note that I disabled tab completion of yasnippets in favour of C-SPC.  I was
finding that the tab key was interfering with the indentation function in
Emacs, so I switched it.  As always, your mileage may vary.

Furthermore, I've arranged for yasnippet abbreviations to be added to
auto-completion candidates.  So far it's been okay, but I may remove this
feature in the future.

## Further considerations

At this point I have what I consider to be a fairly decent python IDE.  It
does most of what I need it to do.

But there's always room for improvements.  Some specific ideas:

 * integration of test results with the Emacs compilation buffer, so you can
   jump directly to a failed test within Emacs
 * integration and display of test coverage results
 * integration of python documentation 

Anyway, I hope this has been useful.  Long live Emacs!

[1]: https://github.com/python-rope/rope
[2]: https://github.com/python-rope/ropemacs
[3]: https://github.com/pinard/Pymacs
[4]: https://virtualenv.pypa.io/en/latest/
[5]: https://www.docker.com/
[6]: https://github.com/porterjamesj/virtualenvwrapper.el
[7]: https://virtualenvwrapper.readthedocs.org/en/latest/
[8]: http://flymake.sourceforge.net/
[9]: http://www.pylint.org/
[10]: https://pypi.python.org/pypi/pep8
[11]: http://auto-complete.org/
[12]: https://github.com/capitaomorte/yasnippet
[13]: http://pedrokroger.net/configuring-emacs-python-ide/
[14]: http://www.enigmacurry.com/2008/05/09/emacs-as-a-powerful-python-ide/
[15]: https://pip.pypa.io/en/stable/
[16]: http://batsov.com/articles/2012/02/19/package-management-in-emacs-the-good-the-bad-and-the-ugly/
