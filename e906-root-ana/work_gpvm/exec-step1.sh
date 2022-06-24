#!/bin/bash
DIR_MACRO=$(dirname $(readlink -f $BASH_SOURCE))

##
## Functions
##
function PrintHelp {
    cat <<-EOF
	$(basename $0):  Script to execute the 1st analysis step on local or GRID.

	Dataset 67 is analyzed by default.  The list of runs is taken from
	the E906 data catalog (i.e. e906-data-catalog run-list 67).

	Typical usage:
	  $0 -n 3
	  Analyze the first three runs on local computer.

	  $0 -n 4-100
	  Analyze the 4th-100th runs on local computer.

	  $0 -g -n 101-
	  Analyze the 101th-last runs on grid.

	Options:
	  -d 70             | Set the dataset to '70' (default = '67').
	  -r 12345 -r 12346 | Analyze runs 12345 and 12346.
	  -l list_run.txt   | Read the run list from 'list_run.txt'.
	  -n 4-100          | Set the number (range) of the runs to be analyzed.
	  -e 9999           | Analyze only 9999 events per run.
	  -o                | Allow to overwrite existing output files.
	  -g                | Use the Grid computing.
	EOF
}

##
## Main
##
DATASET=67 # 57, 59, 62, 67, 70, 5a, 5b, 6
RUN_TGT=()
N_EVT=0
RUN_LIST=
RUN_B=1
RUN_E=3
DO_OVERWRITE=no
USE_GRID=no
OPTIND=1
while getopts ":d:r:l:n:e:og" OPT ; do
    case $OPT in
	d )  DATASET=$OPTARG ;;
	r ) RUN_TGT+=($OPTARG) ;;
        l ) RUN_LIST=$OPTARG ;;
        n )    RUN_E=$OPTARG ;;
        e )    N_EVT=$OPTARG ;;
        o ) DO_OVERWRITE=yes ;;
        g ) USE_GRID=yes     ;;
        * ) PrintHelp ; exit ;;
    esac
done
shift $((OPTIND - 1))
echo "DATASET      = $DATASET"
echo "N_EVT        = $N_EVT"
echo "DO_OVERWRITE = $DO_OVERWRITE"
echo "USE_GRID     = $USE_GRID"

