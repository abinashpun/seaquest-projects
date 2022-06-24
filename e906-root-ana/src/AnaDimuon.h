#ifndef _ANA_DIMUON__H_
#define _ANA_DIMUON__H_
#include <map>
#include "AnaBase.h"
#include "TreeData.h"
class TTree;
class TH1;

/// Analysis module to extract the dimuon data.
/**
 * No meaningful function is implemented yet.
 */
class AnaDimuon : public AnaBase {
  /// Counter
  int m_n_evt_all;
  int m_n_evt_spill;
  int m_n_evt_fpga1;
  int m_n_evt_tgt;

  /// Output
  TTree*     m_tree;
  EventData  m_evt;
  TrackList  m_trk_reco;
  DimuonList m_dim_reco;

 public:
  AnaDimuon(const std::string name="ana_dimuon");
  virtual ~AnaDimuon();
  void Init();
  void End();

 protected:
  void AnalyzeEvent();
};

#endif // _ANA_DIMUON__H_
