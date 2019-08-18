#!/bin/bash

target_dirs=('/home/msyksphinz/work/llvm/llvm-myriscvx/' '/home/msyksphinz/work/llvm/myriscvx-tests/' '/home/msyksphinz/work/llvm/build-myriscvx')

for target_dir in ${target_dirs[*]}
do
    echo "Uploading " ${target_dir} " ..."
    aws s3 sync ${target_dir} s3://llvm-bucket/`basename ${target_dir}` --exclude ".git*"
done
