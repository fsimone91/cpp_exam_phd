#!/bin/bash

export HOME=/lustrehome/fsimone/

g++ $HOME/cpp_exam_phd/muon_analysis.cc -o exe_muon -std=c++11
sleep 1

./exe_muon
