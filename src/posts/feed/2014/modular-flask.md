---
date: 2014-10-16T05:01:35.000Z
modified: 2015-05-16T05:53:02.815Z
tags:
  - python
  - programming
title: Thoughts on Modularizing Flask Applications
---

I've been playing around with the [Flask][1] web framework for a while
now. It's the basis for [YAWT][2], the CMS/blogging system I'm currently
developing (mostly as an exercise in familiarizing myself with [Python][3])

With Flask, it's dead simple to get a very basic web application up and
running:

    :::python
    from flask import Flask

    app = Flask(__name__)

    @app.route("/")
    def hello():
        return "Hello World!"

    @app.route("/blah")
    def blah():
        return "Hello Blah!"

    if __name__ == "__main__":
        app.run()

Put this in a python file called `blah.py`, mark it as executable, and run it.
You should be able to access the web application on locahost:5000.  It does
not, admittedly, do anything even remotely interesting, but it's enough to
get the basic idea.

As your application gets bigger, Flask best practices suggest that you

 * break your application up into discrete modules
 * adopt the [application factory][4] pattern

Ideally you'd like to do *both*.  As it turns out, this is trickier than I
had anticipated.  This article documents what I've learned on the subject.

## Dividing the Application into Modules

First we can try breaking up the application into modules. In this vein,
we'd like to put the view functions (the ones with the @app.route
decorators) into a separate python file/module.  How do we do this?

The [docs][5] actually have a recipe for this. If you adapt the instructions
to the above case, you would do the following:

 * create a folder called blah
 * copy `blah.py` to `blah/__init__.py`
 * move the view functions into `views.py`, existing alongside your
   `__init__.py` file.
 * create a `runserver.py` alongside the new blah folder.

So your `blah/__init__.py` file would look like this:

    :::python
    from flask import Flask
    app = Flask(__name__)

    import blah.views

And your `views.py` would look like this:

    :::python
    from blah import app

    @app.route("/")
    def hello():
        return "Hello World!"

    @app.route("/blah")
    def blah():
        return "Hello Blah!"

And your `runserver.py` would look like this:

    :::python
    from blah import app
    app.run(debug=True)

You'll note that we've introduced a circular dependency (`__init__.py` imports
`views.py`, and `views.py` imports app from `__init__.py`). Most python
programmers would be uncomfortable with that, myself included, but the docs
acknowledge the weirdness and move on.

One possibility that the docs seem to gloss over, but which could be
explored, is to eschew decorators completely and use the equivalent method
calls on the Flask object. That would allow you to do something like this:

`__init__.py`:

    :::python
    from flask import Flask
    app = Flask(__name__)
    init_views(app)

`views.py`:

    :::python
    def init_views(app):
        app.add_url_rule('/', 'hello', hello)
        app.add_url_rule('/blah', 'blah', blah)

    def hello():
        return "Hello World!"

    def blah():
        return "Hello Blah!"

This works, and avoids circular dependencies, but it does mean that you miss
out on one of Flasks nicest features, which are the decorators. I don't
think it's something that a lot of people are willing to give up.

## Modules with Blueprints

Yet another way to do it is to define the URL rules in a [Blueprint][6].
Blueprints are Flask's way of breaking up an application into independent
pieces.  Blueprints can have URL rules, filters, static files, etc. in
basically any combination (it's possible, for example, to define a Blueprint
for the sole purpose of providing a set of static files).

We can define a single Blueprint for our application in a views file:

`views.py`:

    :::python
    blah_bp = Blueprint(__name__)

    @blah_bp.route("/")
    def hello():
        return "Hello World!"

    @blah_bp.route("/blah")
    def blah():
        return "Hello Blah!"

And then the main module file looks like this:

`__init__.py`:

    :::python
    from flask import Flask
    app = Flask(__name__)

    from views import blah_bp
    app.register_blueprint(blah_bp)

No circular imports, and we get to use decorators - seems like a win all
around.  Of course, if you wanted to structure your Blueprint into multiple
files, you can do that too, but you'd have to resort to circular imports
again.

## Modules and Application Factories

The value of Blueprints starts to become clear when we start talking about
application factories.

The simple way of instantiating a Flask application is by doing so directly
in a python file somewhere. That being said, the Flask docs actually
recommend creating the application object via an application factory
function.  So instead of this:

    :::python
    app = Flask(__name__)

You'd have something like this:

    :::python
    def create_app():
        app = Flask(__name__)
        return app

By doing so, you make unit testing a bit more convenient, and you allow for
the possibility of running more than one Flask application in the same
python process.

