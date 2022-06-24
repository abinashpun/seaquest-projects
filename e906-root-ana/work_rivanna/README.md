# e906-root-ana/work_rivanna: Working Directory for Rivanna

It contains a set of shell scripts and ROOT macros for Rivanna.
You could first try the following commands to go through the whole procedure on Rivanna.

## Analysis Step 1: Run-by-Run Analysis

`exec-step1.sh` is the main script that you execute for run-by-run analysis.
You first execute `exec-step1.sh` with no argument.

```
./exec-step1.sh
```

It by default analyzes runs 24279, 24281 and 24282 (which are the 85-87th runs in dataset 6) on the local computer.
You are asked to check the runs to be analyzed, and thus you hit 'y' to proceed.
The script calls the ROOT macro, `AnaStep1.C`.
By default `AnaStep1.C` analyzes the hodoscope TDC time (via `AnaHodoTime`).
Output files are created under `data/default/02/42/79/` etc.
You can look into them as follows for example;

```
cat     data/default/02/42/79/output.txt
display data/default/02/42/79/h1_time_h1t.png &
caja    data/default/02/42/79

root -l data/default/02/42/79/output.root
h1_time_h1t->Draw();
TBrowser l;
```

Note that the first 84 runs in dataset 6 contain quite few analyzable events.
Therefore they are not analyzed above and below.


## Analysis Step 2: All-Run Analysis

Here you analyze the outputs of all the runs processed in the 1st step, 
where you have only three processed runs for now but will have more later.
The ROOT macro, `AnaStep2.C`, reads all runs available under `data/default/` and 
outputs several plots+numbers under `result/`.
You can use `display` and `root` to look into the output files.

```
root -b AnaStep2.C

display result/gr_peak_h1t.png &
caja    result

root -l result/result.root
gr_peak_h1t->Draw();
TBrowser l;
```


## Analysis of Multiple Runs

The 1st analysis step can be executed for multiple runs in parallel.
`exec-step1.sh` is able to do that, using the `sbatch` command on Rivanna.
It shows its usage via `-h` option:

```
./exec-step1.sh -h
```

You should first try processing 100 runs as follows;

```
./exec-step1.sh -g -n 88-187
```

One job is submitted per run to execute `AnaStep1.C`.
You then execute `squeue-mine` time-by-time until all the 100 jobs finish.
The output files will appear under `data/default/`.

You next execute the 2nd analysis step;

```
root -b AnaStep2.C
```

It processes the outputs of the 103 runs that you have processed in the 1st step.
The output of the 2nd step appears under `result/`.

If everything above looks fine, 
You then process the rest of the available runs;

```
./exec-step1.sh -g -n 188-
```

You again use `squeue-mine` and `root -b AnaStep2.C` to complete the analysis.
