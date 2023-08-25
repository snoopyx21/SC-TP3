#!/bin/sh 
ipcs -s | grep $USERNAME | awk ' { print $2 } ' | xargs ipcrm sem