##
## Make a list of runs to be analyzed
##
echo "Runs to be analyzed:"
if [ ${#RUN_TGT[*]} -eq 0 ] ; then
    if [ "$RUN_LIST" -a -e "$RUN_LIST" ] ; then
	echo "  RUN_LIST  = $RUN_LIST"
	RUN_TGT_ALL=( $(cat $RUN_LIST) )
    else
	echo "  e906-data-catalog run-list $DATASET"
	RUN_TGT_ALL=( $(e906-data-catalog run-list $DATASET) )
    fi
    RUN_N_ALL=${#RUN_TGT_ALL[*]}
    echo "  RUN_N_ALL = $RUN_N_ALL"

    if [ "${RUN_E%-*}" != "$RUN_E" ] ; then # Contain '-'
	RUN_B=${RUN_E%-*} # Before '-'
	RUN_E=${RUN_E#*-} # After '-'
    fi
    test -z "$RUN_B" || test $RUN_B -lt 1          && RUN_B=1
    test -z "$RUN_E" || test $RUN_E -gt $RUN_N_ALL && RUN_E=$RUN_N_ALL
    echo "  RUN_B...E = $RUN_B...$RUN_E"

    for (( RUN_I = $RUN_B ; RUN_I <= $RUN_E ; RUN_I++ )) ; do
	RUN_TGT+=( ${RUN_TGT_ALL[ $(( RUN_I - 1 )) ]} )
    done
fi
RUN_N=${#RUN_TGT[*]}
echo "  RUN_N   = $RUN_N"
if [ $RUN_N -le 10 ] ; then
    echo "  RUN_TGT = ${RUN_TGT[*]}"
else
    echo -n "  RUN_TGT = "
    for (( I = 0 ; I < 5 ; I++)) ; do echo -n " ${RUN_TGT[$I]}" ; done
    echo -n " ..."
    for (( I = $((RUN_N - 5)) ; I < $RUN_N ; I++)) ; do echo -n " ${RUN_TGT[$I]}" ; done
    echo
fi
echo -n "Is the configuration above OK (y/N)? "
read YESNO
if [ "X$YESNO" != 'Xy' -a "X$YESNO" != 'Xyes' ] ; then
    echo "Abort."
    exit 0
fi

##
## Set up the working directory
##
if [ $USE_GRID == yes ] ; then
    DIR_WORK=/pnfs/e906/scratch/users/$USER/e906-root-ana
    ln -nfs $DIR_WORK data # for convenience
else
    DIR_WORK=$DIR_MACRO/scratch
fi
echo "DIR_WORK = $DIR_WORK"

cd $DIR_MACRO
mkdir -p $DIR_WORK
rm -f    $DIR_WORK/input.tar.gz
tar czf  $DIR_WORK/input.tar.gz  *.C ../setup.sh ../inst ../opts

##
## Loop over the runs
##
for (( RUN_I = 0 ; RUN_I < $RUN_N ; RUN_I++ )) ; do
    RUN=${RUN_TGT[$RUN_I]}
    #test $RUN -lt 24279 && continue # No good spill before run 24279 at present
    echo "----------------------------------------------------------------"
    printf "Run %6d:  %4d / %4d\n" $RUN $(( RUN_I + 1 )) $RUN_N
    RUN6=$(printf '%06d' $RUN)

    FN_RAW=$(e906-data-catalog run-event-file  $RUN)
    FN_REC=$(e906-data-catalog run-vertex-file $RUN)
    test ! -e $FN_RAW && echo "  Cannot find FN_RAW ($FN_RAW)!" && continue
    test ! -e $FN_REC && echo "  Cannot find FN_REC ($FN_REC)!" && continue

    DIR_WORK_RUN=$DIR_WORK/${RUN6:0:2}/${RUN6:2:2}/${RUN6:4:2}
    if [ -e $DIR_WORK_RUN ] ; then
	echo -n "  DIR_WORK_RUN already exists."
	if [ $DO_OVERWRITE = yes ] ; then
	    echo "  Clean up."
	    rm -rf $DIR_WORK_RUN
	else
	    echo "  Skip."
	    continue
	fi
    fi

    mkdir -p $DIR_WORK_RUN/out
    cp -p $DIR_MACRO/exec-step1-sub.sh $DIR_WORK_RUN
    
    if [ $USE_GRID == yes ] ; then
	CMD="/e906/app/software/script/jobsub_submit_spinquest.sh"
	CMD+=" --expected-lifetime='short'" # medium=8h, short=3h, long=23h
	CMD+=" -L $DIR_WORK_RUN/log_exec-step1-sub.txt"
	CMD+=" -f $DIR_WORK/input.tar.gz"
	CMD+=" -f $FN_RAW"
	CMD+=" -f $FN_REC"
	CMD+=" -d OUTPUT $DIR_WORK_RUN/out"
	CMD+=" file://$DIR_WORK_RUN/exec-step1-sub.sh $DATASET $RUN $(basename $FN_RAW) $(basename $FN_REC) $N_EVT"
	$CMD |& tee $DIR_WORK_RUN/log_jobsub_submit.txt
	RET_SUB=${PIPESTATUS[0]}
	test $RET_SUB -ne 0 && exit $RET_SUB
    else
	export  CONDOR_DIR_INPUT=$DIR_WORK_RUN/in
	export CONDOR_DIR_OUTPUT=$DIR_WORK_RUN/out
	mkdir -p $DIR_WORK_RUN/in
	cp -p $DIR_WORK/input.tar.gz $DIR_WORK_RUN/in
	ln -nfs $FN_RAW $DIR_WORK_RUN/in
	ln -nfs $FN_REC $DIR_WORK_RUN/in
	mkdir -p $DIR_WORK_RUN/exe
	cd       $DIR_WORK_RUN/exe
	$DIR_WORK_RUN/exec-step1-sub.sh $DATASET $RUN $(basename $FN_RAW) $(basename $FN_REC) $N_EVT |& tee $DIR_WORK_RUN/log_exec-step1-sub.txt
    fi
done
