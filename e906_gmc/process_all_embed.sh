#!/bin/bash
for i in {00000..000019} #adjust the range according to your input
do
    FILENAME_BASE=realized_filename_${i}_real
    echo $FILENAME_BASE 
    ./process_embed_inte.sh ${FILENAME_BASE}
    sleep 1
done

