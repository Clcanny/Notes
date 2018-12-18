#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
class Solution1:
    def fib(self, n):
        if n == 0 or n == 1:
            return n
        else:
            return self.fib(n - 1) + self.fib(n - 2)

class Solution2:
    stack = [0, 1]
    def fib(self, n):
        if n >= len(self.stack):
            for i in range(1, 3):
                self.fib(n - i)
            self.stack.append(self.stack[n - 2] + self.stack[n - 1])
        return self.stack[n]

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

print(Solution1().fib(10))
print(Solution2().fib(10))
print(Solution3().fib(10))
print(Solution4().fib(10))
print(Solution5().set(10).fib())
print(Solution6().fib(10))
print(Solution7().fib(10))
