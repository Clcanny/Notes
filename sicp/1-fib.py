#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

# 非尾递归版本
class Solution1:
    def fib(self, n):
        if n == 0 or n == 1:
            return n
        else:
            return self.fib(n - 1) + self.fib(n - 2)

# 记账
class Solution2:
    stack = [0, 1]
    def fib(self, n):
        if n >= len(self.stack):
            for i in range(1, 3):
                self.fib(n - i)
            self.stack.append(self.stack[n - 2] + self.stack[n - 1])
        return self.stack[n]

# 将隐式栈变成显式栈
# 将 i 变成显式栈
class Solution3:
    stack = [0, 1]
    iStack = []
    def fib(self, n):
        if n >= len(self.stack):
            self.iStack.append(1)
            while self.iStack[-1] < 3:
                self.fib(n - self.iStack[-1])
                self.iStack[-1] += 1
            # delete the last element
            del self.iStack[-1]
            self.stack.append(self.stack[n - 2] + self.stack[n - 1])
        return self.stack[n]

# 将隐式栈变成显式栈
# 进一步将 n 也变成显式栈
# 这个版本已经是尾递归
class Solution4:
    stack = [0, 1]
    iStack = []
    nStack = []
    def fib(self, n):
        assert(len(self.iStack) == len(self.nStack))
        if len(self.iStack) != 0 and self.iStack[-1] >= 3:
            assert(self.iStack[-1] == 3)
            del self.iStack[-1]
            n = self.nStack[-1]
            assert(len(self.stack) == n)
            self.stack.append(self.stack[n - 2] + self.stack[n - 1])
            del self.nStack[-1]
            return self.fib(0)
        else:
            if n >= len(self.stack):
                self.iStack.append(1)
                self.nStack.append(n)
                return self.fib(n - self.iStack[-1])
            else:
                if len(self.nStack) != 0:
                    self.iStack[-1] += 1
                    return self.fib(self.nStack[-1] - (self.iStack[-1] - 1))
                else:
                    return self.stack[-1]

# 将 n 变成显式栈之后不再需要变量 n
class Solution5:
    stack = [0, 1]
    iStack = []
    nStack = []
    def set(self, n):
        self.iStack.append(1)
        self.nStack.append(n)
        return self
    def fib(self):
        assert(len(self.iStack) == len(self.nStack))
        if len(self.iStack) != 0 and self.iStack[-1] >= 3:
            assert(self.iStack[-1] == 3)
            del self.iStack[-1]
            n = self.nStack[-1]
            assert(len(self.stack) == n)
            self.stack.append(self.stack[n - 2] + self.stack[n - 1])
            del self.nStack[-1]
            # 退栈之后需要检查 iStack 与 nStack 是否为空
            return self.fib()
        else:
            if len(self.iStack) != 0 and self.nStack[-1] - self.iStack[-1] >= len(self.stack):
                self.iStack.append(1)
                self.nStack.append(self.nStack[-1] - self.iStack[-1])
                return self.fib()
            else:
                if len(self.iStack) != 0:
                    self.iStack[-1] += 1
                    return self.fib()
                else:
                    return self.stack[-1]

# 将全局变量变成传入参数
class Solution6:
    def fib(self, n):
        return self.fibHelper([0, 1], [1], [n])
    def fibHelper(self, stack, iStack, nStack):
        stack = list(stack)
        iStack = list(iStack)
        nStack = list(nStack)
        assert(len(iStack) == len(nStack))
        if len(iStack) != 0 and iStack[-1] >= 3:
            assert(iStack[-1] == 3)
            del iStack[-1]
            n = nStack[-1]
            assert(len(stack) == n)
            stack.append(stack[n - 2] + stack[n - 1])
            del nStack[-1]
            # 退栈之后需要检查 iStack 与 nStack 是否为空
            return self.fibHelper(stack, iStack, nStack)
        else:
            if len(iStack) != 0 and nStack[-1] - iStack[-1] >= len(stack):
                iStack.append(1)
                nStack.append(nStack[-1] - iStack[-1])
                return self.fibHelper(stack, iStack, nStack)
            else:
                if len(iStack) != 0:
                    iStack[-1] += 1
                    return self.fibHelper(stack, iStack, nStack)
                else:
                    return stack[-1]

