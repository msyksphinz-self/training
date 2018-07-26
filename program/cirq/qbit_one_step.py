#!/usr/bin/python3
import cirq
import random

def rand2d(rows, cols):
    return [[random.choice([+1, -1]) for _ in range(rows)] for _ in range(cols)]

def random_instance(length):
    # transverse field terms
    h = rand2d(length, length)
    # links within a row
    jr = rand2d(length, length - 1)
    # links within a column
    jc = rand2d(length - 1, length)
    return (h, jr, jc)

h, jr, jc = random_instance(3)
# print('transverse fields: {}'.format(h))
# print('row j fields: {}'.format(jr))
# print('column j fields: {}'.format(jc))
# prints something like
# transverse fields: [[-1, 1, -1], [1, -1, -1], [-1, 1, -1]]
# row j fields: [[1, 1, -1], [1, -1, 1]]
# column j fields: [[1, -1], [-1, 1], [-1, 1]]

def one_step(h, jr, jc, x_half_turns, h_half_turns, j_half_turns):
    length = len(h)
    yield rot_x_layer(length, x_half_turns)
    yield rot_z_layer(h, h_half_turns)
    yield rot_11_layer(jr, jc, j_half_turns)

def rot_x_layer(length, half_turns):
    """Yields X rotations by half_turns on a square grid of given length."""
    rot = cirq.RotXGate(half_turns=half_turns)
    for i in range(length):
        for j in range(length):
            yield rot(cirq.GridQubit(i, j))
def rot_z_layer(h, half_turns):
    """Yields Z rotations by half_turns conditioned on the field h."""
    gate = cirq.RotZGate(half_turns=half_turns)
    for i, h_row in enumerate(h):
        for j, h_ij in enumerate(h_row):
            if h_ij == 1:
                yield gate(cirq.GridQubit(i, j))
def rot_11_layer(jr, jc, half_turns):
    """Yeilds rotations about |11> conditioned on the jr and jc fields."""
    gate = cirq.Rot11Gate(half_turns=half_turns)
    for i, jr_row in enumerate(jr):
        for j, jr_ij in enumerate(jr_row):
            if jr_ij == -1:
                yield cirq.X(cirq.GridQubit(i, j))
                yield cirq.X(cirq.GridQubit(i + 1, j))
            yield gate(cirq.GridQubit(i, j),
                       cirq.GridQubit(i + 1, j))
            if jr_ij == -1:
                yield cirq.X(cirq.GridQubit(i, j))
                yield cirq.X(cirq.GridQubit(i + 1, j))

    for i, jc_row in enumerate(jc):
        for j, jc_ij in enumerate(jc_row):
            if jc_ij == 1:
                yield gate(cirq.GridQubit(i, j),
                           cirq.GridQubit(i, j + 1))

h, jr, jc = random_instance(3)

circuit = cirq.Circuit()
circuit.append(one_step(h, jr, jc, 0.1, 0.2, 0.3))
print(circuit)
# prints something like
