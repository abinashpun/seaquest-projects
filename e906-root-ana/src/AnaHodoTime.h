#ifndef _ANA_HODO_TIME__H_
#define _ANA_HODO_TIME__H_
#include "TGraph.h"
#include "AnaBase.h"
class TH1;
class TGraph;

/// Analysis module to extract the hodo hit-time data.
class AnaHodoTime : public AnaBase {

  ///
  /// For step 1
  ///
  TH1* m_h1_count; // event counts

  TH1* m_h1_time_h1t;
  TH1* m_h1_time_h2t;
  TH1* m_h1_time_h3t;
  TH1* m_h1_time_h4t;
  TH1* m_h1_time_h1t_int;
  TH1* m_h1_time_h2t_int;
  TH1* m_h1_time_h3t_int;
  TH1* m_h1_time_h4t_int;

  TH1* m_h1_time_h1b;
  TH1* m_h1_time_h2b;
  TH1* m_h1_time_h3b;
  TH1* m_h1_time_h4b;
  TH1* m_h1_time_h1b_int;
  TH1* m_h1_time_h2b_int;
  TH1* m_h1_time_h3b_int;
  TH1* m_h1_time_h4b_int;

  TH1* m_h1_time_h1t_tr;
  TH1* m_h1_time_h2t_tr;
  TH1* m_h1_time_h3t_tr;
  TH1* m_h1_time_h4t_tr;
  TH1* m_h1_time_h1t_tr_int;
  TH1* m_h1_time_h2t_tr_int;
  TH1* m_h1_time_h3t_tr_int;
  TH1* m_h1_time_h4t_tr_int;

  TH1* m_h1_time_h1b_tr;
  TH1* m_h1_time_h2b_tr;
  TH1* m_h1_time_h3b_tr;
  TH1* m_h1_time_h4b_tr;
  TH1* m_h1_time_h1b_tr_int;
  TH1* m_h1_time_h2b_tr_int;
  TH1* m_h1_time_h3b_tr_int;
  TH1* m_h1_time_h4b_tr_int;

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
  TGraph m_gr_h1t_tr[5];
  TGraph m_gr_h2t_tr[5];
  TGraph m_gr_h3t_tr[5];
  TGraph m_gr_h4t_tr[5];
  TGraph m_gr_h1b   [5];
  TGraph m_gr_h2b   [5];
  TGraph m_gr_h3b   [5];
  TGraph m_gr_h4b   [5];
  TGraph m_gr_h1b_tr[5];
  TGraph m_gr_h2b_tr[5];
  TGraph m_gr_h3b_tr[5];
  TGraph m_gr_h4b_tr[5];

 public:
  AnaHodoTime(const std::string name="ana_hodo_time");
  virtual ~AnaHodoTime();
  void Init();
  void End();

  void InitStep2();
  void AnalyzeStep2(const int run_id, TFile* file_in);
  void EndStep2();

 protected:
  void AnalyzeEvent();

  void DrawOnePlane(const char* name, TH1* h1_all, TH1* h1_int);
  void AnaOneHodoHist(const int run_id, TFile* file, const char* name, TGraph (&gr)[5], std::ostream& os_det_ng);
  void DrawOneHodoGraph(const char* name, TGraph (&gr)[5]);
};

#endif // _ANA_HODO_TIME__H_
