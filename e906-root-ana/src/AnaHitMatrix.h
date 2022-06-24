#ifndef _ANA_HIT_MATRIX__H_
#define _ANA_HIT_MATRIX__H_
#include <map>
#include "AnaBase.h"
class TH1;

/// Analysis module to generate the hit matrix data used at UVA.
class AnaHitMatrix : public AnaBase {
  static const int N_DET = 30; // chamber only
  static const int N_ELE = 201; // D1 is not considered
  
  TH1* h1_count; // event counts

  /// Output
  TTree* m_tree_fpga1;
  TTree* m_tree_nim3;
  int    m_run_id;
  int    m_spill_id;
  int    m_event_id;
  int    m_target_pos;
  int    m_rf_intensity;
  bool   m_hit_matrix[N_DET][N_ELE]; // [plane][element]

 public:
  AnaHitMatrix(const std::string name="ana_hit_matrix");
  virtual ~AnaHitMatrix();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();

  void SetUpTree(TTree*& tree, const std::string label);
};

#endif // _ANA_HIT_MATRIX__H_
