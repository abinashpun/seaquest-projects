#!/bin/bash
DS=$1
RUN=$2
FN_RAW=$3
FN_REC=$4
N_EVT=$5

if [ -z "$CONDOR_DIR_INPUT" -o -z "$CONDOR_DIR_OUTPUT" ] ; then
    echo "!ERROR!  CONDOR_DIR_INPUT/OUTPUT is undefined.  Abort."
    exit 1
fi
echo "INPUT  = $CONDOR_DIR_INPUT"
echo "OUTPUT = $CONDOR_DIR_OUTPUT"
echo "HOST   = $HOSTNAME"
echo "PWD    = $PWD"

tar xzf $CONDOR_DIR_INPUT/input.tar.gz
source setup.sh
echo "E906_SOFTWARE = $E906_SOFTWARE"

touch timestamp.txt
time root.exe -b -l -q "AnaStep1.C(\"$DS\", $RUN, \"$CONDOR_DIR_INPUT/$FN_RAW\", \"$CONDOR_DIR_INPUT/$FN_REC\", $N_EVT)"
RET=$?
if [ $RET -ne 0 ] ; then
    echo "Error in Fun4All.C: $RET"
    exit $RET
fi

find . -mindepth 1 -maxdepth 1 -newer timestamp.txt -exec mv {} $CONDOR_DIR_OUTPUT \;

echo "exec-step1-sub.sh finished!"
