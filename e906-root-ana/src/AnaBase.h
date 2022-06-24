#ifndef _ANA_BASE__H_
#define _ANA_BASE__H_
#include <fstream>
#include <SRawEvent.h>
#include <SRecEvent.h>
class TFile;
class TTree;

/// Base class of all analysis modules.
class AnaBase {
  /// Parameter
  std::string m_name;
  std::string m_dir_out;

  std::string m_ds; ///< Dataset
  int  m_run_id; ///< Run ID
  bool m_use_raw; ///< true if using SRawEvent 
  bool m_use_rec; ///< true if using SRecEvent
  unsigned int m_n_evt_max; ///< N of max events to be analyzed per run
  int m_verb; ///< Verbosity

  /// Input
  SRawEvent* m_raw;
  SRecEvent* m_rec;

  /// Output
  std::ofstream m_ofs;
  TFile* m_file_out;

 protected:
  AnaBase(const std::string name);

 public:
  virtual ~AnaBase();
  virtual void Init();
  virtual void End();
  virtual void Analyze(const std::string fn_raw, const std::string fn_rec="");

  virtual void InitStep2();
  virtual void AnalyzeStep2(const int run_id, TFile* file_in);
  virtual void EndStep2();

  /// Get the name of analysis module.
  std::string Name() const { return m_name; }

  /// Set the output directory.
  void SetOutputDir(const std::string dir_out) { m_dir_out = dir_out; }

  /// Set the output directory using the given integer (usually run ID).
  void SetOutputDir(const int dir_out) { m_dir_out = std::to_string(dir_out); }

  /// Get the output directory.
  std::string GetOutputDir() const { return m_dir_out; }

  void SetDataset(const std::string ds) { m_ds = ds; } ///< Set the dataset to be analyzed.
  std::string GetDataset() const { return m_ds; }      ///< Get the dataset to be analyzed.

  void SetRunID(const int run_id) { m_run_id = run_id; } ///< Set the run ID to be analyzed.
  int  GetRunID() const { return m_run_id; }             ///< Get the run ID to be analyzed.

  void UseRawEvent(const bool value) { m_use_raw = value; } ///< Turn on/off the use of SRawEvent.
  void UseRecEvent(const bool value) { m_use_rec = value; } ///< Turn on/off the use of SRecEvent.

  /// Set the max number of events to be analyzed per run.
  void SetNumEventsAnalyzed(const unsigned int n_evt) { m_n_evt_max = n_evt; }

  void Verbosity(const int verb) { m_verb = verb; } ///< Set the verbosity.
  int  Verbosity() const  { return m_verb; }        ///< Get the verbosity.

  /// Get the ofstream object for output.  Usable in `Init()`, `AnalyzeEvent()` and `End()` of each analysis module.
  std::ofstream& OFS() { return m_ofs; }

  /// Get the TFile object for output.  Usable in `Init()`, `AnalyzeEvent()` and `End()` of each analysis module.
  TFile* OFile() { return m_file_out; }

 protected:
  SRawEvent* GetRawEvent() { return m_raw; } ///< Get a pointer to `SRawEvent`.  Usable in `AnalyzeEvent()`.
  SRecEvent* GetRecEvent() { return m_rec; } ///< Get a pointer to `SRecEvent`.  Usable in `AnalyzeEvent()`.
  virtual void AnalyzeEvent();
};

#endif // _ANA_BASE__H_