# iStack 的长度和 nStack 的长度总是相同的
# iStack 和 nStack 只需要其中一个
class Solution7:
    def fib(self, n):
        return self.fibHelper([0, 1], [n])
    def fibHelper(self, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        if len(nStack) != 0:
            if nStack[-1] > len(stack):
                nStack.append(nStack[-1] - 1)
                nStack.append(nStack[-1] - 2)
                return self.fibHelper(stack, nStack)
            elif nStack[-1] == len(stack):
                stack.append(stack[-1] + stack[-2])
                return self.fibHelper(stack, nStack)
            else:
                del nStack[-1]
                return self.fibHelper(stack, nStack)
        else:
            return stack[-1]

# 显式栈仍然是不断增长的
# 注意到计算只需要使用 stack 的最上面两个元素
# 所以我们可以限制 stack 的长度
class Solution8:
    def fib(self, n):
        return self.fibHelper(2, [0, 1], [n])
    def fibHelper(self, stackLength, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        assert(len(stack) == 2)
        if len(nStack) != 0:
            if nStack[-1] > stackLength:
                nStack.append(nStack[-1] - 1)
                nStack.append(nStack[-1] - 2)
                return self.fibHelper(stackLength, stack, nStack)
            elif nStack[-1] == stackLength:
                stack.append(sum(stack))
                stack = stack[1:]
                stackLength += 1
                return self.fibHelper(stackLength, stack, nStack)
            else:
                del nStack[-1]
                return self.fibHelper(stackLength, stack, nStack)
        else:
            return stack[-1]

# nStack 的长度比较难限制
# 需要先查看 nStack 增长与消退的规律
class Solution9:
    def fib(self, n, printNStack=True):
        if printNStack:
            return self.fibHelper(2, [0, 1], [n])
        else:
            return self.fibHelper2(n, 2, [0, 1])
    # 查看 nStack 增长与消退的规律
    def fibHelper(self, stackLength, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        print("nStack: {}".format(nStack))
        assert(len(stack) == 2)
        if len(nStack) != 0:
            if nStack[-1] > stackLength:
                n = nStack[-1]
                nStack.append(n - 1)
                nStack.append(n - 2)
                # 过滤掉不需要的元素，更容易看出 nStack 的规律
                nStack = filter(lambda x: x >= stackLength, nStack)
                return self.fibHelper(stackLength, stack, nStack)
            elif nStack[-1] == stackLength:
                stack.append(sum(stack))
                stack = stack[1:]
                stackLength += 1
                del nStack[-1]
                return self.fibHelper(stackLength, stack, nStack)
            else:
                assert(False)
        else:
            return stack[-1]
    # nStack == range(2, n + 1)
    # 只有 nStack 最顶端的元素会被直接使用到
    # 自然可以从 nStack 的最顶端计算到 nStack 的最底端
    # 退栈的过程就是栈顶从 2 变成 len(stack) 的过程
    # 结束计算的条件是 n == len(stack)
    def fibHelper2(self, n, stackLength, stack):
        stack = list(stack)
        assert(len(stack) == 2)
        if stackLength == n + 1:
            return stack[-1]
        elif stackLength < n + 1:
            stack.append(sum(stack))
            stack = stack[1:]
            stackLength += 1
            return self.fibHelper2(n, stackLength, stack)
        else:
            assert(False)

print(Solution1().fib(10))
print(Solution2().fib(10))
print(Solution3().fib(10))
print(Solution4().fib(10))
print(Solution5().set(10).fib())
print(Solution6().fib(10))
print(Solution7().fib(10))
print(Solution8().fib(10))
print(Solution9().fib(10))
print(Solution9().fib(10, False))