Using application factories complicates the process of setting up URL rules,
or using Flask based decorators in general.  In general, using the
decorators requires access to the app object, and you don't have it until
the `create_app()` call returns, which is too late. One obvious solution is
to define all your rules inside the create_app method, like so:

    :::python
    def create_app():
        app = Flask(__name__)

        @app.route("/")
        def hello():
            return "Hello World!"

        @app.route("/blah")
        def blah():
           return "Hello Blah!"

        return app

This works, and it's a method I've adopted in the past, but it always made
me a bit uncomfortable.  It's just weird to me that I have to nest my
routing functions inside another function. And what if you want to define
the URL rules in another module?

Blueprints help solve the problem.  You define your Blueprint in another
file and import it *after* the application object is created, so you can
register it on the application, in much the same fashion as the non-factory
scenario:

`__init__.py`:

    :::python
    def create_app():
        app = Flask(__name__)
        from views import blah_bp
        app.register_blueprint(blah_bp)

`views.py`:

    :::python
    blah_bp = Blueprint(__name__)

    @blah_bp.route("/")
    def hello():
        return "Hello World!"

    @blah_bp.route("/blah")
    def blah():
        return "Hello Blah!"

## Application Factories and Configuration

As noted in the docs, one downside to the above approach is that the
application object - and hence the configuration - is not available for use
in the Blueprint during import time. You can't use `current_app` to access
the application, because you don't have an application context at this point
(more on that later).  And you can't use the app object itself since it's
tucked away in the create_app method and you can't see it in the `views.py`
module.

But what if you need it? What if, for example, you want to use a piece of
configuration, let's say the `static_url_path`, during the creation of the
Blueprint?

I suspect this is a scenario that does not come up very often, since I found
very little information on how to do this.  I eventually came up with the
following, but it only works on Flask 0.9 and above:

`__init__.py`:

    :::python
    def create_app():
        app = Flask(__name__)
        with app.app_context():
            from views import blah_bp
            app.register_blueprint(blah_bp)

`views.py`:

    :::python
    static_url_path = current_app.config['STATIC_URL_PATH']
    blah_bp = Blueprint(__name__, static_url_path = static_url_path)

    @blah_bp.route("/")
    def hello():
        return "Hello World!"

    @blah_bp.route("/blah")
    def blah():
        return "Hello Blah!"

The `app.app_context()` call creates what's called an
[application context][7]. Application contexts are a somewhat mystifying
concept in Flask and what follows is my best attempt at an explanation (to
be taken with a very large grain of salt).

Basically, a Flask application can exist in one of two states:

 * the setup state, where we create the application object and we haven't
   started processing any requests yet.
 * the request handling state, where we're processing a request.

When we're processing a request, certain global variables become available,
like `request` and `current_app`. These are the standard mechanisms by which
request processing code gets access to the request or the application
object. These variables become available because the processing of a request
is implicitly associated with the creation of a request context, a kind of
scratch pad that isn't shared between threads and where you can stash items
that are only valid for the duration of the request.

Notably, these global variables are *not* implicitly available during the
setup phase because we don't have a request context.  It makes sense - we're
not actually processing a request so why would there be a request context?

In older versions of Flask, you could make those variables available during
the setup phase by manually creating a request context.  That still works in
newer versions of Flask, but if all you're after is the `current_app`
variable (say, because you want access to the configuration object), then
you have a cheaper option - you can manually create an application context
instead. It's the existence of the application context that makes the
`current_app` variable accessible.

Note that in the newer versions of Flask, creating a request context
implicitly creates an application context as well, so the `current_app`
variable is still available just by creating a request context.

One thing that trips people up is the fact that an application context is
not actually scoped to the application - each request gets their own
application context.  Application contexts act as a kind of extension to the
request context, an extension that can stand on its own without the request
context, but not the other way around.

Anyway, to make a long story short, you need one of these application
contexts around for the `current_app` special variable to work. You
automatically have an application context provided for you when you process
a request, but outside of a request (like when you're creating the
Blueprint) you don't have one implicitly and it's up to you to create one
explicitly if you want one.  The `app.app_context()` call will do that for
you.

[1]: http://flask.pocoo.org/
[2]: http://github.com/drivet/yawt
[3]: http://www.python.org/
[4]: http://flask.pocoo.org/docs/0.10/patterns/appfactories/
[5]: http://flask.pocoo.org/docs/0.10/patterns/packages/
[6]: http://flask.pocoo.org/docs/0.10/blueprints/
[7]: http://flask.pocoo.org/docs/0.10/appcontext/
