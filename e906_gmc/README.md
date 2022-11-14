# e906-GMC with root
A documentation of guide to run the GMC in e906 framework with root method (i.e. without using SQL Database directly developed by Kei). 

The GMC is performed in multisteps in e906.
1. Generation
2. Realization
3. Embedding NIM3 Hits
4. Tracking and Reconstruction
5. Vertexing

## Generation

One need not have to build from scratch as there is already standard kTracker library available. Following scripts are needed to be sourced (EXACTLY THE ORDER) in your `seaquestgpvm` machine to set the proper enviornment variables.

```
source /e906/app/software/osg/software/current/setup.sh
``` 

The script `runGMC.py` for submitting the gereration is available in `kei_dev` branch of the ["SeaQuest Distribution"](https://cdcvs.fnal.gov/redmine/projects/seaquest-ktracker/repository/revisions/kei_dev/show). 

Copy the script in your working area and run it as follows;
```
./runGMC.py --grid --preset=run3 --Record=ROOT --server=e906-db3.fnal.gov --raw-name=gmc_name --n-events=1000 --n-subruns=100 --Target=H --EventPosition=Dump --Generator=DY --Acceptance=Acc --grid-args="--expected-lifetime=24h" - --outdir=/path/to/output --first-subrun=0
``` 
You can tune the arguments as you need. Some of the main arguments are;\
`preset=run3` sets the magnet polarity, the beam offset, and the geometry\
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

In this step, the hits' position is changed randomly (following gaussian distribution with resolution of 400 $\mu m$) to mimic the resolution of the detector and also some of them are dropped to mimic the efficiency (94%) of detecor. The script `process_realization.sh` is modified and new script `process_all_realization.sh` is added to realize multiple files at the same time.

```
./process_all_realization.sh
```

Input: Truth info from generated step\
Output:` *_real.root`
 
### Embedding

In this step, the NIM3 (minimum Biased) trigger hits are embeded to the realized hits to mimic the real data scenario. Two kinds of methods of Embedding are available, occupancy (Jason's method) and intensity (Kei's method) based. For our study, we use the Kei's method. Details on Kei's method is documented in [DocDB-2630](https://seaquest-docdb.fnal.gov/cgi-bin/sso/RetrieveFile?docid=2630&filename=messyMC.pdf&version=2). New script `process_all_embed.sh` is added to embed multiple file. 

```
./process_all_embed.sh
```

Input: Output from realization step (`*real.root`)\
Output:` *_real_messy.root` and `*_real_clean.root`

## Tracking and Reconstruction

In this step, the embedded hits are used to create muon tracks from ktraker method. The script for running the tracking is `seaquest-ktracker/scripts/grid/submitAll.py`.  

```
source /e906/app/software/osg/software/current/setup.sh
source /cvmfs/seaquest.opensciencegrid.org/seaquest/software/SL7/seaquest/kTrackerRun5/setup.sh
```

Follow EXACTLY same order to source the scripts above and run the tracking script as

```
 ./submitAll.py -m -j track -l list_track.txt -c tracking.conf -n 1 -s 20
```
- -n argument is to split the input file into the multiple jobs if needed
- -s argument is number of input file passed
- tracking.conf: configuration file for tracking. It inludes the input/output file path and .opt files for reconstruction. All these should be placed in \pnfs\ area to get access from grid nodes.
- list\_track.txt: list of files for input (choose ` *_real_messy.root` and/or `*_real_clean.root`). The sample script to create such list is provided as `make-input-list.sh`

 
## Vertexing
This is the final step where two tracks are combined to form a dimuon. The output from tracking (`track_*.root`)is used as input for this process. The method of running this process is same as the tracking process, only difference being using `vertex` argument.

```
 ./submitAll.py -m -j vertex -l list_vertex.txt -c vertex.conf -n 1 -s 20
```
### More Documentation
More documentation from Kei: [DocDB-10057](https://seaquest-docdb.fnal.gov/cgi-bin/sso/RetrieveFile?docid=10057&filename=2022-06-28%20.pdf&version=3)
