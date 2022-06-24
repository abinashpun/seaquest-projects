#ifndef _RUN_LIST__H_
#define _RUN_LIST__H_
#include <string>
#include <vector>
class TFile;

/// Helper class to handle the run list in ROOT macro.
class RunList {
  std::string m_env; ///< Software environment.  "gpvm" or "rivanna".
  std::string m_ds;
  int m_verb;
  TFile* m_file;
  std::vector<int> m_list_run;

 public:
  RunList(const std::string ds);
  virtual ~RunList();

  /// Select the environment.
  void SelectEnv(const std::string env) { m_env = env; }

  /// Get the number of runs contained.
  int GetN() const { return m_list_run.size(); }

  /// Get the run ID at `idx`.
  int GetRun(const int idx) { return m_list_run[idx]; }

  /// Open the input file for the run at `index` under `dir_base`.
  int OpenRunFile(const std::string dir_base, const int index, int& run_id, TFile*& file);

  void Verbosity(const int verb) { m_verb = verb; } ///< Set the verbosity.
  int  Verbosity() const  { return m_verb; }        ///< Get the verbosity.
};

#endif // _RUN_LIST__H_
