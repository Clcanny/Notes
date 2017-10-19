```shell
git clone https://github.com/yyzybb537/libgo.git
cd libgo

mkdir CompileDebugWithoutBoost
cd CompileDebugWithoutBoost
cmake ..
make debug
make install
// 由于我们在docker容器内执行该命令，所以不需要加sudo
make uninstall

apt-get install libboost-dev
apt-get install libboost-all-dev
mkdir CompileNoDebugWithBoost
cd CompileNoDebugWithBoost
// libgo在Linux系统上默认使用ucontext做协程上下文切换，开启此选项将使用boost.context来替代ucontext
// libgo在Linux系统上默认使用ucontext做协程上下文切换，开启此选项将使用boost.coroutine来替代ucontext
// 两个选项任选一个即可
cmake .. -DENABLE_BOOST_CONTEXT=ON -DENABLE_BOOST_COROUTINE=ON
// Boost.Coroutine is now deprecated. Please switch to Boost.Coroutine2.
rm -f -r *
cmake .. -DENABLE_BOOST_CONTEXT=ON
make install
```

```shell
cd ~
cp libgo/tutorial/sample1_go.cpp test_libgo.cpp
cp libgo/tutorial/win_exit.h .

// 动态链接
g++ -std=c++11 -g -Wall test_libgo.cpp -Ilibgo/libgo/ -Ilibgo/libgo/linux -llibgo -llibgo_main -lboost_coroutine -lboost_context -lboost_system -lboost_thread -lpthread -ldl
// 在执行的时候会出错，提示没有共享库
// error while loading shared libraries: liblibgo.so
// cannot open shared object file: No such file or directory
// 但是存在/usr/local/lib/liblibgo.so

// 查看环境变量
echo $LD_LIBRARY_PATH
// 该环境变量为空，于是设置它
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
./a.out

// 静态链接
// 我们把刚才设置的环境变量清除
LD_LIBRARY_PATH=
echo $LD_LIBRARY_PATH
// 此刻运行a.out会报错
g++ -std=c++11 -g -Wall test_libgo.cpp -Ilibgo/libgo/ -Ilibgo/libgo/linux -llibgo -llibgo_main -lboost_coroutine -lboost_context -lboost_system -lboost_thread -lpthread -static -static-libgcc -static-libstdc++
./a.out
```

通过两种方式编译出来的libgo + 静态／动态链接可执行文件

一共四种组合，都是可以正常工作的

按住(control + p) + (control + q)从容器中脱离，再执行：

```shell
docker cp libgo:/root/a.out ./
./a.out
```

只要a.out是静态链接产生的，无论它静态链接的文件是CompileDebugWithoutBoost产生的，还是CompileNoDebugWithBoost产生的，都可以在主机上运行（即使主机上没有任何特别的库）



test_libgo.cpp

```c++
/************************************************
 * libgo sample1
*************************************************/
#include "coroutine.h"
#include "win_exit.h"
#include <stdio.h>
#include <boost/thread.hpp>

void foo()
{
    printf("function pointer\n");
}

struct A {
    void fA() { printf("std::bind\n"); }
    void fB() { printf("std::function\n"); }
};

int main()
{
    // 使用关键字go创建协程, go后面可以使用:
    //     1.void(*)()函数指针, 比如:foo.
    //     2.也可以使用无参数的lambda, std::bind对象, function对象, 
    //     3.以及一切可以无参调用的仿函数对象
    //   注意不要忘记句尾的分号";".
    go foo;

    go []{
        printf("lambda\n");
    };

    go std::bind(&A::fA, A());

    std::function<void()> fn(std::bind(&A::fB, A()));
    go fn;

    // 也可以使用go_stack创建指定栈大小的协程
    //   创建拥有10MB大栈的协程
    go_stack(10 * 1024 * 1024) []{
        printf("large stack\n");
    };

    // 协程创建以后不会立即执行，而是暂存至可执行列表中，等待调度器调度。
    // co_sched是全局唯一的协程调度器，有以下接口可以调度协程：
    //   1.Run 执行单次调度, 返回本次执行的协程数量
    //   2.RunLoop 无限循环执行Run, 不会返回
    //   3.RunUntilNoTask 循环执行Run, 直至协程数量为零.
    //
    // 当仅使用一个线程进行协程调度时, 协程地执行会严格地遵循其创建顺序.
    co_sched.RunUntilNoTask();
```

win_exit.h
```c++
#pragma once

#ifdef _WIN32
#include <stdlib.h>
struct exit_pause {
	~exit_pause()
	{
		system("pause");
	}
} g_exit;
#endif
```



再提一下scp的使用方式：

```shell
scp demons@115.159.199.121:~/a.out .
```

不过直接拷贝下来的文件在Mac上不可执行

编写该文件是libgo所在的commit是：13a4e6627ac821187277f0b5b3ac4419a34b7190
