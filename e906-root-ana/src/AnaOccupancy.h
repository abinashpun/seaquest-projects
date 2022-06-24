#ifndef _ANA_OCCUPANCY__H_
#define _ANA_OCCUPANCY__H_
#include <map>
#include "AnaBase.h"
class TH1;
class TGraphErrors;

/// Analysis module to extract the plane occupancy.
class AnaOccupancy : public AnaBase {
  static const int N_PL = 54; // cf. GlobalConsts.h
  static const int N_TGT = 3;
  static const int N_TRIG = 2;
  static const char* NAME_TRIG[N_TRIG];

  ///
  /// For step 1
  ///
  TH1* h1_count; // event counts
  TH1* h1_occ_all   [N_TGT+1][N_TRIG][N_PL+1]; // [target][trigger][plane]
  TH1* h1_occ_intime[N_TGT+1][N_TRIG][N_PL+1]; // [target][trigger][plane]

  ///
  /// For step 2
  ///
  ofstream m_ofs_run_ng;
  TFile* m_file_out2;
  TH1* m_h1_occ_all_sum        [N_TGT+1][N_TRIG][N_PL+1];
  TH1* m_h1_occ_intime_sum     [N_TGT+1][N_TRIG][N_PL+1];
  TGraphErrors* m_gr_occ_all   [N_TGT+1][N_TRIG][N_PL+1];
  TGraphErrors* m_gr_occ_intime[N_TGT+1][N_TRIG][N_PL+1];
  TGraphErrors* m_gr_occ_rms_all   [N_TGT+1][N_TRIG][N_PL+1];
  TGraphErrors* m_gr_occ_rms_intime[N_TGT+1][N_TRIG][N_PL+1];

 public:
  AnaOccupancy(const std::string name="ana_occupancy");
  virtual ~AnaOccupancy();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();
};

#endif // _ANA_OCCUPANCY__H_
