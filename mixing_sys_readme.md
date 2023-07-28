- From the `/seaquest/users/apun/e906_projects/mixing_check/e906-root-ana/`, copy the `src` , `opt` directories and `setup.sh` file.
- create your mixing systematic study directory and go inside the directory 
- From the `/seaquest/users/apun/e906_projects/mixing_check/e906-root-ana/kei_test` copy the directories `macros` and `cuts`.
  * Also create the all the directories and sub-directories (don't copy) as in kei_test
- `cuts\cuts.h`: cuts for the GMC dimuons (signal) related your analysis
- `macros` directory contains all the macros and scripts.
    * `exec-step1-sub.sh`, `exec-step1.sh`, `AnaStep1.C`, `MacroCommon.C` serves the same legacy puropose.
- Now follow the following steps;
    1. Prepare MC for embeddings ( `RunPrepareMC.C`): reads the vertexed MC file (`save` tree) and generates output mcfile (in directory `../gmc_files/`) for embedding.
        
        Related function: `AnaSortMixVertex::PrepareMC`. **One needs to apply all the analysis cuts you want to implement for your signal. This can be implemented modifying cuts\cuts.h**
    1. Embed the tracks correpsonding to MC dimuons ( `RunEmbed.C`): Reads the input generated from the previous step. 

        Related function: `AnaSortMixVertex::EmbedMCSignal`
        - can choose where to embed (i.e. sort_tree or mixed_tree) manually in the code. 
        - choose the index of the MC tree you want to start embedding from `RunEmbed.C` macro itself
        - 
