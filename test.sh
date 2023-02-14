#!/bin/bash


EXE=cypher

./cypher $1 < $2 > ct.dat
./cypher -d $1 < ct.dat > pt.dat

diff $2 pt.dat

rm pt.dat ct.dat

