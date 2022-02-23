---
date: 2018-09-02T20:03:25.000Z
modified: 2018-09-02T20:03:25.000Z
tags:
  - technical
  - angular
title: Angst and the Angular Module System
---

Google's [AngularJS][2] is one of the most popular web frameworks out there,
but it comes with its share of criticism (performance around two way binding
being one of the more prominent complaints).  Google responded by releasing
Angular 2 (and, as of now, 4, 5 and 6), which addressed some of the issues
but at the cost of being drastically incompatible with its predecessor, with
no realistic way to upgrade except through pure elbow grease.  Angular 2+
might as well be a completely different framework.

I never got the chance to become familiar with AngularJS, but my employer
did decide to take the plunge with Angular 2 on one of their newer projects
and so, for better or for worse, I am somewhat familiar how an Angular 2+
project works.

Aside from the rather gratuitous break with AngularJS, Angular 2+
(henceforth simply Angular) gets a lot of flak for being overly complicated.
I completely understand this reaction; the learning curve is dauntingly
high, especially compared to something like [React][3], which lets you write
a bare-bones web component with a lot fewer lines of code.  I suspect that
as your application gets more complicated, the differences between the two
frameworks mellow out, but I don't judge the initial impression.

Simply put, there's just a lot more *stuff* in Angular.

## Modules

One of the things that you need to learn to become proficient in Angular is
a concept called a *module*, to be quickly distinguished from standard
[Javascript modules][4], which are entirely different beasts.  It can be a
bit difficult to understand what an Angular module for, and how best to
organize code around it, so I decided to try and fill the gap.

Please note that this article assumes a certain degree of familiar with
Angular in general.  See the fine [documentation][1] if you wish to learn
more.

A module is used in Angular for several things, but in this article we're
mostly concerned with:

* defining what you can use in a template (declarations)
* configuring the dependency injection system (providers)

These two purposes can be somewhat at odds, as we will see.

One defines a module in Angular by declaring a class with an NgModule
decorator:

``` typescript
@NgModule({
    // stuff goes in here, to be explained later
})
export class SomeModule {
    // nothing goes in here
}
```

The class itself doesn't really do anything except provide a name for the
module.

For the most part, I will assume that the modules under discussion here are
are *eagerly loaded*, i.e. they are all processed at once during the
bootstrapping of the application.

Modules can be *lazily loaded* as well, meaning that they are processed some
time after the application has booted (if at all), usually in response to
route navigation.  These kinds of modules have their own set of rules,
particularly around dependency injection, which will be discussed later.


## Components and Templates

