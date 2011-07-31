#!/bin/sh

INCLUDE_DIR=../../
OUT_DIR=../../GameData/Scripts

swig -python -c++ -I$INCLUDE_DIR -outdir $OUT_DIR Volt.i 
swig -python -c++ -I$INCLUDE_DIR -outdir $OUT_DIR Game.i 
