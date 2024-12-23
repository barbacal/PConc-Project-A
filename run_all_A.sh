#!/bin/bash

clear;
make clean-all -C old-photo-parallel-A-code/;
make all -C old-photo-parallel-A-code/;
echo "Done clean all and build.";
