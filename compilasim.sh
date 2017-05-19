#!/bin/bash
export LIBS=$(root-config --glibs)
export CC="-pthread  -Wno-deprecated-declarations -m64 -I/alistorage/Soft/devtool3/alice/sw/slc6_x86-64/ROOT/v5-34-30-alice7-1/include"
#$(root-config --cflags)
export LD=-Wl,--no-as-needed
c++ $LD $CC $LIBS sim.C particle.cxx particleType.cxx resonanceType.cxx -o sim.exe
