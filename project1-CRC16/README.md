Raywen Wu (Ray W.)
UID 705330558

# CRC16 Experiement 
program (in C++) for a sender that takes a bit stream and computes several experiments to do with CRCs.

## Building

type 'make' to create crcexperiments executable. 
 

## Running

./crcexperiments with following flags 

-c   generates codeword with input bitstring  
-v   verifies input codeword is correct with Generator 
-f   output all the 4 bits undetected errors 
-t   get the fraction of the undetected 2 bits errors, output nothing iff the fraction is 0. 


## Cleaning up

type 'make clean' to delete the executable and object files.

