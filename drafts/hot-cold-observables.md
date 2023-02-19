---
date: 2019-07-02 12:00:00-04:00
post-status: draft
title: Hot and Cold Observables
---

You can call publish() on an observable.  This returns a new Observable
(with the old Observable as a source) which has a connect() function
attached to it.

What this means is that the new Observable doesn't subscribe to the source
in its constructor.  And multiple observers can subscribe to the new
connectable Observable and nothing will happen.  The subscription is delayed
until a moment of your choosing.

When you call connect(), then the new Observable will subscribe to the
source Observable and pass along the values to the subscribers, if there are
any.

In rxjs, you have the following operators:

* multicast
* publish
* publishBehaviour
* publishReplay
* publishLast

The publish operator is just shorthand for multicast(new Subject()) and the
other flavours of publish are the same except they delegate to a different
type of Subject (publishLast delegating to AsyncSubject; note that this
isn't used very often)

## BehaviorSubject versus ReplaySubject

BehaviourSubject remembers the last item emitted by the source, and will
emit that to subscribers until the source emits a new item.

ReplaySubject caches the source items, so that subscribers will always see
the same sequence of items.  You can usually choose how many items to cache.

So, if you choose to cache just 1 item in a ReplaySubject, how does it
differ from a BehaviorSubject?  The difference is that a ReplaySubject won't
emit anything until the source emits at least one item.  A BehaviourSubject
starts life with an initial value which will be emitted until the source
Observable emits something.  In this way a BehaviourSubject can model a
variable which changes over time.
