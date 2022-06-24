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
  Nim3Data() {;}
  virtual ~Nim3Data() {;}
  ClassDef(Nim3Data, 1);
};

/// Analysis module to extract key variables of NIM3 events.
class AnaNim3 : public AnaBase {
  /// Output
  TH1* m_h1_count; // event counts

  TTree*   m_tree;
  Nim3Data m_data;

  TH1* m_h1_rfp01;
  TH1* m_h1_rfp00;
  TH1* m_h1_rfm01;

  TH1* m_h1_pot_p01;
  TH1* m_h1_pot_p00;
  TH1* m_h1_pot_m01;

 public:
  AnaNim3(const std::string name="ana_nim3");
  virtual ~AnaNim3();
  void Init();
  void End();

 protected:
  void AnalyzeEvent();
};

#endif // _ANA_NIM3__H_
