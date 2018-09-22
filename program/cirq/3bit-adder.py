import cirq

def FullAdder(a_qbit, b_qbit, c0_qbit, c1_qbit):
    circuit.append(cirq.CCX.on(a_qbit, b_qbit, c1_qbit))
    circuit.append(cirq.CNOT.on(a_qbit, b_qbit))
    circuit.append(cirq.CCX.on(b_qbit, c0_qbit, c1_qbit))
    circuit.append(cirq.CNOT.on(b_qbit, c0_qbit))
    circuit.append(cirq.CNOT.on(a_qbit, b_qbit))

qbits = [cirq.GridQubit(j, i) for j in range(3) for i in range(3)]
qbits.append(cirq.GridQubit(2, 3))

print (qbits)

circuit = cirq.Circuit()
FullAdder(qbits[0], qbits[3], qbits[6], qbits[7])
FullAdder(qbits[1], qbits[4], qbits[7], qbits[8])
FullAdder(qbits[2], qbits[5], qbits[8], qbits[9])
print(circuit)

simulator = cirq.google.XmonSimulator()
circuit.append(cirq.measure(qbits[6], key='s0'), strategy = cirq.InsertStrategy.EARLIEST)
circuit.append(cirq.measure(qbits[7], key='s1'), strategy = cirq.InsertStrategy.EARLIEST)
circuit.append(cirq.measure(qbits[8], key='s2'), strategy = cirq.InsertStrategy.EARLIEST)
circuit.append(cirq.measure(qbits[9], key='s3'), strategy = cirq.InsertStrategy.EARLIEST)
print(circuit)

results = simulator.run(circuit, repetitions=100, qubit_order=qbits)
print (results.histogram(key='s0'))
print (results.histogram(key='s1'))
print (results.histogram(key='s2'))
print (results.histogram(key='s3'))
