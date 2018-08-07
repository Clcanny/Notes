#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

class Solution1:
    def f(self, n):
        if n < 3:
            return n
        else:
            return self.f(n - 1) + self.f(n - 2) + self.f(n -3)

# 将函数泛化以增加难度
class Solution2:
    def f(self, n):
        return self.g(3, n)

    def g(self, k, n):
        if n < k:
            return n
        else:
            sum = 0
            for i in range(1, k + 1):
                sum += self.g(k, n - i)
            return sum

# 将局部变量提成全局变量
# 尤其注意把 for 循环变成栈
class Solution3:
    def f(self, n):
        self.k = 3
        self.sum = 0
        self.iStack = [1]
        self.nStack = [n]
        return self.g()
    def g(self):
        assert(len(self.iStack) == len(self.nStack))
        if len(self.iStack) == 0:
            return self.sum
        elif self.nStack[-1] < self.k:
            self.sum += self.nStack[-1]
            del self.iStack[-1]
            del self.nStack[-1]
            # 退栈之后要检查 iStack 是否为空
            return self.g()
        elif self.iStack[-1] >= self.k + 1:
            assert(self.iStack[-1] == self.k + 1)
            # 退栈
            del self.iStack[-1]
            del self.nStack[-1]
            # 退栈后要检查 iStack 是否为空
            return self.g()
        else:
            n = self.nStack[-1] - self.iStack[-1]
            self.iStack[-1] += 1
            self.iStack.append(1)
            self.nStack.append(n)
            return self.g()

# 借助全局变量和显式栈，很容易写出尾递归函数
# 将全局变量变成参数
class Solution4:
    def f(self, n):
        return self.g(3, 0, [1], [n])
    def g(self, k, sum, iStack, nStack):
        sum = int(sum)
        iStack = list(iStack)
        nStack = list(nStack)
        assert(len(iStack) == len(nStack))
        if len(iStack) == 0:
            return sum
        elif nStack[-1] < k:
            sum += nStack[-1]
            del iStack[-1]
            del nStack[-1]
            # 退栈之后要检查 iStack 是否为空
            return self.g(k, sum, iStack, nStack)
        elif iStack[-1] >= k + 1:
            assert(iStack[-1] == k + 1)
            # 退栈
            del iStack[-1]
            del nStack[-1]
            # 退栈后要检查 iStack 是否为空
            return self.g(k, sum, iStack, nStack)
        else:
            n = nStack[-1] - iStack[-1]
            iStack[-1] += 1
            iStack.append(1)
            nStack.append(n)
            return self.g(k, sum, iStack, nStack)

# 记账
class Solution5:
    def f(self, n):
        if n < 3:
            return n
        return self.g(3, [0, 1, 2], [1], [n])
    def g(self, k, stack, iStack, nStack):
        stack = list(stack)
        iStack = list(iStack)
        nStack = list(nStack)
        assert(len(iStack) == len(nStack))
        if len(iStack) == 0:
            assert(len(stack) != 0)
            return stack[-1]
        elif nStack[-1] < len(stack):
            del iStack[-1]
            del nStack[-1]
            return self.g(k, stack, iStack, nStack)
        elif iStack[-1] >= k:
            assert(nStack[-1] == len(stack))
            n = nStack[-1]
            del iStack[-1]
            del nStack[-1]
            assert(len(stack) == n)
            n = n - k
            stack.append(sum(stack[n:]))
            return self.g(k, stack, iStack, nStack)
        elif iStack[-1] < k:
            n = nStack[-1] - iStack[-1]
            iStack[-1] += 1
            iStack.append(1)
            nStack.append(n)
            return self.g(k, stack, iStack, nStack)
        else:
            assert(0)

# iStack 的长度和 nStack 的长度总是一样的
# iStak 和 nStack 存在其中一个即可
class Solution6:
    def f(self, n):
        if n < 3:
            return n
        return self.g(3, [0, 1, 2], [n])
    def g(self, k, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        if len(nStack) == 0:
            return stack[-1]
        elif nStack[-1] < len(stack):
            del nStack[-1]
            return self.g(k, stack, nStack)
        elif nStack[-1] == len(stack):
            n = nStack[-1] - k
            del nStack[-1]
            stack.append(sum(stack[n:]))
            return self.g(k, stack, nStack)
        else:
            nStack.extend(list(range(nStack[-1] - k, nStack[-1])))
            return self.g(k, stack, nStack)

# 至此非尾递归已经被我们转换成尾递归
# 不过隐式的不断增长的栈被我们换成显式的不断增长的栈
# stack 中有用的仅仅是后 k 个元素
# 有没有办法限制显式栈的长度？
# 先限制 stack 的长度（相对简单一点）
class Solution7:
    def f(self, n):
        if n < 3:
            return n
        return self.g(3, 3, [0, 1, 2], [n])
    def g(self, k, stackLength, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        assert(len(stack) == k)
        if len(nStack) == 0:
            return stack[-1]
        elif nStack[-1] < stackLength:
            del nStack[-1]
            return self.g(k, stackLength, stack, nStack)
        elif nStack[-1] == stackLength:
            stack.append(sum(stack))
            stack = stack[1:]
            stackLength += 1
            return self.g(k, stackLength, stack, nStack)
        else:
            nStack.extend(range(nStack[-1] - k, nStack[-1]))
            return self.g(k, stackLength, stack, nStack)

# 限制 nStack 的长度
# 甚至可以将 nStack 变成一个数
class Solution8:
    def f(self, n, helper=False):
        if n < 3:
            return n
        if helper:
            return self.gHelper(3, 3, [0, 1, 2], [n])
        else:
            return self.g(3, n, 3, [0, 1, 2])
    # 把 nStack 打印出来
    # 看 nStack 增长与消退的规律
    def gHelper(self, k, stackLength, stack, nStack):
        stack = list(stack)
        nStack = list(nStack)
        print("nStack: {}".format(nStack))
        assert(len(stack) == k)
        if len(nStack) == 0:
            return stack[-1]
        elif nStack[-1] < stackLength:
            assert(False)
        elif nStack[-1] == stackLength:
            stack.append(sum(stack))
            stack = stack[1:]
            stackLength += 1
            del nStack[-1]
            return self.gHelper(k, stackLength, stack, nStack)
        else:
            nStack.extend(range(nStack[-1] - k, nStack[-1])[::-1])
            nStack = filter(lambda x: x >= stackLength, nStack)
            return self.gHelper(k, stackLength, stack, nStack)
    # nStack == range(k, n + 1)
    # 只有 nStack 最顶端的元素会被直接使用到
    # 自然可以从 nStack 的最顶端计算到 nStack 的最底端
    # 退栈的过程就是栈顶从 k 变成 len(stack) + 1 的过程
    # 结束计算的条件是 n == len(stack) - 1
    def g(self, k, n, stackLength, stack):
        stack = list(stack)
        assert(len(stack) == k)
        topNStack = stackLength - 1
        if topNStack == n:
            return stack[-1]
        else:
            stack.append(sum(stack))
            stack = stack[1:]
            stackLength += 1
            return self.g(k, n, stackLength, stack)

n = 10
print(Solution1().f(n))
print(Solution2().f(n))
print(Solution3().f(n))
print(Solution4().f(n))
print(Solution5().f(n))
print(Solution6().f(n))
print(Solution7().f(n))
print(Solution8().f(n, True))
print(Solution8().f(n))
