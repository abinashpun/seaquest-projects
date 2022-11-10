# e906-GMC with root
A documentation of guide to run the GMC in e906 framework with root method (i.e. without using SQL Database directly developed by Kei). 

The GMC is performed in multisteps in e906.
1. Generation
2. Realization
3. Embedding NM4 Hits
4. Tracking and Reconstruction
5. Vertexing

## Generation

One need not have to build from scratch as there is already standard kTracker library available. Following scripts are needed to be sourced (EXACTLY THE ORDER) in your `seaquestgpvm` machine to set the proper enviornment variables.

```
source /e906/app/software/osg/software/current/setup.sh
source /cvmfs/seaquest.opensciencegrid.org/seaquest/software/SL7/seaquest/kTrackerRun5/setup.sh
``` 

The script `runGMC.py` for submitting the gereration is available in `kei_dev` branch of the ["SeaQuest Distribution"](https://cdcvs.fnal.gov/redmine/projects/seaquest-ktracker/repository/revisions/kei_dev/show). 
Copy the script in your working area and run it as follows;
```
./runGMC_root.py --grid --server=e906-db4.fnal.gov --port=3306 --raw-name=test_delete --n-events=100000 --n-subruns=5 --geometry=geometry_G18_run3 --gmc-args="/set/beamYOffset 1.6 cm" --Target=H --EventPosition=Dump --Generator=JPsi --Acceptance=Acc --gmc-args="/set/fmagMultiplier -1.044" --gmc-args="/set/kmagMultiplier -1.025" --grid-args="--expected-lifetime=6h" --gmc-args="/set/recordTo root" --outdir=/pnfs/e906/scratch/users/apun/GMC/generated --first-subrun=0 --osg
``` 
You can tune the arguments as you need. Some of the main arguments are;\
Generator: Here you choose which process you want to run for the dimuon genration. Possible options are "DY/JPsi/PsiPrime" \ 
Acceptance: Choose you want to get the particles in 4-pi (4pi) or rough detector acceptance (acc).\
first-subrun: Sets the random seed for your subruns.

More about the argument and process is explained in detail on [SeaQuest Monte Carlo wiki page](https://cdcvs.fnal.gov/redmine/projects/seaquest-gmc/wiki/Running_GMC_on_Fermigrid) by Kei.


