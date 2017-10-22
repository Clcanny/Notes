# Context #

## Overview ##

**Boost.Context** is a foundational library that provides a sort of cooperative multitasking on a single thread. 

> **Boost.Context**是协程的基础库

By providing an abstraction of the current execution state in the current thread, including the stack (with local variables) and stack pointer, all registers and CPU flags, and the instruction pointer, a execution context represents a specific point in the application's execution path.

> 这个基础库提供了哪些基础功能呢？
>
> 对线程执行环境的抽象：栈／栈指针／寄存器／CPU flags／指令寄存器
>
> 指令指针寄存器存储的是下一个时钟周期将要执行的指令所在的程序存储器地址

#### IR vs PC

程序的机器码（也就是二进制代码）存储在程序存储器（也就是内存中，对应着可执行文件的代码段）

每一个取指周期到来，CPU从指令指针指向的地址中取出一条指令，并将指令指针加1（指向下一条指令的地址）；如果遇到跳转指令，将会修改指令指针到要跳转的地址

来自StackOverflow的提问（其实我也一直不能区分PC／IR，刚好答疑）：

> Program counter holds the address of the instruction that should be executed next, while instruction register holds the actual instruction to be executed. wouldn't one of them be enough?
>
> And what is the length of each one of these registers?

> You will need both always. The program counter (PC) holds the address of the next instruction to be executed, while the instruction register (IR) holds the encoded instruction. Upon fetching the instruction, the program counter is incremented by one "address value" (to the location of the next instruction). The instruction is then decoded and executed appropriately.

PC／IR 都是需要的，它们有各自的作用；PC告诉我们下一条要执行的指令的地址；IR告诉我们“编码”过的指令

直到取指指令的到来，PC增加一条指令长度（增加的数可能是不定的，因为不同指令的长度是不一样的）；此时IP解码指令并且放到CPU中执行？

> The reason why you need both is because if you only had a program counter and used it for both purposes you would get the following troublesome system:

如果没有IR，你会遇到这些问题：

> [Beginning of program execution]
>
> + PC contains 0x00000000 (say this is start address of program in memory)
> + Encoded instruction is fetched from the memory and placed into PC.
> + The instruction is decoded and executed.
> + Now it is time to move onto the next instruction so we go back to the PC to see what the address of the next instruction is. However, we have a problem because PC's previous address was removed so we have no idea where the next instruction is.

说穿了就是一个很简单的问题，如果指令的获取和解码在PC中进行，那么会把下一条指令的地址覆盖掉

> Therefore, we need another register to hold the actual instruction fetched from memory. Once we fetch that memory, we increase PC so that we know where to fetch the next instruction.

> P.S. the width of the registers varies depending on the architecture's word size. For example, for a 32-bit processor, the word size is 32-bits. Therefore, the registers on the CPU would be 32 bits. Instruction registers are no different in dimensions. The difference is in the behavior and interpretation. Instructions are encoded in various forms, however, they still occupy a 32-bit register. For example, the Nios II processor from Altera contains 3 different instruction types, each encoded differently.

再多说一句，不同的指令集对于指令长度是否可变这个问题有不同的处理方式

对于那些具有可变长度指令的指令集，PC增加的数就不会是一个固定的数

最后，可以粗略地理解：PC = IP

——————这是一条结束**IR vs PC**的分界线——————

