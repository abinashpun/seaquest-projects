#!/bin/bash
cd $KTRACKER_ROOT

TARG=("dummy" "LH2" "empty" "LD2" "NONE" "Fe" "C" "W")

command=eventMixer_inte
############ EDIT HERE ############
FILENAME_BASE=$1
OPT=opts/default.opts

INPUT_FILE_PATH=/path/to/realized/input
OUTPUT_FILE_PATH=/path/to/embed/output

TARGET=3

BACKGROUND_RANDOM_SEED=$(( ( RANDOM % 5 )  + 1 )) ### 1-5
############ EDIT HERE ############

INPUT_FILE=${INPUT_FILE_PATH}/${FILENAME_BASE}.root
OUTPUT_CLEAN=${OUTPUT_FILE_PATH}/${FILENAME_BASE}_clean.root
OUTPUT_MESSY=${OUTPUT_FILE_PATH}/${FILENAME_BASE}_messy.root

./${command} ${OPT} ${INPUT_FILE} ${OUTPUT_CLEAN} ${OUTPUT_MESSY} ${TARGET} ${BACKGROUND_RANDOM_SEED}

cd -

