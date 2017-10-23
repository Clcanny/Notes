# 第一步：写一个合适的例子 #

![1](1.jpg)

首先，要明确不是所有的阻塞操作都被hook过，都能表现出自动切换协程的特性（比如std::cin对应的系统调用就肯定没有，所以这不是一个好的例子）

![2](2.jpg)

无论你在输入3之前等多久，"Hello, libgo!"一定会在"i = 3"之后才输出

![3](3.jpg)

这才是一个优秀的例子，因为sleep对应的操作系统调用是被hook过的

![4](4.jpg)

libgo的作者告诉我们以下这些系统调用是hook过的：

```c++
connect   
read      
readv     
recv      
recvfrom  
recvmsg   
write     
writev    
send      
sendto    
sendmsg   
poll      
select    
accept    
sleep     
usleep    
nanosleep
gethostbyaddr_r
```

# 第二步：Let’s Hook a Library Function #

If you are a developer, and want to change the way a library function works, this article will give you a basic idea of how to get started — just enough knowledge to be able to experiment with your library functions. All code here is in C, and tested with GCC on Linux.

> 学习如何hook：改变库函数的功能
>
> 在库函数入口点和出口点做一些事情？

According to Wikipedia, “In computer programming, the term hooking covers a range of techniques used to alter or augment the behaviour of an operating system, applications, or other software components, by intercepting function calls or messages or events passed between software components. Code that handles such intercepted function calls, events or messages is called a hook.”

> 什么是kook？
>
> 打断函数调用／消息传递／事件传递

Intercepting a library call, and calling your own wrapper code, is also called Function Interposition.

> 术语：Function Interposition

Hooking has two benefits:

+ You don’t have to search for the function definition in the library, such as `libc` (`glibc` is the GNU C Library, and `libc` is almost half of the size of `glibc`) and change it. Seriously, this is a very nasty technical task (at least for me!).

  > 无需动源代码，无需重新编译

+ You don’t need to recompile the library’s source code.

![12](12.jpg)

## Library functions and system calls ##

Please look at Figures 1 and 2 for a graphical representation of what happens when a library function is hooked.

![13](13.jpg)

![14](14.jpg)

Now let’s look at hooking a library function. The simple `prog1.c` program below just allocates 10 bytes of memory from the heap, and frees it:

```c++
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
int main(void)
{
    int *p;
    printf("calling from main...\n");
    p = (int *)malloc(10);
    if (!p)
    {
        printf("Got allocation error...\n");
        exit(1);
    }
    printf("returning to main...\n");
    free(p);/* freeing memory from heap */
    printf("freeing memory...\n");
    return 0;
}
```

When we compile and run the above program, the output is as shown below:

![15](15.jpg)

The next program, called `prog2.c`, is a simple hook for the `malloc()` function:

![16](16.jpg)

Compiling and running the above, goes like this:

（我把prog1.c改名成test.cpp，把prog2.c改名成hook.c）

```shell
gcc -shared -ldl -fPIC hook.c -o libhook.so
g++ test.cpp -o test.out
LD_PRELOAD=/root/libhook.so ./test.out
```

不过我自己做不出来结果：coredump

![17](17.jpg)

So, let’s take a closer look at our first hook. The `dlsym()` function takes two parameters: the first is a handle returned by `dlopen()`. Here, we must use `RTLD_NEXT` for function interposition.

> `dlsym()`函数用于动态加载函数

This tells the dynamic linker to find the next reference to the specified function, not the one that is calling `dlsym()`. The second parameter is the symbol name (`malloc`, in this case), as a character string. `dlsym()`returns the address of the symbol specified as the second parameter. While compiling, `fPIC` is used to create a position-independent object.

> 解释一下`dlsym()`函数的用法

The `LD_PRELOAD` environment variable gives the loader a list of libraries to load before anything else. Our use of it will load `libprog2.so` and dynamically link it with the `prog1` binary. Don’t forget to provide an absolute path to the `.so`, in `LD_PRELOAD`. And, of course, include `_GNU_SOURCE` if you want to use certain extensions while using the GNU C library, because some extensions may not be available on other non-GNU systems and adding this `#define` will increase portability.

> 一种hook的方法就是替换掉动态链接库

## Can we hook every function with *dlsym()*? ##

The above hook method will not work if you want to wrap `dlsym()` itself, or wrap any library functions that internally call `dlsym()`.

> 上面的方法不能用于hook`dlsym()`函数本身
>
> 上面的方法不能用于hook内部调用过`dlsym()`函数的函数

So, is there any way to interpose `dlsym()`? Yes, but you can’t use the same hook procedure — check the output if you try that.

> 可以hook这些函数，但是不能用相同的方法

First, create a shared object `libfile.so` from `file1.c` and `file2.c` (below). Then compile it with `gcc -rdynamic -o dl_prog1 dl_prog1.c -ldl`. Yes, the output is obvious:

```c++
/* file1.c */
void file1(int *i)
{
    *i=100;
}
/* file2.c  */
void file2(int *i)
{
    *i=200;
}
```

The following `dl_prog1.c` is a simple program to show the functionality of `dlopen()` and `dlsym()`. The functions `file1()` and `file2()` are defined in `file1.c` and `file2.c`.

