# 安装 #

1. 下载相应版本的 valgrind 并解压
2. apt-get install build-essential
3. cd valgrind
4. ./configure --prefix=/where/you/want/to/install
5. make && make install

# Using and understanding the Valgrind core #

Your program is then run on a synthetic CPU provided by the Valgrind core. As new code is executed for the first time, the core hands the code to the selected tool. The tool adds its own instrumentation code to this and hands the result back to the core, which coordinates the continued execution of this instrumented code.

 valgrind 仍然是一个进程虚拟机



# 数据竞争 #

```c
#include <pthread.h>

int var = 0;

void* child_fn ( void* arg ) {
   var++; /* Unprotected relative to parent */ /* this is line 6 */
   return NULL;
}

int main ( void ) {
   pthread_t child;
   pthread_create(&child, NULL, child_fn, NULL);
   var++; /* Unprotected relative to child */ /* this is line 13 */
   pthread_join(child, NULL);
   return 0;
}
```

```shell
gcc race.c -o race -lpthread
g++ race.c -o race -static -lpthread
```

```sh
/home/valgrind/bin/valgrind --tool=helgrind ./race
```

Two important parts of the message are:

1. 两个栈
2. 尝试给出冲突的符号名称（在编译期间加上 -ggdb 选项有助于 valgrind 给出符号信息）

## happens-before relation ##

这是一个非常类似于 sql 冲突可串行化的概念：即使这些 operations 实际上是并行或者以一种混乱的顺序发生的，但从最后的结果看，它们一定会满足某个 DAG

> In [computer science](https://en.wikipedia.org/wiki/Computer_science), the **happened-before** [relation](https://en.wikipedia.org/wiki/Binary_relation) (denoted: →) is a relation between the result of two events, such that if one event should happen before another event, the result must reflect that, even if those events are in reality executed out of order (usually to optimize program flow). This involves [ordering](https://en.wikipedia.org/wiki/Partially_ordered_set) events based on the potential [causal relationship](https://en.wikipedia.org/wiki/Causal_relationships) of pairs of events in a concurrent system, especially [asynchronous](https://en.wikipedia.org/wiki/Asynchronous_communication) [distributed systems](https://en.wikipedia.org/wiki/Distributed_systems). It was formulated by [Leslie Lamport](https://en.wikipedia.org/wiki/Leslie_Lamport). In Java specifically, a **happens-before** relationship is a guarantee that memory written to by statement A is visible to statement B, that is, that statement A completes its write before statement B starts its read.

1. 传递性
2. 非对称性
3. 反自反性

一般而言，一种语言会保证几个基本的 happens-before relations ，举例来说：

1. 线程的创建先于线程的销毁
2. 对象的创建先于对象的销毁
3. 锁的 lock 操作先于 unlock 操作
4. volatile 关键字
5. ……

不同语言的内存模型带来的基础 happends-before relations + 传递性 = 整个程序的 happens-before relation

那竞争问题可不可以在非运行期静态检测？

1. 理论上是可以的
2. Helgrind 是动态的还是静态的？

## 冲突检测算法 ##

Helgrind's algorithm is (conceptually) very simple. It monitors all accesses to memory locations. If a location -- in this example, `var`, is accessed by two different threads, Helgrind checks to see if the two accesses are ordered by the happens-before relation. If so, that's fine; if not, it reports a race.

1. 动态的
2. monitors all accesses to memory locations
3. 如何去判断两个线程在访问数据时是否具备 happens-before relation ？

从系统调用的角度来说，有这么几条 happens-before relations ：

1. When a mutex is unlocked by thread T1 and later (or immediately) locked by thread T2, then the memory accesses in T1 prior to the unlock must happen-before those in T2 after it acquires the lock.
2. The same idea applies to reader-writer locks, although with some complication so as to allow correct handling of reads vs writes.
3. When a condition variable (CV) is signalled on by thread T1 and some other thread T2 is thereby released from a wait on the same CV, then the memory accesses in T1 prior to the signalling must happen-before those in T2 after it returns from the wait. If no thread was waiting on the CV then there is no effect.
4. If instead T1 broadcasts on a CV, then all of the waiting threads, rather than just one of them, acquire a happens-before dependency on the broadcasting thread at the point it did the broadcast.
5. A thread T2 that continues after completing sem_wait on a semaphore that thread T1 posts on, acquires a happens-before dependence on the posting thread, a bit like dependencies caused mutex unlock-lock pairs. However, since a semaphore can be posted on many times, it is unspecified from which of the post calls the wait call gets its happens-before dependency.
6. For a group of threads T1 .. Tn which arrive at a barrier and then move on, each thread after the call has a happens-after dependency from all threads before the barrier.
7. A newly-created child thread acquires an initial happens-after dependency on the point where its parent created it. That is, all memory accesses performed by the parent prior to creating the child are regarded as happening-before all the accesses of the child.
8. Similarly, when an exiting thread is reaped via a call to `pthread_join`, once the call returns, the reaping thread acquires a happens-after dependency relative to all memory accesses made by the exiting thread.

In summary: Helgrind intercepts the above listed events, and builds a directed acyclic graph represented the collective happens-before dependencies. It also monitors all memory accesses.

具体的手段有可能是 hook ，或者建立一个进程虚拟机

If a location is accessed by two different threads, but Helgrind cannot find any path through the happens-before graph from one access to the other, then it reports a race.

There are a couple of caveats:

1. Helgrind doesn't check for a race in the case where both accesses are reads. That would be silly, since concurrent reads are harmless.
2. Two accesses are considered to be ordered by the happens-before dependency even through arbitrarily long chains of synchronisation events. For example, if T1 accesses some location L, and then `pthread_cond_signals` T2, which later `pthread_cond_signals` T3, which then accesses L, then a suitable happens-before dependency exists between the first and second accesses, even though it involves two different inter-thread synchronisation events.

## Hints and Tips for Effective Use of Helgrind ##

1. Make sure your application, and all the libraries it uses, use the POSIX threading primitives. Helgrind needs to be able to see all events pertaining to thread creation, exit, locking and other synchronisation events. To do so it intercepts many POSIX pthreads functions.

   不能使用非 POSIX 的线程操作，这本身不是非常现实

   1. 监测的层次能不能低一点？直接检测系统调用？
   2. 监测的层次能不能高一点？让用户指定某个函数是什么类型的锁？自定义原子 happens-before relation
   3. 如果使用非 POSIX 的线程操作，可以使用[宏](http://valgrind.org/docs/manual/hg-manual.html#hg-manual.lock-orders)挽救一下

2. Avoid POSIX condition variables.

3. If your application is using thread local variables, helgrind might report false positive race conditions on these variables, despite being very probably race free. On Linux, you can use `--sim-hints=deactivate-pthread-stack-cache-via-hack` to avoid such false positive error messages (see [--sim-hints](http://valgrind.org/docs/manual/manual-core.html#opt.sim-hints)).

4. Round up all finished threads using `pthread_join`.

5. ……

很多问题是因为 Helgrind 收集信息不完全导致的，有空的话可以自己调整一下 Helgrind

## Helgrind Command-line Options ##

1. `--free-is-write=no|yes [default: no]`

   打开这个选项会在 free 操作之前假装有一次 write 操作，但此项是实验项目

2. `--history-level=none|approx|full [default: full]`

# 内存安全 #

```shell
valgrind --tool=memcheck ls -l
```