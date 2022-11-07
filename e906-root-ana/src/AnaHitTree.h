#ifndef _ANA_HIT_TREE__H_
#define _ANA_HIT_TREE__H_
#include <map>
#include "AnaBase.h"
class TH1;

/// Analysis module to generate the hit-tree data used at UVA.
class AnaHitTree : public AnaBase {
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
  short  m_n_hits;
  short  m_det_id    [9999];
  short  m_ele_id    [9999];
  float  m_drift_dist[9999];

  ///
  /// For step 2
  ///
  TFile* m_file_out2;
  TH2* m_h2_det_ele;
  TH2* m_h2_det_dd ;

 public:
  AnaHitTree(const std::string name="ana_hit_matrix");
  virtual ~AnaHitTree();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();

  void SetUpTree(TTree*& tree, const std::string label);
};

#endif // _ANA_HIT_TREE__H_
