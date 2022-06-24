#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_
#include <vector>
#include <TLorentzVector.h>

/// @private
struct EventData {
  int  run_id;
  int  spill_id;
  int  event_id;
  bool fpga1;
  bool fpga2;
  bool fpga3;
  bool nim3;
  int  rec_status;

  EventData();
  virtual ~EventData() {;}
  ClassDef(EventData, 1);
};

/// @private
struct TrackData {
  int            charge;
  TVector3       pos_vtx;
  TLorentzVector mom_vtx;

  TrackData();
  virtual ~TrackData() {;}
  ClassDef(TrackData, 1);
};
typedef std::vector<TrackData > TrackList;

/// @private
struct DimuonData {
  TVector3       pos;
  TLorentzVector mom;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;
  double         pT;
  double         x1;
  double         x2;
  double         xF;
  double         costh;
  double         phi;

  DimuonData();
  virtual ~DimuonData() {;}
  ClassDef(DimuonData, 1);
};
typedef std::vector<DimuonData> DimuonList;

/// @private
struct OccData {
  static const int N_PL = 54; // cf. GlobalConsts.h
  int run_id;
  int spill_id;
  int event_id;
  short target_pos;
  bool fpga1;
  bool fpga2;
  bool fpga3;
  bool nim3;
  int turn_id;
  int rf_id;
  int rfp00;
  short occ_all[N_PL+1];
  short occ_intime[N_PL+1];

  OccData() {;}
  virtual ~OccData() {;}
  ClassDef(OccData, 1);
  void Clear();
};


#endif // _TREE_DATA__H_
