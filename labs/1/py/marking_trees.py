# Lab 1 - Marking trees

import sys
from random import randint
from statistics import mean, stdev

"""
The binary tree is complete, i.e. no or both children.

Marking rules: 
 1. a non leaf node is marked when both its children are marked
 2. a non root node is marked if its parent and sibling is marked
"""


def knuth_shuffle(a):
    for i in range(len(a) - 1, 1, -1):
        j = randint(0, i)
        t = a[i]
        a[i] = a[j]
        a[j] = t
    return a


def left_child(i):
    return 2 * i + 1


def right_child(i):
    return 2 * i + 2


def parent(i):
    return (i - 1) // 2  # ⌊ (i-1) / 2 ⌋


def sibling(i):
    if i % 2 == 1:  # left
        return i + 1
    else:
        return i - 1


def mark_node(i, counter, marked):
    # returns those nodes that were marked
    if not 0 <= i < len(marked):
        return set()

    if marked[i]:
        return set()

    marked[i] = True
    counter += 1
    marks = {i}

    # both children are marks
    if marked[sibling(i)]:
        marks = marks.union(mark_node(parent(i), counter, marked))

    # one child is marked
    if i < len(marked) // 2 and (marked[left_child(i)] ^ marked[right_child(i)]):
        if marked[left_child(i)]:
            marks = marks.union(mark_node(right_child(i), counter, marked))
        elif marked[right_child(i)]:
            marks = marks.union(mark_node(left_child(i), counter, marked))

    # parent is marked but not its sibling
    if i > 0 and marked[parent(i)] and not marked[sibling(i)]:
        marks = marks.union(mark_node(sibling(i), counter, marked))

    return marks


def print_w_idx(m):
    for i, v in enumerate(m):
        print(f"{i+1}: {v}", end=", ")
    print()
    return


def calc_N(h):
    return 2 ** h - 1


def next_R1(N):
    return randint(1, N)


def gen_inst(N):
    inst = list(range(N))
    inst = knuth_shuffle(inst)
    return inst


def next_R3(i, inst, marked):
    while marked[inst[i]]:
        i += 1
    return i, inst[i]


def R1(N):
    counter = 0
    marked = [False] * N
    marked_nodes = set()

    rounds = 0
    while len(marked_nodes) < N:
        n = next_R1(N)
        m = mark_node(n, counter, marked)
        marked_nodes = marked_nodes.union(m)
        rounds += 1

    return rounds


def R2(N):
    counter = 0
    marked = [False] * N
    marked_nodes = set()

    inst = gen_inst(N)

    rounds = 0
    while len(marked_nodes) < N:
        n = inst[rounds]
        m = mark_node(n, counter, marked)
        marked_nodes = marked_nodes.union(m)
        rounds += 1

    return rounds


def R3(N):
    counter = 0
    marked = [False] * N
    marked_nodes = set()

    inst = gen_inst(N)

    rounds = i = 0
    while len(marked_nodes) < N:
        i, n = next_R3(i, inst, marked)
        m = mark_node(n, counter, marked)
        marked_nodes = marked_nodes.union(m)
        rounds += 1

    return rounds


def pp(rounds):
    m = round(mean(rounds), 1)
    s = round(stdev(rounds), 1)
    return f"{m}±{s}"


def main():
    lim_h = 20
    trials = 1000

    print("h | N | R_1 | R_2 | R_3")
    for h in range(2, lim_h + 1):
        N = calc_N(h)
        r1_rounds = []
        r2_rounds = []
        r3_rounds = []
        for _ in range(trials):
            r1_rounds.append(R1(N))
            r2_rounds.append(R2(N))
            r3_rounds.append(R3(N))
        print(f"{h} | {N} | {pp(r1_rounds)} | {pp(r2_rounds)} | {pp(r3_rounds)}")


if __name__ == "__main__":
    main()
    # print(R1(7))
    # print(R2(7))
    # print(R3(7))
