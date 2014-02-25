#! /bin/bash
 
set -x
(
for N in $(seq 100 100 1500); do
 echo $N; ./example2 N
done 
)> out

