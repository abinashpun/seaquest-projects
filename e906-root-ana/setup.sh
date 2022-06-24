DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
DIR_BUILD=$DIR_TOP/build
DIR_INST=$DIR_TOP/inst

if [ -e /e906/app/software/osg ] ; then # GPVM
    export E906_TOP=n/a # Probably not necessary
    export  E906_SOFTWARE=/e906/app/software/osg/software
    source $E906_SOFTWARE/SL7/Distribution/setup/setup.sh
    source $E906_SOFTWARE/SL7/seaquest/kTrackerRun6/setup.sh
    source $E906_SOFTWARE/e906dc/this-catalog.sh
elif [ -e /cvmfs/seaquest.opensciencegrid.org/seaquest ] ; then # GRID
    export E906_TOP=n/a # Probably not necessary
    export  E906_SOFTWARE=/cvmfs/seaquest.opensciencegrid.org/seaquest/software
    source $E906_SOFTWARE/SL7/Distribution/setup/setup.sh
    source $E906_SOFTWARE/SL7/seaquest/kTrackerRun6/setup.sh
    source $E906_SOFTWARE/e906dc/this-catalog.sh
elif [ -e /project/ptgroup/seaquest ] ; then # UVA Rivanna
    source /project/ptgroup/seaquest/setup.sh
else
    echo "Cannot find a proper directory for E906_SOFTWARE."
    return 1
fi
export LD_LIBRARY_PATH=$DIR_TOP/inst/lib:$LD_LIBRARY_PATH

##
## Functions for source build
##
function cmake-this {
    if [ -e $DIR_BUILD ] ; then
	echo "Clean up the build directory..."
	\rm -rf $DIR_BUILD
    fi
    if [ -e $DIR_INST ] ; then
	echo "Clean up the install directory..."
	\rm -rf $DIR_INST
    fi
    mkdir -p $DIR_BUILD
    echo "Run cmake..."
    ( cd $DIR_BUILD && cmake -DCMAKE_INSTALL_PREFIX=$DIR_INST $DIR_TOP/src )
    local RET=$?
    if [ $RET -eq 0 ] ; then
	echo "OK.  Run 'make-this' at any time/directory."
    else
	echo "NG.  Need a fix."
    fi
    return $RET
}

function make-this {
    if [ ! -e $DIR_BUILD/Makefile ] ; then
	echo "The build directory is not ready.  Complete 'cmake-this' first."
	return 1
    fi
    ( cd $DIR_BUILD && make install )
    return $?
}
