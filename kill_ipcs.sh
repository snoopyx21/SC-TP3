#!/bin/sh 
ipcs -m | egrep "0x[0-9a-f]+ [0-9]+" | grep cdivriotis | cut -f2 -d" " | xargs ipcrm shm

