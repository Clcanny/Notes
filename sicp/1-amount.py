#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

import numpy as np

money = None
stack = None
count = None
amount = None
method = None

def cleanup(index):
    global money, stack, count, amount, method
    while index < stack.shape[0]:
        stack[index] = 0
        count[index] = 0
        index += 1

def count_change_helper_s1(index):
    global money, stack, count, amount, method
    res = amount - np.dot(stack, money)
    if res < 0:
        pass
    elif res == 0:
        method += 1
        pass
    elif index >= money.shape[0]:
        pass
    else:
        while True:
            if count[index] * money[index] > res:
                break
            stack[index] = count[index]
            count_change_helper_s1(index + 1)
            count[index] += 1
        stack[index] = 0
        count[index] = 0

def count_change_s1(a):
    global amount
    amount = a
    count_change_helper_s1(0)
    return method

def count_change_helper_s2(index):
    global money, stack, count, amount, method
    cleanup(index)
    res = amount - np.dot(stack, money)
    if res < 0:
        pass
    elif res == 0:
        method += 1
        pass
    elif index >= money.shape[0]:
        pass
    else:
        while True:
            if count[index] * money[index] > res:
                break
            stack[index] = count[index]
            count_change_helper_s2(index + 1)
            count[index] += 1

def count_change_s2(a):
    global amount
    amount = a
    count_change_helper_s2(0)
    return method

def count_change_helper_s3(index, amount, money, stack, count):
    global method
    stack = np.copy(stack)
    count = np.copy(count)
    res = amount - np.dot(stack, money)
    if res < 0:
        pass
    elif res == 0:
        method += 1
        pass
    elif index >= money.shape[0]:
        pass
    else:
        while True:
            if count[index] * money[index] > res:
                break
            stack[index] = count[index]
            count_change_helper_s3(index + 1, amount, money, stack, count)
            count[index] += 1

def count_change_s3(a):
    global amount
    amount = a
    count_change_helper_s3(0,
                           amount=amount,
                           money=money,
                           stack=stack,
                           count=count)
    return method

def count_change_helper_s4(index, amount, money, stack, count):
    def cleanup(index, stack, count):
        while index < stack.shape[0]:
            stack[index] = 0
            count[index] = 0
            index += 1

    global method
    stack = np.copy(stack)
    count = np.copy(count)
    res = amount - np.dot(stack, money)
    if res == 0:
        method += 1

    if index >= money.shape[0] or index < 0:
        raise Exception
    elif index == money.shape[0] - 1:
        if res > 0:
            if res % money[index] == 0:
                method += 1
        cleanup(index, stack, count)
        index -= 1
        count[index] += 1
        stack[index] = count[index]
        count_change_helper_s4(index, amount, money, stack, count)
    else:
        if res <= 0:
            cleanup(index, stack, count)
            if index == 0:
                pass
            else:
                index -= 1
                count[index] += 1
                stack[index] = count[index]
                count_change_helper_s4(index, amount, money, stack, count)
        else:
            count_change_helper_s4(index + 1, amount, money, stack, count)

def count_change_s4(amount):
    count_change_helper_s4(0,
                           amount=amount,
                           money=money,
                           stack=stack,
                           count=count)
    return method

def count_change_helper_s5(index, amount, money, stack, count):
    def pop_stack(index, stack, count):
        for i in range(index, stack.shape[0]):
            stack[i] = 0
            count[i] = 0
        if index < 0 or index >= stack.shape[0]:
            raise Exception
        else:
            index -= 1
            count[index] += 1
            stack[index] = count[index]
        return index

    global method
    stack = np.copy(stack)
    count = np.copy(count)
    res = amount - np.dot(stack, money)
    if res == 0:
        method += 1

    if index >= money.shape[0] or index < 0:
        raise Exception
    elif index == money.shape[0] - 1:
        if res > 0:
            if res % money[index] == 0:
                method += 1
        index = pop_stack(index, stack, count)
        count_change_helper_s5(index, amount, money, stack, count)
    else:
        if res <= 0:
            if index == 0:
                pass
            else:
                index = pop_stack(index, stack, count)
                count_change_helper_s5(index, amount, money, stack, count)
        else:
            count_change_helper_s5(index + 1, amount, money, stack, count)

def count_change_s5(amount):
    count_change_helper_s5(0,
                           amount=amount,
                           money=money,
                           stack=stack,
                           count=count)
    return method

def count_change_helper_s6(index, amount, money, stack):
    def pop_stack(index, stack):
        for i in range(index, stack.shape[0]):
            stack[i] = 0
        if index < 0 or index >= stack.shape[0]:
            raise Exception
        else:
            index -= 1
            stack[index] += 1
        return index

    global method
    stack = np.copy(stack)
    res = amount - np.dot(stack, money)
    if res == 0:
        method += 1

    if index >= money.shape[0] or index < 0:
        raise Exception
    elif index == money.shape[0] - 1:
        if res > 0:
            if res % money[index] == 0:
                method += 1
        index = pop_stack(index, stack)
        count_change_helper_s6(index, amount, money, stack)
    else:
        if res <= 0:
            if index == 0:
                pass
            else:
                index = pop_stack(index, stack)
                count_change_helper_s6(index, amount, money, stack)
        else:
            count_change_helper_s6(index + 1, amount, money, stack)

def count_change_s6(amount):
    count_change_helper_s6(0,
                           amount=amount,
                           money=money,
                           stack=stack)
    return method

def count_change_helper_s7(index, method, amount, money, stack):
    def pop_stack(index, stack):
        for i in range(index, stack.shape[0]):
            stack[i] = 0
        if index < 0 or index >= stack.shape[0]:
            raise Exception
        else:
            index -= 1
            stack[index] += 1
        return index

    stack = np.copy(stack)
    res = amount - np.dot(stack, money)
    if res == 0:
        method += 1

    if index >= money.shape[0] or index < 0:
        raise Exception
    elif index == money.shape[0] - 1:
        if res > 0:
            if res % money[index] == 0:
                method += 1
        index = pop_stack(index, stack)
        return count_change_helper_s7(index, method, amount, money, stack)
    else:
        if res <= 0:
            if index == 0:
                return method
            else:
                index = pop_stack(index, stack)
                return count_change_helper_s7(index, method, amount, money, stack)
        else:
            return count_change_helper_s7(index + 1, method, amount, money, stack)

def count_change_s7(amount):
    return count_change_helper_s7(0,
                                  method=0,
                                  amount=amount,
                                  money=money,
                                  stack=stack)

def init(solution):
    global money, stack, count, amount, method
    money = np.array([1, 5, 10, 25, 50])
    stack = np.zeros(money.shape[0])
    count = np.zeros(money.shape[0])
    amount = None
    method = 0
    if solution == 1:
        return count_change_s1
    elif solution == 2:
        return count_change_s2
    elif solution == 3:
        return count_change_s3
    elif solution == 4:
        return count_change_s4
    elif solution == 5:
        return count_change_s5
    elif solution == 6:
        return count_change_s6
    elif solution == 7:
        return count_change_s7

print(init(1)(100))
print(init(2)(100))
print(init(3)(100))
print(init(4)(30))
print(init(5)(30))
print(init(6)(30))
print(init(7)(30))
