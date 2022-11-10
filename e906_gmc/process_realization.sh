!/bin/bash

cd $KTRACKER_ROOT

TARG=("dummy" "LH2" "empty" "LD2" "NONE" "Fe" "C" "W")

command=realization

FILENAME_BASE=$1

############ EDIT HERE ############
OPT=opts/realization.opts
INPUT_FILE_PATH=/path/to/generated/input
OUTPUT_FILE_PATH=/path/to/realized/output
SEED=1 #### random seed
############ EDIT HERE ############

INPUT_FILE=${INPUT_FILE_PATH}/${FILENAME_BASE}.root
OUTPUT_FILE=${OUTPUT_FILE_PATH}/${FILENAME_BASE}_real.root

./${command} $OPT $INPUT_FILE $OUTPUT_FILE $SEED

cd -

