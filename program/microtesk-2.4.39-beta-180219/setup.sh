#!/bin/bash

# Installing Tools : z3
mkdir tools/z3
mkdir tools/z3/unix

pushd tools/z3/unix/
wget https://github.com/Z3Prover/z3/releases/download/z3-4.6.0/z3-4.6.0-x64-ubuntu-16.04.zip
unzip z3-4.6.0-x64-ubuntu-16.04.zip
ln -s z3-4.6.0-x64-ubuntu-16.04 z3
popd

# Installing Tools : cvc4
mkdir tools/cvc4
mkdir tools/cvc4/unix

pushd tools/cvc4/unix/
wget http://cvc4.cs.stanford.edu/downloads/builds/x86_64-linux-opt/unstable/cvc4-2018-03-15-x86_64-linux-opt
ln -s cvc4-2018-03-15-x86_64-linux-opt  cvc4
popd
