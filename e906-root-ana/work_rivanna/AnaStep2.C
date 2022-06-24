/// ROOT macro for the 2nd analysis step.
/**
 * This step processes the outputs of all runs processed by the 1st step (`AnaStep1.C`).
 * Therefore you should include in this step
 *  - Variable parts of your analysis (like analysis-cut tuning and plot painting).
 *
 * The contents of the analysis are implemented in the analysis modules.
 */
void AnaStep2(const int verb=1)
{
  const string ds = "6";
  const string dir_data = "data/default";
  gSystem->Load("libE906Ana.so");
  gROOT->ProcessLine(".x MacroCommon.C");

  ana->InitStep2();

  RunList run_list(ds);
  run_list.SelectEnv("rivanna");
  run_list.Verbosity(verb); // 0=quiet, 1=normal, 2=verbose
  for (int i_run = 0; i_run < run_list.GetN(); i_run++) {
    int run_id;
    TFile* file;
    if (run_list.OpenRunFile(dir_data, i_run, run_id, file) != 0) continue;

    ana->AnalyzeStep2(run_id, file);
  }

  ana->EndStep2();
  delete ana;
  exit(0);
}
