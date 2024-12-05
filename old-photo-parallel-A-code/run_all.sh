#!/bin/bash

WORK_DIR="../"
clear;
make clean-all;
sudo mkdir -p "$WORK_DIR"  # Ensure the directory exists
sudo chmod 777 "$WORK_DIR"
make coregrind;
echo "Done run test.";