This is useful for building higher-level abstractions, like *coroutines*, *cooperative threads (userland threads)* or an equivalent to [C# keyword *yield*](http://msdn.microsoft.com/en-us/library/9k7k7cf0%28v=vs.80%29.aspx) in C++.

> 这种抽象对于建立一些更高层次的抽象（比如说合作式线程／协程）是很有用的

[*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc)/[*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) provides the means to suspend the current execution path and to transfer execution control, thereby permitting another context to run on the current thread.

> callcc提供了一种挂起当前执行路径并切换控制流的方法，因此它允许当前线程去执行一个完全不相关的控制流

This state full transfer mechanism enables a context to suspend execution from within nested functions and, later, to resume from where it was suspended.

> 这种“状态完全转移”的机制允许从函数内部（即函数还没有执行完退出之前）挂起，然后在某个时刻从被挂起的地方恢复

While the execution path represented by a [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) only runs on a single thread, it can be migrated to another thread at any given time.

> 这种“状态完全转移”的机制甚至能把被挂起的执行路径／控制流转移到另一个线程去执行

A [context switch](http://en.wikipedia.org/wiki/Context_switch) between threads requires system calls (involving the OS kernel), which can cost more than thousand CPU cycles on x86 CPUs. By contrast, transferring control vias [*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc)/[*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) requires only few CPU cycles because it does not involve system calls as it is done within a single thread.

> context switch对操作系统是不可见的，不通过系统调用即可完成，所以会快很多

In order to use the classes and functions described here, you can either include the specific headers specified by the descriptions of each class or function, or include the master library header:

```c++
#include <boost/context/all.hpp>
```

which includes all the other headers in turn.

## Requirements ##

If **Boost.Context** uses fcontext_t (the default) as its implementation, it must be built for the particular compiler(s) and CPU architecture(s) being targeted. Using [*fcontext_t*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc/implementations__fcontext_t__ucontext_t_and_winfiber.html#implementation), **Boost.Context** includes assembly code and, therefore, requires GNU as and GNU preprocessor for supported POSIX systems, MASM for Windows/x86 systems and ARMasm for Windows/arm systems.

> Context依赖的其中一个组件是平台相关的

```shell
docker run -t -i --name boost165 vimcpp:latest /bin/bash
apt-get install wget
mkdir boost && cd boost
wget -O boost_1_65_1.tar.gz https://sourceforge.net/projects/boost/files/boost/1.65.1/boost_1_65_1.tar.gz/download
tar -zxvf boost_1_65_1.tar.gz
cd boost_1_65_1
apt-get install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev
./bootstrap.sh --prefix=/usr/local
user_configFile=`find $PWD -name user-config.jam`
echo "using mpi ;" >> $user_configFile
n=`cat /proc/cpuinfo | grep "cpu cores" | uniq | awk '{print $NF}'`
./b2 --with=all -j $n install 
sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/local.conf'
ldconfig
```

## Context switching with call/cc ##

*call/cc* (call with current continuation) is a universal control operator (well-known from the programming language Scheme) that captures the current continuation as a first-class object and pass it as an argument to another continuation.

> call/cc 是一个通用的控制操作符，把当前的continuation打包成一等公民传给另一个continuation

A continuation (abstract concept of functional programming languages) represents the state of the control flow of a program at a given point in time.

> continuation代表着：任给一个时刻，一个程序的控制流的状态

Continuations can be suspended and resumed later in order to change the control flow of a program.

Modern micro-processors are registers machines; the content of processor registers represent a continuation of the executed program at a given point in time.

> 在现代微处理器的架构下，控制流状态是由寄存器状态决定的

Operating systems simulate parallel execution of programs on a single processor by switching between programs (context switch) by preserving and restoring the continuation, e.g. the content of all registers.

> 操作系统的多线程在很多年前就是通过保存／切换寄存器来做到控制流切换（线程切换）

### callcc() ###

[*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) is the C++ equivalent to Scheme's *call/cc* operator. It captures the current continuation (the rest of the computation; code after [*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc)) and triggers a context switch.

> callcc in C++ = call/cc in Scheme
>
> 捕获当前的continuation（剩下的计算）+ 触发环境切换

The context switch is achieved by preserving certain registers (including instruction and stack pointer), defined by the calling convention of the ABI, of the current continuation and restoring those registers of the resumed continuation.

> 环境切换是通过保存寄存器来实现的（可以保留所有寄存器，也可以按照调用约定保存关键寄存器）
>
> 有一些通用寄存器是不需要保存的（从原来的内存中load进来 vs 从context中load进来）

The control flow of the resumed continuation continues. The current continuation is suspended and passed as argument to the resumed continuation.

> 挂起的continuation被打包成一个参数传递给唤醒的continuation（这样，唤醒的continuation可以再次唤醒当前continuation）
>
> 话说，你可以认为协程在实现上不过是多了一个调度器continuation，其他continuation只和这个调度器continuation交互（也就是所有的其它continuation都被调度器continuation唤醒，其它的continuation也只唤醒调度器continuation）
>
> 所以，如果语言本身支持CPS（或者说是call/cc），写一个协程都不是事！

[*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) expects a *context-function* with signature `'continuation(continuation && c)'`. The parameter `c` represents the current continuation from which this continuation was resumed (e.g. that has called [*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc)).

> callcc期望得到一个context-function（带环境的函数？干脆翻译成函数状态机？），context-function的类型签名是continuation(continuation && c)，其中参数c代表着现在的continuation

On return the *context-function* of the current continuation has to specify an [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) to which the execution control is transferred after termination of the current continuation.

> 从当前continuation跳转到context-function的时候，需要告诉context-function：你结束后应该往什么地方走，你自己的控制流结束后需要往什么地方跳转

If an instance with valid state goes out of scope and the *context-function* has not yet returned, the stack is traversed in order to access the control structure (address stored at the first stack frame) and continuation's stack is deallocated via the *StackAllocator*.

> 这一段我就不太懂了（喵喵喵？），我无法理解变量为什么会在context-function还没返回的时候就跑出作用域，从而需要被销毁？
>
> 不过至少得到一个有用的信息：控制结构（control structure）存储在第一个栈帧（也就是第一个压入栈帧的变量？所以对它的访问只能通过指针越界，而不能从context内部显式访问？）

### continuation ###

[*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) represents a continuation; it contains the content of preserved registers and manages the associated stack (allocation/deallocation). [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) is a one-shot continuation - it can be used only once, after calling *continuation::resume()* or *continuation::resume_with()* it is invalidated.

> continuation（此处指一个类型或者是一个对象）代表着一个continuation（此处指控制流状态）
>
> continuation是只能使用一次的对象（否则可以对同一个控制流恢复多次，很容易出问题）
>
> 那么，continuation必然禁止拷贝复制，只能按照移动语义移动

[*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) is only move-constructible and move-assignable.

> 拷贝构造和赋值构造之类的就不要想了，不存在的（借用C++11的左值右值可以很简单禁用某些成员函数）

As a first-class object [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) can be applied to and returned from a function, assigned to a variable or stored in a container.

> 一等公民啊！同学们！
>
> 控制流是一等公民啊！至函数后，欢迎控制流加入一等公民俱乐部！

A continuation is continued by calling `resume()`/`resume_with()`.

### Usage ###

```c++
namespace ctx=boost::context;
int a;
ctx::continuation source=ctx::callcc(
    [&a](ctx::continuation && sink){
        a=0;
        int b=1;
        for(;;){
            sink=sink.resume();
            int next=a+b;
            a=b;
            b=next;
        }
        return std::move(sink);
    });
for (int j=0;j<10;++j) {
    std::cout << a << " ";
    source=source.resume();
}
```

这段代码比AnalyseCoroutinesSwitch中的Boost.Context的代码要好理解多了

+ ctx::continuation&& 表达了continuation类型的右值（continuation是不能使用两次的，右值传递可以保证外面的程序无法对它进行操作）
+ sink代表了某一个continuation，每次到for循环里，都会让这个continuation执行一次
+ sink不会代表lambda函数在某一时刻的continuation，否则lambda函数内调用sink.resume()就没有意义了
+ sink.resume()会使得sink失效，所以需要为它赋一个新值：sink = sink.resume()
+ sink非常有可能代表着main函数在某个时刻的continuation
+ 不难猜出，source代表着lambda在某个时刻的continuation

This simple example demonstrates the basic usage of *call/cc* as a *generator*. The continuation `sink` represents the *main*-continuation (function `main()`). `sink` is captured (current-continuation) by invoking [*callcc()*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc)and passed as parameter to the lambda.

> 果然越丑的代码越好理解（嘿嘿嘿），这个generator比AnalyseCoroutinesSwitch里面的那个好理解多了
>
> 因为两个continuation是通过“全局变量”来传递值的（lambda函数采取了引用捕获），这不是真正意义上的generator（所以说比AnalyseCoroutinesSwitch里面的那个generator丑多了）

Because the state is invalidated (one-shot continuation) by each call of *continuation::resume()*, the new state of the [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc), returned by *continuation::resume()*, needs to be assigned to `sink` after each call.

The lambda that calculates the Fibonacci numbers is executed inside the continuation represented by `source`. Calculated Fibonacci numbers are transferred between the two continuations via variable `a` (lambda capture reference).

> 这次倒是可以真正地理解上面两段话：lambda的环境是source，main的环境是sink

The locale variables `b` and `next` remain their values during each context switch. This is possible due `source` has its own stack and the stack is exchanged by each context switch.

> 局部变量的存在进一步证明了lambda函数具有自己的环境（尤其是栈）

### Parameter passing ###

Data can be transferred between two continuations via global pointers, calling wrappers (like `std::bind`) or lambda captures.

> 进程共享变量／线程共享变量／continuation共享变量

```c++
namespace ctx=boost::context;
int i=1;
ctx::continuation c1=callcc([&i](ctx::continuation && c2){
            std::printf("inside c1,i==%d\n",i);
            i+=1;
            return c2.resume();
        });
std::printf("i==%d\n",i);
```

`callcc(<lambda>)` enters the lambda in continuation represented by `c1` with lambda capture reference `i=1`. The expression `c2.resume()` resumes the continuation `c2`. On return of `callcc(<lambda>)`, the variable `i` has the value of `i+1`.

### Exception handling ###

If the function executed inside a *context-function* emits ans exception, the application is terminated by calling `std::terminate()`. `std::exception_ptr` can be used to transfer exceptions between different continuations.

> 一场可以在不同的continuation间传递，这个厉害了

Do not jump from inside a catch block and then re-throw the exception in another continuation.

### Executing function on top of a continuation ###

Sometimes it is useful to execute a new function on top of a resumed continuation. For this purpose *continuation::resume_with()* has to be used. The function passed as argument must accept a rvalue reference to [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) and return `void`.

> 是不是看不懂这个在说什么？没关系！

```c++
namespace ctx=boost::context;
int data=0;
ctx::continuation c=ctx::callcc([&data](ctx::continuation && c) {
                    std::cout << "f1: entered first time: " << data << std::endl;
                    data+=1;
                    c=c.resume();
                    std::cout << "f1: entered second time: " << data << std::endl;
                    data+=1;
                    c=c.resume();
                    std::cout << "f1: entered third time: " << data << std::endl;
                    return std::move(c);
                });
std::cout << "f1: returned first time: " << data << std::endl;
data+=1;
c=c.resume();
std::cout << "f1: returned second time: " << data << std::endl;
data+=1;
c=c.resume_with([&data](ctx::continuation && c){
                    std::cout << "f2: entered: " << data << std::endl;
                    data=-1;
                    return std::move( c);
                });
std::cout << "f1: returned third time" << std::endl;

output:
    f1: entered first time: 0
    f1: returned first time: 1
    f1: entered second time: 2
    f1: returned second time: 3
    f2: entered: 4
    f1: entered third time: -1
    f1: returned third time
```

> 所谓on top of就是在恢复cotinuation之前先执行一个额外的函数，可能会在cotinuation的栈上面增加一个栈

The expression `c.resume_with(...)` executes a lambda on top of continuation `c`, e.g. an additional stack frame is allocated on top of the stack. This lambda assigns `-1` to `data` and returns to the second invocation of `c.resume()`.



Another option is to execute a function on top of the continuation that throws an exception.

```c++
namespace ctx=boost::context;
struct my_exception : public std::runtime_error {
    ctx::continuation    c;
    my_exception(ctx::continuation && c_,std::string const& what) :
        std::runtime_error{ what },
        c{ std::move( c_) } {
    }
};

ctx::continuation c=ctx::callcc([](ctx::continuation && c) {
    for (;;) {
        try {
            std::cout << "entered" << std::endl;
            c=c.resume();
        } catch (my_exception & ex) {
            std::cerr << "my_exception: " << ex.what() << std::endl;
            return std::move(ex.c);
        }
    }
    return std::move(c);
});
c = c.resume_with(
       [](ctx::continuation && c){
           throw my_exception(std::move(c),"abc");
           return std::move( c);
       });

output:
    entered
    my_exception: abc
```

n this exception `my_exception` is throw from a function invoked on-top of continuation `c` and catched inside the `for`-loop.

> 能够在for循环里捕获还是非常神奇的
>
> 关键是在continuation上面的那个函数先抛出异常，之后continuation才捕获它
>
> 这个非常神奇，应该是有异常的保存机制

### Stack unwinding ###

On construction of [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) a stack is allocated. If the *context-function* returns the stack will be destructed. If the *context-function* has not yet returned and the destructor of an valid [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) instance (e.g. *continuation::operator bool()* returns `true`) is called, the stack will be destructed too.

> 在continuation的构造过程中一个栈会被分配
>
> continuation的析构／context-function返回（结束）都会导致栈的回收

Code executed by *context-function* must not prevent the propagation ofs the *detail::forced_unwind* exception. Absorbing that exception will cause stack unwinding to fail. Thus, any code that catches all exceptions must re-throw any pending *detail::forced_unwind* exception.

> detail::force_unwind异常不应该被continuation捕获

### Allocating control structures on top of stack ###

Allocating control structures on top of the stack requires to allocated the *stack_context* and create the control structure with placement new before [*continuation*](http://www.boost.org/doc/libs/1_65_1/libs/context/doc/html/context/cc.html#cc) is created.

> 在栈的顶部分配控制结构需要在continuation创建之前，分配stack_context并且创建控制结构

The user is responsible for destructing the control structure at the top of the stack.

```c++
namespace ctx=boost::context;
// stack-allocator used for (de-)allocating stack
fixedsize_stack salloc(4048);
// allocate stack space
stack_context sctx(salloc.allocate());
// reserve space for control structure on top of the stack
void * sp=static_cast<char*>(sctx.sp)-sizeof(my_control_structure);
std::size_t size=sctx.size-sizeof(my_control_structure);
// placement new creates control structure on reserved space
my_control_structure * cs=new(sp)my_control_structure(sp,size,sctx,salloc);
...
// destructing the control structure
cs->~my_control_structure();
...
struct my_control_structure  {
    // captured continuation
    ctx::continuation   c;

    template< typename StackAllocator >
    my_control_structure(void * sp,std::size_t size,stack_context sctx,StackAllocator salloc) :
        // create captured continuation
        c{} {
        c=ctx::callcc(std::allocator_arg,preallocated(sp,size,sctx),salloc,entry_func);
    }
    ...
};
```

以下这些问题怕是需要看boost相关部分的源代码才能解答了：

+ 所谓的控制结构到底在干嘛？
+ 为什么要隐藏控制结构？
+ 上面那段代码在干什么？

### Inverting the control flow ###



### Implementations: fcontext_t, ucontext_t and WinFiber ###

### Class continuation ###

## Class execution_context (version 2) ##

## Stack allocation ##

### Class protected_fixedsize ###

### Class pooled_fixedsize_stack ###

### Class fixedsize_stack ###

### Class segmented_stack ###

### Class stack_traits ###

### Class stack_context ###

### Support for valgrind ###

## Struct preallocated ##