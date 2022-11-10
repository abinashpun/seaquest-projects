#!/bin/bash

for i in {000..0019} #need to adjust this limit based on the number of your generated input
do
    FILENAME_BASE=generated_name_00$i
    echo $FILENAME_BASE 
    ./process_realization.sh ${FILENAME_BASE}
    sleep 1
done
