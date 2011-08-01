#!/bin/sh

INCLUDE_DIR="-I../../ -I../../Contrib/Box2D_v2.1.2/Box2D/"
OUT_DIR="../../GameData/Scripts"
SWIGFLAGS="-Wall -python -c++ $INCLUDE_DIR -outdir $OUT_DIR"

swig $SWIGFLAGS Game.i
