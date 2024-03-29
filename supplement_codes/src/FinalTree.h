#ifndef _FINAL_TREE__H_
#define _FINAL_TREE__H_

class FinalTree
{

 public:
  FinalTree (){};
  virtual ~ FinalTree (){};
  virtual void AnalyzeGMC (char* file_in);
  virtual void AnalyzeData_Mix (const int run_id, TFile * file_in);
  virtual void ResetVars();
  void SetOutputDir(const std::string dir_out) { m_dir_out = dir_out; }

 protected:

/// Run and event level info
  std::string m_dir_out;

  int runN, runID, spillID, eventID, fpga1, occuD1,  occuD2,  occuD3m,occuD3p, targetPos, TargetPos;
  float rfp00c, pot_p00, liveP;

  float true_mass, true_pT, true_xF,true_xT, weight, true_costh,true_phi;

  float px1, py1, pz1, px2, py2, pz2;
  float trackSeparation;

  float px1_st1, py1_st1, pz1_st1, px2_st1, py2_st1, pz2_st1;
  float px1_st3, py1_st3, pz1_st3, px2_st3, py2_st3, pz2_st3;
  int D1, D2, D3;

  int nHits1St1, nHits1St2, nHits1St3;
  int nHits2St1, nHits2St2, nHits2St3;

  float  x1_t, y1_t,  x1_d, y1_d, x1_v, y1_v, z1_v; 
  double x1_st1, x1_st3, x2_st1, x2_st3;
  double y1_st1, y1_st3, y2_st1, y2_st3;
  float  x2_t, y2_t,  x2_d, y2_d,x2_v, y2_v,z2_v;
  float chisq1, chisq1_upstream, chisq1_target,chisq1_dump;
  float chisq2, chisq2_upstream, chisq2_target,chisq2_dump;
  int nHits1, nHits2;


  float mass, costh, phi, xF, xT, xB, pT,  vtx_x, vtx_y, vtx_z;
  float dpx, dpy, dpz, dim_mup_px,dim_mup_py,dim_mup_pz,dim_mum_px,dim_mum_py,dim_mum_pz;
  float chisq_target, chisq_dump, chisq_dimuon, chisq_upstream;

  float dx, dy, dz;



//Tracks corresponding to dimuons
 //positive-track
 float ptrk_chisq_upstream, ptrk_chisq_target, ptrk_chisq_dump, ptrk_chisq_vtx;//?chisq at various hypothesis
 float ptrk_x_d, ptrk_y_d; /// position at dump a
 float ptrk_x_t, ptrk_y_t; /// position at target
 float ptrk_vtx_z;/// position and momentum at vertex
 double ptrk_x_st1, ptrk_y_st1; //position and momentum at st1
 double ptrk_px_st1,  ptrk_py_st1,  ptrk_pz_st1;
 double ptrk_x_st3,   ptrk_y_st3; //position and momentum at st3
 double ptrk_px_st3, ptrk_py_st3, ptrk_pz_st3;
 int  ptrk_nHits;//number of hits at various stations
 
  int ptrk_TargetPos, ptrk_D1,  ptrk_D2,  ptrk_occuD3m, ptrk_occuD3p;
  float ptrk_rfp00c, ptrk_pot_p00, ptrk_liveP;



  int plus_D1, plus_D2, plus_occuD3p, plus_occuD3m,plus_TargetPos;
  float plus_rfp00c, plus_pot_p00, plus_liveP;


 //negative-track
 //double ntrk_px, ntrk_py, ntrk_pz;//?
 float ntrk_chisq_upstream, ntrk_chisq_target, ntrk_chisq_dump, ntrk_chisq_vtx;//chisq at various hypothesis
 float ntrk_x_d, ntrk_y_d; /// position at dump
 float ntrk_x_t, ntrk_y_t; /// position at target
 float ntrk_vtx_z;/// position and momentum at vertex


 double ntrk_x_st1, ntrk_y_st1; //position and momentum at st1
 double ntrk_px_st1,  ntrk_py_st1,  ntrk_pz_st1;
 double ntrk_x_st3,   ntrk_y_st3; //position and momentum at st3
 double ntrk_px_st3, ntrk_py_st3, ntrk_pz_st3;
 int  ntrk_nHits; //number of hits at various stations
 

  int ntrk_TargetPos, ntrk_D1,  ntrk_D2,  ntrk_occuD3m, ntrk_occuD3p;
  float ntrk_rfp00c, ntrk_pot_p00, ntrk_liveP;

  int minus_D1, minus_D2, minus_occuD3p, minus_occuD3m,minus_TargetPos;
  float minus_rfp00c, minus_pot_p00, minus_liveP;
};
#endif
