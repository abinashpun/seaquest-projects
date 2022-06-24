/// ROOT macro for the 1st analysis step.
/**
 * This step processes one run given as the 1st argument (run_id).
 * It is executed for multiple runs in parallel.
 * Therefore you should include in this step
 *  - Time-consuming parts of your analysis (like event reconstruction) and
 *  - Stable parts of your analysis (like basic analysis cuts).
 *
 * The contents of the analysis are implemented in the analysis modules.
 * You find or create all analysis modules under `src/`, such as `AnaHodoTime`.
 * You select one of them in `MacroCommon.C`.
 */
void AnaStep1(const string ds, const int run_id, const string fn_raw, const string fn_rec, const int n_evt=0)
{
  gSystem->Load("libE906Ana.so");
  gROOT->ProcessLine(".x MacroCommon.C");

  ana->SetDataset(ds);
  ana->SetRunID(run_id);
  ana->SetNumEventsAnalyzed(n_evt);

  ana->Init();
  ana->Analyze(fn_raw, fn_rec);
  ana->End();
  delete ana;
  exit(0);
}
