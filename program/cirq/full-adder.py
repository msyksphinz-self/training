import cirq
qbits = [cirq.GridQubit(0, i) for i in range(4)]
print (qbits)

circuit = cirq.Circuit()
circuit.append(cirq.X(qbits[0]))   # Set initial value of a
circuit.append(cirq.X(qbits[1]))   # Set initial value of b
circuit.append(cirq.CCX.on(qbits[0], qbits[1], qbits[3]))
circuit.append(cirq.CNOT.on(qbits[0], qbits[1]))
circuit.append(cirq.CCX.on(qbits[1], qbits[2], qbits[3]))
circuit.append(cirq.CNOT.on(qbits[1], qbits[2]))
circuit.append(cirq.CNOT.on(qbits[0], qbits[1]))
print(circuit)

simulator = cirq.google.XmonSimulator()
circuit.append(cirq.measure(qbits[2], key='s'))
circuit.append(cirq.measure(qbits[3], key='c'))
print(circuit)

results = simulator.run(circuit, repetitions=100, qubit_order=qbits)
print (results.histogram(key='s'))
print (results.histogram(key='c'))
