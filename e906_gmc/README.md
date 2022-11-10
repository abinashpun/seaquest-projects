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
./runGMC.py --grid --server=e906-db4.fnal.gov --port=3306 --raw-name=test_delete --n-events=100000 --n-subruns=5 --geometry=geometry_G18_run3 --gmc-args="/set/beamYOffset 1.6 cm" --Target=H --EventPosition=Dump --Generator=JPsi --Acceptance=Acc --gmc-args="/set/fmagMultiplier -1.044" --gmc-args="/set/kmagMultiplier -1.025" --grid-args="--expected-lifetime=6h" --gmc-args="/set/recordTo root" --outdir=/pnfs/e906/scratch/users/apun/GMC/generated --first-subrun=0 --osg
``` 
You can tune the arguments as you need. Some of the main arguments are;\
`Generator`: Here you choose which process you want to run for the dimuon genration. Possible options are DY/JPsi/PsiPrime\
`Acceptance`: Choose you want to get the particles in 4-pi (4pi) or rough detector acceptance (acc).\
`first-subrun`: Sets the random seed for your subruns.

More about the argument and process is explained in detail on [SeaQuest Monte Carlo wiki page](https://cdcvs.fnal.gov/redmine/projects/seaquest-gmc/wiki/Running_GMC_on_Fermigrid) by Kei.

## Realization and Embedding 

For realization and embedding you need to get `seaquest-ktracker` repository and build it.
```
git clone http://p-seaquest-ktracker@cdcvs.fnal.gov/cvs/projects/seaquest-ktracker
source /cvmfs/seaquest.opensciencegrid.org/seaquest/software/current/Distribution/setup/setup.sh
source seaquest-ktracker/setup.sh
``` 
For now you need to checkout to `kei_dev` branch and follow README.md of the repository. The scripts for running the realization (`process_realization.sh` and `process_embed_*.sh`) are found in the location: `seaquest-ktracker/scripts/embed`. These two methods are performed in gpvm machine itself. Embedding process takes relatively longer time than realization.

### Realization

In this step, the hits are moved randomly (following gaussian distribution) to mimic the resolution of the detector and also some of them are dropped to mimic the efficiency of detecor. The script `process_realization.sh` is modified and new script `process_all_realization.sh` is added to realize multiple files at the same time.

Input: Truth info from generated step\
Output:` *_real.root`
 
### Embedding

In this step, the NM3 (minimum Biased) trigger hits are added to the realized hits to mimic the real data scenario. Two kind of methods of Embedding are available, occupancy (Jason's method) and intensity (Kei's method) based. For our study, we use the Kei's method. Details on Kei's method is documented in [DocDB-2630](https://seaquest-docdb.fnal.gov/cgi-bin/sso/RetrieveFile?docid=2630&filename=messyMC.pdf&version=2). New script `process_all_embed.sh` is added to embed multiple file. 

Input: Output from realization step (`*real.root`)\
Output:` *_real_messy.root` and `*_real_clean.root`

## Tracking and Reconstruction