When you create a component in Angular (again, see the [docs][1] if you
don't know what these are), you are creating an entity with the intention of
using it in a HTML template somewhere else.

(Though we'll focus on components in this document, pipes and directives are
treated similarly in that they are entities which are used in templates).

When you create a component you define the HTML element with which it's
associated.  You instantiate that component by using its HTML element in the
template of some other component.

Before you can instantiate a component, you are required to declare it in a
module somewhere.  There are no such things as "floating" components in
Angular; if you want to use a component in a template somewhere, it needs to
be part of a module.  This means your application must have a minimum of one
module, and it must be explicitly declared.

So, for example, if you have a component called AwesomeComponent
(associated, let's say, with the `comp-awesome` HTML element), then
AwesomeComponent needs to be declared in a module somewhere (let's call it
AwesomeModule):

``` typescript
@Component({
    selector: 'comp-awesome',
    template: `put your template here`
})
export class AwesomeComponent {
}

@NgModule({
    declarations: [AwesomeComponent]
})
export class AwesomeModule {}
```

Declarations are *private* to the module in which they are defined.  So
while the `comp-awesome` element is now available for use in other
components in AwesomeModule it is, notably, *not* available for use in
components declared in other modules.

If you want to use AwesomeComponent *outside* of AwesomeModule - say, for
example, in CoolComponent which is declared in CoolModule - then you need
to:

* put AwesomeComponent in the *exports* field of AwesomeModule and
* put AwesomeModule in the *imports* field of CoolModule.

The modules in question now look like this:

``` typescript
@NgModule({
    declarations: [AwesomeComponent],
    exports: [AwesomeComponent]
})
export class AwesomeModule {}

@NgModule({
    declarations: [CoolComponent],
    imports: [AwesomeModule]
})
export class CoolModule {}
```

To summarize:

* the declarations field of a module shows which components *belong* to this
  module.  A component can belong to only one module.  A component can use
  another component within the same module.

* Components are by default private to the module (i.e they can only be
  instantiated by other components in the same module).  To use a component
  in *another* module you need to list the component in the exports field of
  the module to which it belongs.

* Using a component declared in another module's export field means
  importing it into every module where you want to use it.

Though not especially germane to the ongoing discussion, it's worth noting
in passing that an exports field can also contain other modules, in which
case you are exporting all the public things which the module is exporting.


## Dependency Injection, Modules and Providers

The second major use for modules is to configure Angular's Dependency
Injection (DI) system.  DI is a really big topic in Angular and I won't
pretend to cover it all here but, in essence, DI is how components receive
their service dependencies.  We say that a service is *injected* into a
component.  When you list a service class in a component's constructor, the
DI system will provide an instance of the service to the component so it can
use it.

For a service to be available for injection in some class, that service
needs to be configured in the *providers* field of a module somewhere (as
always, read the [docs][1] for the details of how this is done; the easiest
way is usually just to use the class name directly in the providers field):

``` typescript
@NgModule({
    ...
    providers: [AwesomeService]
})
export class AwesomeModule {}
```

Broadly speaking, if you don't do anything special, Angular services are
singletons.  You can override this behaviour by supplying a different kind
of provider.

The Angular docs like to describe the DI system as hierarchical, and it is,
but I find that thinking about it this way makes things more confusing when
you start off.  The hierarchical nature of the DI system only really comes
into play when you start using relatively advanced features like lazily
loaded modules or when you start overriding providers at the component level
(something that's often done only for testing purposes - I won't cover that
here).

In particular, developers sometimes assume that there exists some kind of
hierarchical relationship between importing and exporting modules, or that
providers and services are somehow scoped to the module in which they are
defined, like the declarations field.

That's not how it works. For the most part, it doesn't actually matter where
you declare your provider, though for your own sanity you'll of course want
to keep your provider fields relevant to the module you're defining
(i.e. don't declare AwesomeService in CoolModule when it really belongs in
AwesomeModule).  It doesn't matter because providers declared in a module
become global in scope (lazily loaded modules are an exception, as we will
soon see).  As long as the module in which your provider is defined is
imported *somewhere* in your application (or, alternatively, is configured
as the bootstrap application module), then your service is injectable
anywhere in your application.

Unless one is trying to write a re-usable Angular library, one doesn't
generally write "floating" application modules; as a rule, every module you
declare is meant to be, and will be, imported somewhere at least once (or
used as the application's bootstrap module).  The application's root module
will usually import core utility and feature modules, for example, which
will themselves import still other modules.  As far as the DI provider
system is concerned, all these imports are flattened into one global
injector at boot time, which is then used everywhere.

Once again, lazily loaded module are exceptional here; they aren't loaded at
boot time, but rather on demand.  It makes sense that they are not part of
the global injector.


## Declarations Versus Providers

You may have noted during the ongoing discussion that the declarations field
and the providers field are, in a sense, working against each other.  The
declarations field are for things (components, etc.) which are private to
the module.  The providers field, on the other hand, are for services which
are global in scope.

If your module contains nothing but declarations, then things are simple - you
just import the module wherever you want to use the component.

If your module contains nothing but providers, then things are also simple -
you'll want to import it in a module somewhere so it's available in your
application (often in your root/application module, or in a module which is
imported into your root/application module).  You will only need to import
it *once* because once configured in this fashion, a provider is global in
scope and you can use the service it provides anywhere in your application.

It's when your module contains both declarations and providers that things
become tricky, and it's here where we finally need to start making a
distinction between eagerly loaded modules and lazily loaded modules.

If all of your modules are eagerly loaded, then things are once again
simple.  You import your module where needed (because of your declarations
clause).  Your service providers field will be globally re-declared each
time you import, which is admittedly useless but more or less harmless.

It's when you start using lazily loaded modules that things become less
simple.


## Root Modules, Lazy Loading, and the forRoot() Convention

Angular allows you to lazy load modules.  The basic idea here is that you
can create a module that represents everything in a particular "page" or
"route" in the application.  The code is loaded only when the user navigates
to the route, and will never be loaded if the user doesn't visit the route.

By definition, a lazily loaded module is activated only when you navigate to
a particular route, not during the bootstrapping of the application.  This
means that a lazily loaded module will not participate in the configuration
of the DI system during boot time.  Instead, Angular creates a new injector
as needed when the new module is loaded.  The new injector is considered a
*child* of the root injector, and this is one of the ways that the DI's
hierarchy comes into play.

If your module only contains declarations, and no providers, then you don't
have to worry about this detail; you just import your module where needed,
including the lazily loaded one, and you're done.  You can import your
module into the root module as well with no issue.

It's when your module contains providers that possible issues arise. Angular
will create a new injector for the lazily loaded module when it's finally
loaded - when the application navigates to the associated route.  Since new
injector is considered a child of the root injector, any providers declared
here will override those declared on the root.  The practical upshot is that
you'll be provided with one instance of the service in your root module and
another instance in your lazily loaded module (if you import them in both).
Your service, in other words, won't be a singleton anymore.

For many services, this is irrelevant, because many services don't keep
state, and it doesn't matter if you use different instances in different
parts of your application.  For other kinds of services, particularly those
which keep state, it *definitely* matters, and you need to mitigate this
issue.  So what do you do?

Normally, what you would *like* to happen is for the lazily loaded module to
use the same service instance as your root.  If your module *only* contains
providers, this is easy - import the module *only* in your root module and
be done with it.  The lazily loaded module will use the root instance of the
service if it lacks a provider of its own.  This option isn't open to you,
however, if your module *also* contains declarations, since you need to
import those everywhere they're used.

This is where the forRoot() convention comes in.  When a module has a static
forRoot() method, it's meant to return a version of the module that contains
the declarations *and* the providers.  This version is meant to be imported
in the root module.  The standard, non-root version of the module (or,
sometimes, the version returned from a forChild() static method) will
contain just the declarations, and it's meant to be imported in lazily
loaded modules.  Since this version contains no providers, the lazily loaded
module will use the providers and services from the root module, and you'll
end up using the same service instances across your application.


``` typescript
@NgModule({
    // declarations and providers
    imports: [ModuleWithComponentsAndServices.forRoot()] 
})
export class EagerModule {}

@NgModule({
    // just declarations, no providers
    imports: [ModuleWithComponentsAndServices] 
})
export class LazyModule {}
```

Like I said...complicated, right?  Sometimes, I get the hate.

[1]: https://angular.io/docs
[2]: https://angularjs.org/
[3]: https://reactjs.org/
[4]: https://medium.freecodecamp.org/javascript-modules-a-beginner-s-guide-783f7d7a5fcc
