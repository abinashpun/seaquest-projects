#ifndef _ANA_CHAM_HIT__H_
#define _ANA_CHAM_HIT__H_
#include "TGraph.h"
#include "AnaBase.h"
class TH1;
class TH2;
class TGraph;

/// Analysis module to extract the chamber hit data.
class AnaChamHit : public AnaBase {
  static const short DET_ID_LO =  1;
  static const short DET_ID_HI = 30;

  ///
  /// For step 1
  ///
  TH1* m_h1_count; // event counts

  TH2* m_h2_time    [99]; // [detector ID]
  TH2* m_h2_time_int[99]; // [detector ID]

  ///
  /// For step 2
  ///
  ofstream m_ofs_run_ng;
  ofstream m_ofs_det_ng;
  TFile* m_file_out2;
  TGraph m_gr_h1t   [5];
  TGraph m_gr_h2t   [5];
  TGraph m_gr_h3t   [5];
  TGraph m_gr_h4t   [5];
  TGraph m_gr_h1b   [5];
  TGraph m_gr_h2b   [5];
  TGraph m_gr_h3b   [5];
  TGraph m_gr_h4b   [5];

 public:
  AnaChamHit(const std::string name="ana_cham_hit");
  virtual ~AnaChamHit();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();

  void DrawOnePlane(const char* name, TH1* h1_all, TH1* h1_int);
};

#endif // _ANA_CHAM_HIT__H_
