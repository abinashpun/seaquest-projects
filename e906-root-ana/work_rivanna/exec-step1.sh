#!/bin/bash
DIR_MACRO=$(dirname $(readlink -f $BASH_SOURCE))

##
## Functions
##
function PrintHelp {
    cat <<-EOF
	$(basename $0):  Script to execute the 1st analysis step on local or GRID.

	Dataset 6 is analyzed by default.  The list of runs is taken from
	the E906 data catalog (i.e. e906-data-catalog run-list 6).

	Typical usage:
	  $0 -n 3
	  Analyze the first three runs on local computer.

	  $0 -n 4-100
	  Analyze the 4th-100th runs on local computer.

	  $0 -g -n 101-
	  Analyze the 101th-last runs on grid.

	Options:
	  -j job_name       | Change the job name.  Default = 'default'.
	  -r 12345 -r 12346 | Analyze runs 12345 and 12346.
	  -l list_run.txt   | Read the run list from 'list_run.txt'.
	  -n 4-100          | Set the number (range) of the runs to be analyzed.
	  -e 9999           | Analyze only 9999 events per run.
	  -g                | Use the Grid computing.
	EOF
}

##
## Main
##
if [ -z $E906_TOP ] ; then
    echo "ERROR:  E906_TOP is not defined.  Abort."
    exit
fi

JOB_NAME=default
DATASET=6 # 57, 59, 62, 67, 70, 5a, 5b, 6
RUN_TGT=()
N_EVT=0
RUN_LIST=
RUN_B=85
RUN_E=87
USE_GRID=no
OPTIND=1
while getopts ":j:r:l:n:e:g" OPT ; do
    case $OPT in
	j ) JOB_NAME=$OPTARG ;;
	r ) RUN_TGT+=($OPTARG) ;;
        l ) RUN_LIST=$OPTARG ;;
        n )    RUN_E=$OPTARG ;;
        e )    N_EVT=$OPTARG ;;
        g ) USE_GRID=yes     ;;
	* ) PrintHelp ; exit ;;
    esac
done
shift $((OPTIND - 1))
echo "JOB_NAME = $JOB_NAME"
echo "DATASET  = $DATASET"
echo "N_EVT    = $N_EVT"
echo "USE_GRID = $USE_GRID"

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
DIR_DATA=/scratch/$USER/e906-root-ana
ln -nfs $DIR_DATA data  # For convenience
echo "Data directory = $DIR_DATA"

##
## Loop over the runs
##
for (( RUN_I = 0 ; RUN_I < $RUN_N ; RUN_I++ )) ; do
    RUN=${RUN_TGT[$RUN_I]}
    echo "----------------------------------------------------------------"
    printf "Run %6d:  %4d / %4d\n" $RUN $(( RUN_I + 1 )) $RUN_N
    RUN6=$(printf '%06d' $RUN)

    FN_RAW=/project/ptgroup/seaquest/data/digit/${RUN6:0:2}/${RUN6:2:2}/digit_${RUN6}_009.root
    FN_REC=/project/ptgroup/seaquest/data/vertex/${RUN6:0:2}/${RUN6:2:2}/vertex_${RUN6}_r1.7.0.root
    test ! -e $FN_RAW && echo "  Cannot find FN_RAW ($FN_RAW)!" && continue
    test ! -e $FN_REC && echo "  Cannot find FN_REC ($FN_REC)!" && continue

    #DIR_JOB=$DIR_DATA/$JOB_NAME/$RUN
    DIR_JOB=$DIR_DATA/$JOB_NAME/${RUN6:0:2}/${RUN6:2:2}/${RUN6:4:2}
    rm -rf   $DIR_JOB
    mkdir -p $DIR_JOB

    cd $DIR_MACRO
    cp -a *.C ../setup.sh ../inst ../opts  $DIR_JOB

    cd $DIR_JOB
    if [ $USE_GRID == yes ] ; then
	{
	    echo "#!/bin/bash"
	    echo "root.exe -b 'AnaStep1.C($RUN, \"$FN_RAW\", \"$FN_REC\", $N_EVT)'"
	} >e906-ana.sh
	sbatch  --ntasks=1  --ntasks-per-node=1  --time=03:30:00 \
	    --output=slurm.out  --error=slurm.err \
	    --partition=standard  --account=spinquest  e906-ana.sh
    else
	root.exe -b "AnaStep1.C($RUN, \"$FN_RAW\", \"$FN_REC\", $N_EVT)"
    fi
done
