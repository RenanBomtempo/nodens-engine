#!/bin/sh
#
# -- our name ---
#$ -N MonoAlg3D_DTDXAdpt
#$ -S /bin/sh
# Make sure that the .e and .o file arrive in the
# working directory
#$ -cwd
#Merge the standard out and standard error to one file
#$ -j y
bin/alg -f 10.0 -d 2.0 -l 3200.0 -a -p 10 -i 50.0 -m 50.0 -x 200.0 -r 0.10 -b 0.20
