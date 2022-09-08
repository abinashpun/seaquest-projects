#ifndef _ANA_NMSU__H_
#define _ANA_NMSU__H_
#include <map>
#include "AnaBase.h"
#include "TreeData.h"

class TTree;
class TH1;
class SRecTrack;
/// Analysis module to extract the NMSU data.
/**
 * No meaningful function is implemented yet.
 */
/// @private
/*
struct NMSUData {
  int   run_id;
  int spill_id;
  int event_id;
  int rfp00; ///< RF+00
  //float rfp00c; ///< RF+00 corrected for the pedestal
  //float pot_p00;

  int DimMult;
  int D1;
  int TrkMult;
  int PosTrkMult;
  int NegTrkMult;
  float dim_mass;
  float dim_vtxz;

  NMSUData() {;}
  virtual ~NMSUData() {;}
  ClassDef(NMSUData, 1);

};
*/

class AnaNMSU : public AnaBase {
  /// Counter
  int m_n_evt_all;
  int m_n_evt_spill;
  int m_n_evt_fpga1;
  int m_n_evt_tgt;

  /// Output
  TTree*     m_tree;
  //NMSUData m_data;
  //EventData  m_evt;
  //TrackList  m_trk_reco;
  //DimuonList m_dim_reco;

 public:
  AnaNMSU(const std::string name="ana_nmsu");
  virtual ~AnaNMSU();
  void Init();
  void End();
  int ResetValues();
 private:

  int   run_ID;
  int spill_ID;
  int event_ID;
  //int rfp00; ///< RF+00
  float rfp00c; ///< RF+00 corrected for the pedestal
  float pot_p00; ///proton number corrsponding to RF+00 (pedestal corrected)
  float liveP; //Live PoT

  int DimMult;
  int occuD1;
  int occuD2;
  int occuD3p;
  int occuD3m;
  int TrkMult;
  int PosTrkMult;
  int NegTrkMult;
  int TargetPos;
  std::vector<SRecTrack> pos_tracks, neg_tracks;

  int  fpga1;
  int  fpga2;
  int  fpga3;
  int  fpga4;
  int  fpga5;
 
  float dim_vtx_z;

  std::vector<double> dim_M,pos_px,pos_py,pos_pz,pos_E,neg_px,neg_py,neg_pz,neg_E;

 protected:
  void AnalyzeEvent();
};

#endif // _ANA_NMSU__H_
