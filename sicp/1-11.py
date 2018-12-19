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
        return self.g(3, [0, 1, 2], [1], [n])
    def g(self, k, stack, iStack, nStack):
        stack = list(stack)
        iStack = list(iStack)
        nStack = list(nStack)
        assert(len(iStack) == len(nStack))
        if len(iStack) == 0:
            assert(len(stack) != 0)
            return stack[-1]
        elif nStack[-1] < k:
            del iStack[-1]
            del nStack[-1]
            return self.g(k, stack, iStack, nStack)
        elif iStack[-1] >= k:
            assert(iStack[-1] == k)
            n = nStack[-1]
            del iStack[-1]
            del nStack[-1]
            assert(len(stack) == n)
            n = n - 1 - k
            stack.append(sum(stack[n:]))
            return self.g(k, stack, iStack, nStack)
        elif iStack[-1] < k:
            n = nStack[-1] - iStack[-1]
            iStack[-1] += 1
            iStack.append(1)
            nStack.append(n)
            return self.g(k, stack, iStack, nStack)
        else:
            print("stack: {}".format(stack))
            print("iStack: {}".format(iStack))
            print("nStack: {}".format(nStack))
            print("len(iStack) == 0: {}, len(iStack): {}".format(len(iStack) == 0, len(iStack)))
            print("nStack[-1] < k: {}, nStack[-1]: {}, k: {}".format(nStack[-1] < k, nStack[-1], k))
            print("iStack[-1] < k: {}, iStack[-1]: {}, k: {}".format(iStack[-1] < k, iStack[-1], k))
            print("\n")
            assert(0)

print(Solution1().f(5))
print(Solution2().f(5))
print(Solution3().f(5))
print(Solution4().f(5))
print(Solution5().f(5))