```c++
#include<stdio.h>
#include<dlfcn.h>
#include<stdlib.h>
 
void file1(int *i);
void file2(int *i);
int main(void)
{
    void *handler;
    int (*fn) (int *);
    int x;
    char *error;
    handler = dlopen("/home/dibyendu/dlsym_hook/libfile.so", RTLD_LAZY);
    if (!handler)
    {
        fprintf(stderr,"%s\n", dlerror());
        exit(1);
    }
    fn = dlsym(handler,"file1");     /* getting the handle of file1 through dlsym() */
    if ((error = dlerror()) != NULL) /* checking error through dlerror() */
    {
        fprintf(stderr,"%s\n", error);
        exit(1);
    }
    (*fn)(&x);                            /* Calling file1() to resolve x */
    printf("The value of x is %d\n", x);
    dlclose(handler);                 /* closing the file handle */
    return 0;
}
```

![18](18.jpg)

在这里要说两句：

+ file1.c & file2.c 用于提供静态链接库
+ d1_prog1.c 是主函数所在的文件，负责测试动态链接库及其配套函数

Now try to hook `dlsym()`, and you will get a segmentation fault, due to recursive calls (`dlsym()` will call itself). The following `dl_prog2.c` will be a recursive `dlsym()` call to itself, which causes a stack overflow and segmentation fault:

```c++
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
 
void *dlsym(void *handle, const char *name)
{
    void *(*dlsym_fn)(void *, const char *)=NULL;
    printf("inside shared object::before dlsym()...\n");
    dlsym_fn=dlsym(RTLD_NEXT, "dlsym"); /* this will call itself again and again */
    printf("inside shared object::after dlsym()...\n");
    return (*dlsym_fn)(handle, name);
}
```

![19](19.jpg)

> 无限递归

Here is the code (`dl_prog3.c`) that successfully interposes `dlsym()`:

```c++
#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
 
extern void *__libc_dlsym (void *, const char *);
void *dlsym(void *handle, const char *symbol)
{
    printf("Ha Ha...dlsym() Hooked\n");
    void* result = __libc_dlsym(handle, symbol); /* now, this will call dlsym() library function */
    return result;
}
```

![20](20.jpg)

> 根本的问题：如何不递归？

## What else can we do? ##

I have already interposed other functions like `getaddrinfo()`, `open()`, etc, so you can intercept any function you want. But there are a few limitations:

+ Do beware of functions that themselves call dlsym(), when you need to call __libc_dlsym (handle, symbol) in the hook.
+ Ensure that the SUID bit is not set, otherwise you can’t use `LD_PRELOAD`.
+ Also, internal library function calls are resolved before runtime — say, if some function in `libc` calls `getaddrinfo()` or `malloc()`, it will never call the hook from a different library.

## References ##

+ Tutorial: [Function Interposition in Linux](http://www.jayconrod.com/cgi/view_post.py?23) by Jay Conrod
+ Linux man pages: `dlopen()`, `dlsym()`, `dlerror()`, `dlclose()`

本小节来自[DIBYENDU ROY](http://opensourceforu.com/author/dibyendu-roy/)同名文章[Let’s Hook a Library Function](http://opensourceforu.com/2011/08/lets-hook-a-library-function/)

![21](21.jpg)

# 第三步：跟踪其中一个被hook过的系统调用 #

![5](5.jpg)

进入到Run函数内，不过还没有真正开始运行协程／Continuation／Task

![6](6.jpg)

看到了变量tk：根据打印出来的信息，我们可以判断这正是我们用`go`原语提交的第一个任务

在这个任务的执行中，会碰到sleep系统调用

![7](7.jpg)

不过一波操作失误，没有跟进去（呜呜呜～），只看到SwapIn函数返回之后，continuation／协程／Task的状态变成了`co::TaskState::sleep`，确实是我们期待的行为

然后我们又重来一遍（呵呵呵～）

```shell
gdb a.out
b libgo/libgo/processer.cpp:42
start
c
shell clear
list
```

![8](8.jpg)

![9](9.jpg)

我们发现：不得不跟进去，到boost.context里才能看到sleep函数的调用

![11](11.jpg)

> fn封装到ctx_里
>
> 只能呼叫ctx_()才能把它召唤出来

但是我们现在还没看过boost.context的源代码，还不想跟进去（这就很蛋疼了不是）

所以我们用一个奇招（碰一碰运气）：

![10](10.jpg)

你别说，还真行！

我们顺利找到了hook的入口点？（问号脸？它在停下来之前有没有做点别的什么？？？）

# 第四步：静态分析linux_glic_hook.cpp #

下面的分析会以`connect`函数作为例子

![22](22.jpg)

![24](24.jpg)

这是在静态“链接”系统调用（“链接”用的不准确，准确的来说只是提供一个函数声明，等待编译器的静态链接）

![23](23.jpg)

这是在动态“链接”系统调用（“链接”用的不准确，准确来说是通过API提出链接的要求，然后等待编译器的动态链接）

至此为止：connect_f 把静态“链接”和动态“链接”的函数统一了起来

![25](25.jpg)

![26](26.jpg)

`coroutine_hook_init()`函数就做了一件很简单的事情：

统一静态链接和动态链接的函数 + 检查一下是不是所有的函数都存在

（其实就是为真正的hook做准备）

重头戏马上就要来了啊（激动不激动啊！！！）

——————**这是重头戏的分界线**——————

![27](27.jpg)

如果不是在协程内，直接阻塞式调用

![28](28.jpg)

![29](29.jpg)

`typedef std::shared_ptr<FileDescriptorCtx> FdCtxPtr`

::diamond_shape_with_a_dot_inside:

