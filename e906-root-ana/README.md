# User-Level Analysis Program for E906 Data on GPVM and Rivanna

A compact program to analyze the real data of E906.
This page (README.md) is to explain the usage of the program concretely.
You can also visit [this Doxygen document](https://e906-gat1.fnal.gov/e906-root-ana/doc/) to browse the contents and details of the program.
This program and its documentation are yet under development.
If you find any unclear/incorrect part, please let the contact know.


## Program Acquisition

The program is available on the Fermilab Redmine.
You can clone it as follows;

- On Fermilab GPVM

    ```
    kinit
    cd /path/to/your/working/directory
    git clone ssh://p-e906-root-ana@cdcvs.fnal.gov/cvs/projects/e906-root-ana
    ```

- On UVA Rivanna

    ```
    source /project/ptgroup/seaquest/setup.sh
    kinit your_Fermilab_Kerberos_name
    cd /path/to/your/working/directory
    git clone ssh://p-e906-root-ana@cdcvs.fnal.gov/cvs/projects/e906-root-ana
    ```

You could ask the contact person to make you a developer of this program.


## Software Environment

The software environment needed by this program is specified in `setup.sh`.
You have to source it every time you open a text terminal.

```
cd /path/to/your/e906-root-ana
source setup.sh
```


## Information on E906 Real Data

This program utilizes the E906 Data Catalog (https://e906-gat1.fnal.gov/e906dc/doc/) to obtain the lists of runs, spills, data files, etc.
Later you could look at the online document to learn details.

After sourcing `setup.sh`, you can use the command-line interface (`e906-data-catalog`).
For example, you execute the following commands to find that the 1st command prints out a list of the datasets and the 2nd command a (long) list of runs in roadset 67.

```
e906-data-catalog ds-list
e906-data-catalog run-list 67
```

Probably you will often need to look at the run list to be analyzed, which can be printed out (with line index) by the following command;

```
e906-data-catalog run-list 67 | less -N
```

Some more information is posted on this Confluence page;
https://confluence.its.virginia.edu/display/twist/E906+Analysis+on+Rivanna


## Source Build

The source code of this program is located under `src/`.
You simply execute `cmake-this` and `make-this` at any directory to build the source code in a proper way.
You will see the library file, `inst/lib/libE906Ana.so`, created.

```
cmake-this
make-this
```


## Analysis Procedure

The analysis procedure in this program is devided into two steps.
The 1st step is to read the data file(s) of one run to carry out run-by-run analysis.
It can/should be executed for multiple runs in parallel, because we have O(1000) of runs per dataset.
Thus this step should include

- Time-consuming parts of your analysis (like event reconstruction) and
- Stable parts of your analysis (like basic analysis cuts).

The 2nd step is to process the outputs of all runs created by the 1st step.
Here we can combine the statistics of all the runs or check the run dependence of variables.
Thus this step should include

- Variable parts of your analysis (like analysis-cut tuning and plot painting).

A set of commands used in these steps varies with the software environment that you use (i.e. GPVM or Rivanna).
You now cd to `work_gpvm` or `work_rivanna` and follow `README.md` in it.
You will find that `AnaStep1.C` and `AnaStep2.C` are for the two steps.

- [work_gpvm/README.md](work_gpvm/README.md)
- [work_rivanna/README.md](work_rivanna/README.md)


## Advanced Information

You will have to modify a part of this program for the purpose of your analysis.
You are expected to read and understand all files in this program step-by-step.
Below are some information to help you understand the program structure.


### `cmake-this` and `make-this`

`cmake` and `make` are the standard tools to configure and build a source code.
`cmake-this` and `make-this` are shell functions that execute `cmake` and `make` with proper options for this analysis program.
They are defined in `setup.sh`.

You have to execute `cmake-this` mainly when you add/delete files under `src/`.
It cleans up all files in the build & install directories.

You have to execute `make-this` when you modify files under `src/`.


### Analysis Module

The methods of all analyses are implemented in the source code under `src/`.
`AnaBase.(h|cc)` is the base class that provides the basic function for analysis.
One analysis method is implemented as one analysis class, called "analysis module", that is inherited from `AnaBase`.
You find several pre-defined modules under `src/`;

* `AnaDimuon.(h|cc)`
* `AnaHitMatrix.(h|cc)`
* `AnaHodoTime.(h|cc)`
* `AnaNim3.(h|cc)`
* `AnaOccupancy.(h|cc)`

You can enable one of the analysis modules in `AnaStep[12].C`.
`AnaHodoTime` is selected by default.


### How to Implement Your Analysis

You can either directly modify one of the existing analysis modules,
or create a new one (by copying from the one that is most similar to your analysis).
When you create a new module, you have to add it to `LinkDef.h`.
You then compile it, by executing `cmake-this` and `make-this`.

There are additional classes/namespaces that provide common information/function for analysis.
They are not much documented yet, but you can find how they are used in the existing analysis modules.

* List of good runs and spills --- `E906RunCatalog.(h|cc)`, `E906RunInfo.(h|cc)`, `E906SpillCatalog.(h|cc)`
* Utility functions --- `UtilHist.(h|cc)`, `UtilBeam.(h|cc)`, `UtilTdc.(h|cc)`


## Contents of `GeomSvc`, `SRawEvent` and `SRecEvent`

These classes are defined externally, i.e. in kTracker.
Probably you often use their member functions to obtain the geometry info and the data variables.
You might use the following commands to quickly find the member functions;

* `less $KTRACKER_ROOT/inc/GeomSvc.h`
* `less $KTRACKER_ROOT/inc/SRawEvent.h`
* `less $KTRACKER_ROOT/inc/SRecEvent.h`


## Contact

Kenichi Naknao
