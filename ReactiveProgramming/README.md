# The introduction to Reactive Programming you've been missing #

The hardest part of the learning journey is **thinking in Reactive**. It's a lot about letting go of old imperative and stateful habits of typical programming, and forcing your brain to work in a different paradigm.

> 最难的还是学习一个新范式，在一定程度上抛弃原有的命令式思维
>
> 如何以响应式的方式来思考

## 1. "What is Reactive Programming?" ##

能在网络上找到的定义：

+ 维基百科

  In computing, reactive programming is an asynchronous programming paradigm concerned with data streams and the propagation of change. This means that it becomes possible to express static (e.g. arrays) or dynamic (e.g. event emitters) data streams with ease via the employed programming language(s), and that an inferred dependency within the associated execution model exists, which facilitates the automatic propagation of the change involved with data flow.

+ 微软

  The Reactive Extensions (Rx) is a library for composing asynchronous and event-based programs using observable sequences and LINQ-style query operators. Using Rx, developers represent asynchronous data streams with Observables, query asynchronous data streams using LINQ operators, and parameterize the concurrency in the asynchronous data streams using Schedulers. Simply put, Rx = Observables + LINQ + Schedulers.

事实上，对于一个新手来说：定义往往不能告诉他们这是什么；我认为更好的介绍一个新概念的方式是：它具备什么样的特点 -> 这些特点导致了什么样的结果 -> 所以适用于什么样的场景

### 1.1asynchronous data streams ###

Reactive programming is programming with asynchronous data streams.

> 响应式编程就是操作异步数据流的方式
>
> 重点：操作异步数据流

不妨回想一下：在命令式编程里，我们操作的对象是什么？数据结构（内建数据结构 / 自定义的数据结构）

再想想看：在面向对象编程里，我们操作的对象是什么？对象（object），或者认为是以有限的方式（只能使用暴露出来的方法）去操作数据结构

作者在此处提出了一个很重要的概念：“流”；那么什么是“流”，它和我们常常简单的`int x`有什么区别？`array of int`是不是“流”？

In a way, this isn't anything new. Event buses or your typical click events are really an asynchronous event stream, on which you can observe and do some side effects. Reactive is that idea on steroids. You are able to create data streams of anything, not just from click and hover events. Streams are cheap and ubiquitous, anything can be a stream: variables, user inputs, properties, caches, data structures, etc. For example, imagine your Twitter feed would be a data stream in the same fashion that click events are. You can listen to that stream and react accordingly.

> 事件总线 / 常见的点击事件都是异步事件流，你可以监听它们并作出一些反应（这些反应通常是具备副作用的）
>
> 响应式编程扩展了异步事件流的概念：用任何东西都可以创建异步事件流
>
> 任何东西包括：变量、用户输入、属性、缓存、数据结构

用任何东西都可以创建异步事件流（异步事件流是一等公民？）

### 1.2 combine, create and filter ###

On top of that, you are given an amazing toolbox of functions to combine, create and filter any of those streams.

> 最重要的是，你会拥有一些令人惊艳的函数去结合、创建和过滤任何一组数据流

That's where the "functional" magic kicks in. A stream can be used as an input to another one. Even multiple streams can be used as inputs to another stream. You can *merge* two streams. You can *filter* a stream to get another one that has only those events you are interested in. You can *map* data values from one stream to another new one.

> 我们的老朋友：merge / filter / map

![1](1.png)



