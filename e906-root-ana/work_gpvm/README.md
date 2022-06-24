# e906-root-ana/work_gpvm: Working Directory for GPVM 

It contains a set of shell scripts and ROOT macros for GPVM.
You could try the following commands to go through the whole procedure on GPVM.

## Analysis Step 1: Run-by-Run Analysis

`exec-step1.sh` is the top-level script that you execute for run-by-run analysis.
You first execute `exec-step1.sh` with no argument.

```
./exec-step1.sh
```

It by default analyzes the first three runs of dataset 67.
You are asked to check the runs to be analyzed, and thus you hit 'y' to proceed.
The script calls the sub-script, `exec-step1-sub.sh`, once per run.
The sub-script calls the ROOT macro, `AnaStep1.C`.
By default `AnaStep1.C` analyzes the hodoscope TDC time (via `AnaHodoTime`).
Output files are created under `scratch/01/25/2[789]/out/` etc.
You can look into them as follows for example;

```
cat scratch/01/25/27/out/output.txt
display scratch/01/25/27/out/h1_time_h1t.png &

root -l scratch/01/25/27/out/output.root
h1_time_h1t->Draw();
TBrowser l;
```


## Analysis Step 2: All-Run Analysis

Here you analyze the outputs of all the runs processed in the 1st step,
where you have only three processed runs for now but will have more later.
The ROOT macro, `AnaStep2.C`, reads all runs available under `scratch/` and 
outputs several plots+numbers under `result/`.
You can use `display` and `root` to look into the output files.

```
root -b AnaStep2.C

display result/gr_peak_h1t.png &

root -l result/result.root
gr_peak_h1t->Draw();
TBrowser l;
```


## Analysis of Multiple Runs

The 1st analysis step can be executed for multiple runs in parallel.
`exec-step1.sh` is able to do that, using the `jobsub_submit` command on GPVM.
It shows its usage via `-h` option:

```
./exec-step1.sh -h
```

You should first try processing the first 100 runs as follows,
where `-g` means the use of GRID and `-n 100` mean the 1st 100 runs;

```
./exec-step1.sh -g -n 100
```

You are asked to check the runs to be analyzed and hit 'y' to proceed.
You might have to execute the following commands if encountering an error about 'jobsub_submit';

```
source /e906/app/software/script/setup-jobsub-seaquest.sh
kinit
```

You then wait for all the jobs finished, by executing `jobsub_q --user=$USER` time-by-time.
The run-by-run output files will appear under `data/**/**/**/out/`.

You next execute the 2nd analysis step as follows,
where `data` is the output directory of the 1st analysis step executed on GRID;

```
root -b 'AnaStep2.C("data")'
```

It processes the outputs of the 100 runs that you have processed in the 1st step.
The output of the 2nd step appears under `result/`.

If everything above looks fine, 
You then process the rest of the available runs;

```
./exec-step1.sh -g -n 101-
```

You again use `jobsub_q` and `AnaStep2.C` to complete the analysis.


## Advanced Usage

### How to Analyze Other Datasets

In the 1st step, the `-d` option is to change the dataset.
For example `./exec-step1.sh -d 6` targets dataset 6.
In the 2nd step, the 2nd argument of `AnaStep2.C` is to change the dataset.

- `root -b 'AnaStep2.C("scratch", "6")'`
- `root -b 'AnaStep2.C("data", "6")'`


### How to Change Analysis Module

The selection of analysis module is done in `MacroCommon.C`.
You uncomment the one that you want to use.
Note that only one analysis module must be selected.
Multiple modules cannot run at once.

...to be written more...

