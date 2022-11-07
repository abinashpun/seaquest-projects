#ifndef _ANA_NIM3__H_
#define _ANA_NIM3__H_
#include <map>
#include "AnaBase.h"
#include "TreeData.h"
class TFile;
class TTree;
class TH1;

/// @private
struct Nim3Data {
  int   run_id;
  int spill_id;
  int  tgt_pos;
  int event_id;
  int  turn_id;
  int    rf_id;
  int rfp01; ///< RF+01
  int rfp00; ///< RF+00
  int rfm01; ///< RF-01
  float rfp01c; ///< RF+01 corrected for the pedestal
  float rfp00c; ///< RF+00 corrected for the pedestal
  float rfm01c; ///< RF-01 corrected for the pedestal
  float pot_p01;
  float pot_p00;
  float pot_m01;
  int occ_D0;
  int occ_D1;
  int occ_D2;
  int occ_D3;
  Nim3Data() {;}
  virtual ~Nim3Data() {;}
  ClassDef(Nim3Data, 1);
};

/// Analysis module to extract key variables of NIM3 events.
class AnaNim3 : public AnaBase {
  ///
  /// For step 1
  ///
  TH1* m_h1_count; // event counts
  TTree*   m_tree;
  Nim3Data m_data;

  ///
  /// For step 2
  ///
  ofstream m_ofs_inte;
  TFile* m_file_out2;
  TH2* m_h2_turn_rf[10]; // [tgt_pos]
  TH1* m_h1_rfp00  [10]; // [tgt_pos]
  TH1* m_h1_rfp00c [10]; // [tgt_pos]
  TH1* m_h1_pot_p00[10]; // [tgt_pos]
  TH1* m_h1_occ_D1 [10]; // [tgt_pos]

 public:
  AnaNim3(const std::string name="ana_nim3");
  virtual ~AnaNim3();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();
};

#endif // _ANA_NIM3__H_
