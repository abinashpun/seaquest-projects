/// ROOT macro for making output format consitent with uiuc.
/**
 * This step processes the outputs of all runs processed by the 1st step (`AnaStep1.C`).
 * Therefore you should include in this step
 *  - Variable parts of your analysis (like analysis-cut tuning and plot painting).
 *
 * The contents of the analysis are implemented in the analysis modules.
 */
void RunFinalTree()
{
  gSystem->Load("libE906Ana.so");
  const char* dir_data="/pnfs/e906/scratch/users/apun/e906-root-ana/target_all"; //Area for reco files (save_sorted, save_mix)
  const string ds="67"; //roadset
  const verb=1;
  auto ana = new FinalTree();
  //ana->InitStep2();


  RunList run_list(ds);
  run_list.Verbosity(verb); // 0=quiet, 1=normal, 2=verbose

 for (int i_run = 0; i_run < run_list.GetN(); i_run++) {
    int run_id;
    TFile* file;
    if (run_list.OpenRunFile(dir_data, i_run, run_id, file) != 0) continue;

    ana->AnalyzeData_Mix(run_id, file);
  }

 // const char* gmcfile = "/seaquest/users/apun/e906_projects/gmc_m027/vertex_mc_drellyan_dump_M027_S001_messy.root"; 
 // const char* gmcfile = "/seaquest/users/mhossain/SeaQuest-Ana/NMSU-e906-ana/nmsu_e906_ana/e906-root-ana/work_gpvm/work_macro/run_200and300_dy_beamdump.root";
/*
  const char* gmcfile = "/pnfs/e906/persistent/users/knagai/E906/MC/M027_S001/embed/raw/vertex_mc_jpsi_dump_M027_S001_messy.root";
  ana->AnalyzeGMC(gmcfile);
  //ana->EndStep2();
*/
  delete ana;
  exit(0);
}
