#ifndef _FINAL_TREE__H_
#define _FINAL_TREE__H_

class FinalTree
{

 public:
  FinalTree (){};
  virtual ~ FinalTree (){};
  virtual void AnalyzeGMC (char* file_in);
  virtual void AnalyzeData_Mix (const int run_id, TFile * file_in);


 protected:

/// Run and event level info
  int runN, spillID, eventID, fpga1, occuD1,  occuD2,  occuD3m,occuD3p, TargetPos;
  float rfp00c, pot_p00, liveP;

  float true_dim_mass, true_dim_pT, true_dim_xF,true_dim_xT, weight;


  float dim_mass, dim_costh, dim_phi, dim_xF, dim_xT, dim_xB, dim_pT,
    dim_vtx_x, dim_vtx_y, dim_vtx_z;
  float dim_px, dim_py, dim_pz;
  float dim_chisq_target, dim_chisq_dump, dim_chisq_vtx, dim_chisq_upstream;



//Tracks corresponding to dimuons
 //positive-track
 //double ptrk_px, ptrk_py, ptrk_pz;//?
 float ptrk_chisq_upstream, ptrk_chisq_target, ptrk_chisq_dump, ptrk_chisq_vtx;//?chisq at various hypothesis
 float ptrk_x_d, ptrk_y_d; /// position at dump a
 float ptrk_x_t, ptrk_y_t; /// position at target
 float ptrk_vtx_z;/// position and momentum at vertex
 //double ptrk_px_v,  ptrk_py_v,  ptrk_pz_v;
 double ptrk_x_st1, ptrk_y_st1; //position and momentum at st1
 double ptrk_px_st1,  ptrk_py_st1,  ptrk_pz_st1;
 double ptrk_x_st3,   ptrk_y_st3; //position and momentum at st3
 double ptrk_px_st3, ptrk_py_st3, ptrk_pz_st3;
 int  ptrk_nHits;// ptrk_nHits_St1, ptrk_nHits_St2,  ptrk_nHits_St3, ptrk_nHits_St4H, ptrk_nHits_St4V; //number of hits at various stations
 //double ptrk_z0x; //?
 //int ptrk_charge,  ptrk_Road;



 //negative-track
 //double ntrk_px, ntrk_py, ntrk_pz;//?
 float ntrk_chisq_upstream, ntrk_chisq_target, ntrk_chisq_dump, ntrk_chisq_vtx;//chisq at various hypothesis
 float ntrk_x_d, ntrk_y_d; /// position at dump
 float ntrk_x_t, ntrk_y_t; /// position at target
 float ntrk_vtx_z;/// position and momentum at vertex
 //double ntrk_px_v,  ntrk_py_v,  ntrk_pz_v;
 double ntrk_x_st1, ntrk_y_st1; //position and momentum at st1
 double ntrk_px_st1,  ntrk_py_st1,  ntrk_pz_st1;
 double ntrk_x_st3,   ntrk_y_st3; //position and momentum at st3
 double ntrk_px_st3, ntrk_py_st3, ntrk_pz_st3;
 int  ntrk_nHits;//, ntrk_nHits_St1, ntrk_nHits_St2,  ntrk_nHits_St3, ntrk_nHits_St4H, ntrk_nHits_St4V; //number of hits at various stations
 //double ntrk_z0x; //?
 //int ntrk_charge,  ntrk_Road;


};
#endif
