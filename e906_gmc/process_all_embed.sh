#!/bin/bash
source ../../setup.sh
for i in {000..0019} #adjust the range according to your input
do
    FILENAME_BASE=realized_filename_00${i}_real
    echo $FILENAME_BASE 
    ./process_embed_inte.sh ${FILENAME_BASE}
    sleep 1
done

