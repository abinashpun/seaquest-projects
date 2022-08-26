#include <fstream>
#include <ctime>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <JobOptsSvc.h>
#include <GeomSvc.h>
#include <SRawEvent.h>
#include <SRecEvent.h>
#include <E906DataCatalog.h>
#include "FinalTree.h"
using namespace std;

void FinalTree::AnalyzeData_Mix (const int run_id, TFile * file_in)
{
  TFile* fout = new TFile (Form("result_LD2_new/result_run%i.root",run_id),"recreate");
  TTree* result= new TTree("result", "The tree for final results");

  /// Run and event level info
  result->Branch("runID", &runID, "runID/I");
  result->Branch("spillID", &spillID, "spillID/I");
  result->Branch("D1", &D1, "D1/I");///DC occupancy at st. 1
  result->Branch("D2", &D2, "D2/I"); ///DC occupancy at st. 2
  result->Branch("D3", &D3, "D3/I"); ///DC occupancy at st. 3
  result->Branch("occuD3m", &occuD3m, "occuD3m/I"); ///DC occupancy at st. 3m
  result->Branch("occuD3p", &occuD3p, "occuD3p/I"); ///DC occupancy at st. 3p
  result->Branch("rfp00c", &rfp00c, "rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result->Branch("pot_p00", &pot_p00, "pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result->Branch("liveP", &liveP, "liveP/f");
  result->Branch("targetPos", &targetPos, "targetPos/I"); /// Position of target


  ///Dimuon level info
  result->Branch("mass", &mass, "mass/f");
  result->Branch("costh", &costh, "costh/f");
  result->Branch("phi", &phi, "phi/f");
  result->Branch("pT", &pT, "pT/f");
  result->Branch("xF", &xF, "xF/f");
  result->Branch("xT", &xT, "xT/f");
  result->Branch("xB", &xB, "xB/f");

  result->Branch("dpx", &dpx, "dpx/f");
  result->Branch("dpy", &dpy, "dpy/f");
  result->Branch("dpz", &dpz, "dpz/f");
  result->Branch("dx", &dx, "dx/f");
  result->Branch("dy", &dy, "dy/f");
  result->Branch("dz", &dz, "dz/f");
  result->Branch("chisq_upstream", &chisq_upstream, "chisq_upstream/f");
  result->Branch("chisq_target", &chisq_target, "chisq_target/f");
  result->Branch("chisq_dump", &chisq_dump, "chisq_dump/f");
  result->Branch("chisq_dimuon", &chisq_dimuon, "chisq_dimuon/f");
  ///Trk level info

  result->Branch("px1_st1", &px1_st1, "px1_st1/f");
  result->Branch("py1_st1", &py1_st1, "py1_st1/f");
  result->Branch("pz1_st1", &pz1_st1, "pz1_st1/f");
  result->Branch("px2_st1", &px2_st1, "px2_st1/f");
  result->Branch("py2_st1", &py2_st1, "py2_st1/f");
  result->Branch("pz2_st1", &pz2_st1, "pz2_st1/f");
  result->Branch("px1_st3", &px1_st3, "px1_st3/f");
  result->Branch("py1_st3", &py1_st3, "py1_st3/f");
  result->Branch("pz1_st3", &pz1_st3, "pz1_st3/f");
  result->Branch("px2_st3", &px2_st3, "px2_st3/f");
  result->Branch("py2_st3", &py2_st3, "py2_st3/f");
  result->Branch("pz2_st3", &pz2_st3, "pz2_st3/f");

  result->Branch("x1_t", &x1_t, "x1_t/f");
  result->Branch("y1_t", &y1_t, "y1_t/f");
  result->Branch("x1_d", &x1_d, "x1_d/f");
  result->Branch("y1_d", &y1_d, "y1_d/f");
  result->Branch("x1_v", &x1_v, "x1_v/f");
  result->Branch("y1_v", &y1_v, "y1_v/f");
  result->Branch("z1_v", &z1_v, "z1_v/f");


  result->Branch("x1_st1", &x1_st1, "x1_st1/d");
  result->Branch("y1_st1", &y1_st1, "y1_st1/d");
  result->Branch("x1_st3", &x1_st3, "x1_st3/d");
  result->Branch("y1_st3", &y1_st3, "y1_st3/d");


  result->Branch("x2_t", &x2_t, "x2_t/f");
  result->Branch("y2_t", &y2_t, "y2_t/f");
  result->Branch("x2_d", &x2_d, "x2_d/f");
  result->Branch("y2_d", &y2_d, "y2_d/f");
  result->Branch("x2_v", &x2_v, "x2_v/f");
  result->Branch("y2_v", &y2_v, "y2_v/f");
  result->Branch("z2_v", &z2_v, "z2_v/f");

  result->Branch("trackSeparation", &trackSeparation, "trackSeparation/f");

  result->Branch("x2_st1", &x2_st1, "x2_st1/d");
  result->Branch("y2_st1", &y2_st1, "y2_st1/d");
  result->Branch("x2_st3", &x2_st3, "x2_st3/d");
  result->Branch("y2_st3", &y2_st3, "y2_st3/d");

  result->Branch("nHits1", &nHits1, "nHits1/i");
  result->Branch("nHits2", &nHits2, "nHits2/i");
  result->Branch("nHits1St1", &nHits1St1, "nHits1St1/i");
  result->Branch("nHits1St2", &nHits1St2, "nHits1St2/i");
  result->Branch("nHits1St3", &nHits1St3, "nHits1St3/i");
  result->Branch("nHits2St1", &nHits2St1, "nHits2St1/i");
  result->Branch("nHits2St2", &nHits2St2, "nHits2St2/i");
  result->Branch("nHits2St3", &nHits2St3, "nHits2St3/i");

  result->Branch("chisq1", &chisq1, "chisq1/f");
  result->Branch("chisq1_upstream", &chisq1_upstream, "chisq1_upstream/f");
  result->Branch("chisq1_target", &chisq1_target, "chisq1_target/f");
  result->Branch("chisq1_dump", &chisq1_dump, "chisq1_dump/f");

  result->Branch("chisq2", &chisq2, "chisq2/f");
  result->Branch("chisq2_upstream", &chisq2_upstream, "chisq2_upstream/f");
  result->Branch("chisq2_target", &chisq2_target, "chisq2_target/f");
  result->Branch("chisq2_dump", &chisq2_dump, "chisq2_dump/f");


  //----------------------
  TTree *unmixed = (TTree*) file_in->Get("save_sorted");
  SRecEvent* rec_unmixed = new SRecEvent();
  int D1_, D2_, occuD3p_, occuD3m_,TargetPos_;
  float rfp00c_, pot_p00_, liveP_;

  unmixed->SetBranchAddress("recEvent", &rec_unmixed);
  unmixed->SetBranchAddress("occuD1", &D1_);
  unmixed->SetBranchAddress("occuD2", &D2_);
  unmixed->SetBranchAddress("occuD3p", &occuD3p_);
  unmixed->SetBranchAddress("occuD3m", &occuD3m_);
  unmixed->SetBranchAddress("TargetPos", &TargetPos_);
  unmixed->SetBranchAddress("rfp00c", &rfp00c_);
  unmixed->SetBranchAddress("pot_p00", &pot_p00_);
  unmixed->SetBranchAddress("liveP", &liveP_);

  /// Real Data Loop
  for(int i_evt=0;i_evt<unmixed->GetEntries();i_evt++) 
    {
      unmixed->GetEntry(i_evt);

      if (rec_unmixed->getNDimuons()<1) continue;

      ///Initializations
      spillID = 0.;
      runID = 0.;
      D1 = -9999;
      D2 =-9999;
      D3 =-9999;
      occuD3p = -9999;
      occuD3m = -9999;
      targetPos = -9999;
      rfp00c = -9999;
      pot_p00 = -9999;
      liveP = -9999;
 
      dx = -9999.0;
      dy = -9999.0;
      dz = -9999.0;
      dpx = -9999.0;
      dpy = -9999.0;
      dpz = -9999.0;
      chisq_dimuon = 0.;
      chisq_upstream = 0.;
      chisq_target = 0.;
      chisq_dump = 0.;


      pT = -999.;
      xT = -999.;
      xB = -999.;
      xF = -999.;
      costh = -999.;
      phi = -999.;
      mass = -999.;
      ntrk_pz_st3 = -999.;

      runID= rec_unmixed->getRunID();
      D1 = D1_;
      D2 = D2_;
      D3 = occuD3p_ + occuD3m_;
      occuD3p = occuD3p_;
      occuD3m = occuD3m_;
      targetPos = TargetPos_;
      rfp00c = rfp00c_;
      pot_p00 = pot_p00_;
      liveP = liveP_;

      

      int TrkMult = rec_unmixed->getNTracks();
      for(int j = 0; j<TrkMult; j++)
	{
	  SRecTrack recTrk = rec_unmixed->getTrack(j);
	  if (recTrk.getCharge()<0)
	    {   
	      x2_t = recTrk.getTargetPos().X();
	      y2_t = recTrk.getTargetPos().Y();

	      x2_d = recTrk.getDumpPos().X();
	      y2_d = recTrk.getDumpPos().Y();

	      x2_v = recTrk.getVertexPos().X();
	      y2_v = recTrk.getVertexPos().Y();
	      z2_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x2_st1,y2_st1);
	      recTrk.getPositionSt3(x2_st3,y2_st3);

	      px2_st1 = recTrk.getMomentumVecSt1().X();
	      py2_st1 = recTrk.getMomentumVecSt1().Y();
	      pz2_st1 = recTrk.getMomentumVecSt1().Z();

	      px2_st3 = recTrk.getMomentumVecSt3().X();
	      py2_st3 = recTrk.getMomentumVecSt3().Y();
	      pz2_st3 = recTrk.getMomentumVecSt3().Z();    


	      chisq2 = recTrk.getChisqVertex();
	      chisq2_upstream = recTrk.getChisqUpstream();
	      chisq2_target = recTrk.getChisqTarget();
	      chisq2_dump = recTrk.getChisqDump();

	      nHits2 = recTrk.getNHits();
	      nHits2St1 = recTrk.getNHitsInStation(1);
	      nHits2St2 = recTrk.getNHitsInStation(2);
	      nHits2St3 = recTrk.getNHitsInStation(3);

	    } 

	  if (recTrk.getCharge()>0)
	    {

	      x1_t = recTrk.getTargetPos().X();
	      y1_t = recTrk.getTargetPos().Y();
	      x1_d = recTrk.getDumpPos().X();
	      y1_d = recTrk.getDumpPos().Y();
	      x1_v = recTrk.getVertexPos().X();
	      y1_v = recTrk.getVertexPos().Y();
	      z1_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x1_st1,y1_st1);
	      recTrk.getPositionSt3(x1_st3,y1_st3);

	      chisq1 = recTrk.getChisqVertex();
	      chisq1_upstream = recTrk.getChisqUpstream();
	      chisq1_target = recTrk.getChisqTarget();
	      chisq1_dump = recTrk.getChisqDump();
	      
	      nHits1 = recTrk.getNHits();
	      px1_st1 = recTrk.getMomentumVecSt1().X();
	      py1_st1 = recTrk.getMomentumVecSt1().Y();
	      pz1_st1 = recTrk.getMomentumVecSt1().Z();
	      px1_st3 = recTrk.getMomentumVecSt3().X();
	      py1_st3 = recTrk.getMomentumVecSt3().Y();
	      pz1_st3 = recTrk.getMomentumVecSt3().Z();
	      nHits1St1 = recTrk.getNHitsInStation(1);
	      nHits1St2 = recTrk.getNHitsInStation(2);
	      nHits1St3 = recTrk.getNHitsInStation(3);
	    }
	}
      
      trackSeparation=fabs(z1_v -z2_v);
      for (int i =0; i<rec_unmixed->getNDimuons(); i++)
	{
    	  SRecDimuon recDimuon = rec_unmixed->getDimuon(i);

          dx = recDimuon.vtx.X();
	  dy = recDimuon.vtx.Y();
	  dz = recDimuon.vtx.Z();
	  dpx = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();
	  dpy = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();
	  dpz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();

          chisq_dimuon = recDimuon.chisq_kf;
   	  chisq_upstream = recDimuon.chisq_upstream;
          chisq_target = recDimuon.chisq_target;
          chisq_dump = recDimuon.chisq_dump;

       
          pT = recDimuon.pT;
          xT = recDimuon.x2;
	  xB = recDimuon.x1;
          xF = recDimuon.xF;
 	  costh = recDimuon.costh;
	  phi = recDimuon.phi;
          mass = recDimuon.mass;
	}
      result->Fill(); 
    }
  fout->cd();
  result->Write();







  ///============================================================================================
  ///------------------------------------mixed---------------------------------
  ///
  int ptrk_TargetPos, ptrk_D1,  ptrk_D2,  ptrk_occuD3m, ptrk_occuD3p;
  float ptrk_rfp00c, ptrk_pot_p00, ptrk_liveP; 
  int ntrk_TargetPos, ntrk_D1,  ntrk_D2,  ntrk_occuD3m, ntrk_occuD3p;
  float ntrk_rfp00c, ntrk_pot_p00, ntrk_liveP; 
  TTree* result_mix= new TTree("result_mix", "The tree for final results");
  /// Run and event level info
  result_mix->Branch("runID", &runID, "runID/I");
  result_mix->Branch("spillID", &spillID, "spillID/I");
  result_mix->Branch("D1", &D1, "D1/i");
  result_mix->Branch("D2", &D1, "D2/i");
  result_mix->Branch("D3", &D1, "D3/i");
  result_mix->Branch("ptrk_D1", &ptrk_D1, "ptrk_D1/I");///DC occupancy at st. 1
  result_mix->Branch("ptrk_D2", &ptrk_D2, "ptrk_D2/I"); ///DC occupancy at st. 2
  result_mix->Branch("ptrk_occuD3m", &ptrk_occuD3m, "ptrk_occuD3m/I"); ///DC occupancy at st. 3m
  result_mix->Branch("ptrk_occuD3p", &ptrk_occuD3p, "ptrk_occuD3p/I"); ///DC occupancy at st. 3p
  result_mix->Branch("ptrk_rfp00c", &ptrk_rfp00c, "ptrk_rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result_mix->Branch("ptrk_pot_p00", &ptrk_pot_p00, "ptrk_pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result_mix->Branch("ptrk_liveP", &ptrk_liveP, "ptrk_liveP/f");
  result_mix->Branch("ptrk_TargetPos", &ptrk_TargetPos, "ptrk_TargetPos/I"); /// Position of target

  result_mix->Branch("ntrk_D1", &ntrk_D1, "ntrk_D1/I");///DC occupancy at st. 1
  result_mix->Branch("ntrk_D2", &ntrk_D2, "ntrk_D2/I"); ///DC occupancy at st. 2
  result_mix->Branch("ntrk_occuD3m", &ntrk_occuD3m, "ntrk_occuD3m/I"); ///DC occupancy at st. 3m
  result_mix->Branch("ntrk_occuD3p", &ntrk_occuD3p, "ntrk_occuD3p/I"); ///DC occupancy at st. 3p
  result_mix->Branch("ntrk_rfp00c", &ntrk_rfp00c, "ntrk_rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result_mix->Branch("ntrk_pot_p00", &ntrk_pot_p00, "ntrk_pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result_mix->Branch("ntrk_liveP", &ntrk_liveP, "ntrk_liveP/f");
  result_mix->Branch("ntrk_TargetPos", &ntrk_TargetPos, "ntrk_TargetPos/I"); /// Position of target

  result_mix->Branch("pot_p00", &pot_p00, "pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)

  ///Dimuon level info
  result_mix->Branch("mass", &mass, "mass/f");
  result_mix->Branch("costh", &costh, "costh/f");
  result_mix->Branch("phi", &phi, "phi/f");
  result_mix->Branch("pT", &pT, "pT/f");
  result_mix->Branch("xF", &xF, "xF/f");
  result_mix->Branch("xT", &xT, "xT/f");
  result_mix->Branch("xB", &xB, "xB/f");
  result_mix->Branch("dpx", &dpx, "dpx/f");
  result_mix->Branch("dpy", &dpy, "dpy/f");
  result_mix->Branch("dpz", &dpz, "dpz/f");

  result_mix->Branch("dx", &dx, "dx/f");
  result_mix->Branch("dy", &dy, "dy/f");
  result_mix->Branch("dz", &dz, "dz/f");

  result_mix->Branch("chisq_upstream", &chisq_upstream, "chisq_upstream/f");
  result_mix->Branch("chisq_target", &chisq_target, "chisq_target/f");
  result_mix->Branch("chisq_dump", &chisq_dump, "chisq_dump/f");
  result_mix->Branch("chisq_dimuon", &chisq_dimuon, "chisq_dimuon/f");
  ///Trk level info
  result_mix->Branch("px1_st1", &px1_st1, "px1_st1/f");
  result_mix->Branch("py1_st1", &py1_st1, "py1_st1/f");
  result_mix->Branch("pz1_st1", &pz1_st1, "pz1_st1/f");
  result_mix->Branch("px2_st1", &px2_st1, "px2_st1/f");
  result_mix->Branch("py2_st1", &py2_st1, "py2_st1/f");
  result_mix->Branch("pz2_st1", &pz2_st1, "pz2_st1/f");
  result_mix->Branch("px1_st3", &px1_st3, "px1_st3/f");
  result_mix->Branch("py1_st3", &py1_st3, "py1_st3/f");
  result_mix->Branch("pz1_st3", &pz1_st3, "pz1_st3/f");
  result_mix->Branch("px2_st3", &px2_st3, "px2_st3/f");
  result_mix->Branch("py2_st3", &py2_st3, "py2_st3/f");
  result_mix->Branch("pz2_st3", &pz2_st3, "pz2_st3/f");

  result_mix->Branch("x1_t", &x1_t, "x1_t/f");
  result_mix->Branch("y1_t", &y1_t, "y1_t/f");
  result_mix->Branch("x1_d", &x1_d, "x1_d/f");
  result_mix->Branch("y1_d", &y1_d, "y1_d/f");
  result_mix->Branch("x1_v", &x1_v, "x1_v/f");
  result_mix->Branch("y1_v", &y1_v, "y1_v/f");
  result_mix->Branch("z1_v", &z1_v, "z1_v/f");

  result_mix->Branch("x1_st1", &x1_st1, "x1_st1/d");
  result_mix->Branch("y1_st1", &y1_st1, "y1_st1/d");
  result_mix->Branch("x1_st3", &x1_st3, "x1_st3/d");
  result_mix->Branch("y1_st3", &y1_st3, "y1_st3/d");

  result_mix->Branch("x2_t", &x2_t, "x2_t/f");
  result_mix->Branch("y2_t", &y2_t, "y2_t/f");
  result_mix->Branch("x2_d", &x2_d, "x2_d/f");
  result_mix->Branch("y2_d", &y2_d, "y2_d/f");
  result_mix->Branch("x2_v", &x2_v, "x2_v/f");
  result_mix->Branch("y2_v", &y2_v, "y2_v/f");
  result_mix->Branch("z2_v", &z2_v, "z2_v/f");

  result_mix->Branch("x2_st1", &x2_st1, "x2_st1/d");
  result_mix->Branch("y2_st1", &y2_st1, "y2_st1/d");
  result_mix->Branch("x2_st3", &x2_st3, "x2_st3/d");
  result_mix->Branch("y2_st3", &y2_st3, "y2_st3/d");

  result_mix->Branch("trackSeparation", &trackSeparation, "trackSeparation/f");

  result_mix->Branch("nHits1", &nHits1, "nHits1/i");
  result_mix->Branch("nHits2", &nHits2, "nHits2/i");

  result_mix->Branch("nHits1St1", &nHits1St1, "nHits1St1/i");
  result_mix->Branch("nHits1St2", &nHits1St2, "nHits1St2/i");
  result_mix->Branch("nHits1St3", &nHits1St3, "nHits1St3/i");

  result_mix->Branch("nHits2St1", &nHits2St1, "nHits2St1/i");
  result_mix->Branch("nHits2St2", &nHits2St2, "nHits2St2/i");
  result_mix->Branch("nHits2St3", &nHits2St3, "nHits2St3/i");

  result_mix->Branch("chisq1", &chisq1, "chisq1/f");
  result_mix->Branch("chisq1_upstream", &chisq1_upstream, "chisq1_upstream/f");
  result_mix->Branch("chisq1_target", &chisq1_target, "chisq1_target/f");
  result_mix->Branch("chisq1_dump", &chisq1_dump, "chisq1_dump/f");

  result_mix->Branch("chisq2", &chisq2, "chisq2/f");
  result_mix->Branch("chisq2_upstream", &chisq2_upstream, "chisq2_upstream/f");
  result_mix->Branch("chisq2_target", &chisq2_target, "chisq2_target/f");
  result_mix->Branch("chisq2_dump", &chisq2_dump, "chisq2_dump/f");



  //
  ///Read mix tree

  TTree *mixed = (TTree*) file_in->Get("save_mix");

  SRecEvent* rec_mixed = new SRecEvent();

  mixed->SetBranchAddress("recEvent", &rec_mixed);

  int plus_D1, plus_D2, plus_occuD3p, plus_occuD3m,plus_TargetPos;
  float plus_rfp00c, plus_pot_p00, plus_liveP;

  int minus_D1, minus_D2, minus_occuD3p, minus_occuD3m,minus_TargetPos;
  float minus_rfp00c, minus_pot_p00, minus_liveP;
 
  mixed->SetBranchAddress("plus_occuD1", &plus_D1);
  mixed->SetBranchAddress("plus_occuD2", &plus_D2);
  mixed->SetBranchAddress("plus_occuD3p", &plus_occuD3p);
  mixed->SetBranchAddress("plus_occuD3m", &plus_occuD3m);
  mixed->SetBranchAddress("plus_TargetPos", &plus_TargetPos);
  mixed->SetBranchAddress("plus_rfp00c", &plus_rfp00c);
  mixed->SetBranchAddress("plus_pot_p00", &plus_pot_p00);
  mixed->SetBranchAddress("plus_liveP", &plus_liveP);

  mixed->SetBranchAddress("minus_occuD1", &minus_D1);
  mixed->SetBranchAddress("minus_occuD2", &minus_D2);
  mixed->SetBranchAddress("minus_occuD3p", &minus_occuD3p);
  mixed->SetBranchAddress("minus_occuD3m", &minus_occuD3m);
  mixed->SetBranchAddress("minus_TargetPos", &minus_TargetPos);
  mixed->SetBranchAddress("minus_rfp00c", &minus_rfp00c);
  mixed->SetBranchAddress("minus_pot_p00", &minus_pot_p00);
  mixed->SetBranchAddress("minus_liveP", &minus_liveP);

  ///Mixed loop
  for(int i_evt=0;i_evt<mixed->GetEntries();i_evt++) 
    {
	
      mixed->GetEntry(i_evt);
      if (rec_mixed->getNDimuons()<1) continue;

      ptrk_D1 = -9999;
      ptrk_D2 =-9999;
      ptrk_occuD3p = -9999;
      ptrk_occuD3m = -9999;
      ptrk_TargetPos = -9999;
      ptrk_rfp00c = -9999;
      ptrk_pot_p00 = -9999;
      ptrk_liveP = -9999;
      ptrk_D1 = -9999;
      ptrk_D2 =-9999;
      ptrk_occuD3p = -9999;
      ptrk_occuD3m = -9999;
      ptrk_TargetPos = -9999;
      ptrk_rfp00c = -9999;
      ptrk_pot_p00 = -9999;
      ptrk_liveP = -9999;

      vtx_x = -999.;
      vtx_y = -999.;
      vtx_z = -999.;
      chisq_dimuon = -999.0;
      chisq_upstream = -999.0;
      chisq_target = -999.0;
      chisq_dump = -999.0;


      pT = -999.;
      xT = -999.;
      xB = -999.;
      xF = -999.;
      costh = -999.;
      phi = -999.;
      mass = -999.;

      runID= rec_mixed->getRunID();

      ptrk_D1 = plus_D1;
      ptrk_D2 = plus_D2;
      ptrk_occuD3p = plus_occuD3p;
      ptrk_occuD3m = plus_occuD3m;
      ptrk_TargetPos = plus_TargetPos;
      ptrk_rfp00c = plus_rfp00c;
      ptrk_pot_p00 = plus_pot_p00;
      ptrk_liveP = plus_liveP;


      ntrk_D1 = minus_D1;
      ntrk_D2 = minus_D2;
      ntrk_occuD3p = minus_occuD3p;
      ntrk_occuD3m = minus_occuD3m;
      ntrk_TargetPos = minus_TargetPos;
      ntrk_rfp00c = minus_rfp00c;
      ntrk_pot_p00 = minus_pot_p00;
      ntrk_liveP = minus_liveP;


      D1 = int((plus_D1+minus_D1)/2);
      D2 = int((plus_D2+minus_D2)/2);
      D3 = int((plus_occuD3p+minus_occuD3p)/2) + int((plus_occuD3m+minus_occuD3m)/2);
      pot_p00 = (ptrk_pot_p00+ntrk_pot_p00)/2;

      for(int j = 0; j<rec_mixed->getNTracks(); j++) ///Negative Track
	{
	  SRecTrack recTrk = rec_mixed->getTrack(j);
	  if (recTrk.getCharge()<0)
	    {   
	      x2_t = recTrk.getTargetPos().X();
	      y2_t = recTrk.getTargetPos().Y();
	      x2_d = recTrk.getDumpPos().X();
	      y2_d = recTrk.getDumpPos().Y();
	      x2_v = recTrk.getVertexPos().X();
	      y2_v = recTrk.getVertexPos().Y();
	      z2_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x2_st1,y2_st1);
	      recTrk.getPositionSt3(x2_st3,y2_st3);

	      px2_st1 = recTrk.getMomentumVecSt1().X();
	      py2_st1 = recTrk.getMomentumVecSt1().Y();
	      pz2_st1 = recTrk.getMomentumVecSt1().Z();

	      px2_st3 = recTrk.getMomentumVecSt3().X();
	      py2_st3 = recTrk.getMomentumVecSt3().Y();
	      pz2_st3 = recTrk.getMomentumVecSt3().Z();    


	      chisq2 = recTrk.getChisqVertex();
	      chisq2_upstream = recTrk.getChisqUpstream();
	      chisq2_target = recTrk.getChisqTarget();
	      chisq2_dump = recTrk.getChisqDump();
	    
 
	      nHits2 = recTrk.getNHits();
	      nHits2St1 = recTrk.getNHitsInStation(1);
	      nHits2St2 = recTrk.getNHitsInStation(2);
	      nHits2St3 = recTrk.getNHitsInStation(3);

	    } 

	  if (recTrk.getCharge()>0)///Positive Track
	    {

	      x1_t = recTrk.getTargetPos().X();
	      y1_t = recTrk.getTargetPos().Y();
	      x1_d = recTrk.getDumpPos().X();
	      y1_d = recTrk.getDumpPos().Y();
	      x1_v = recTrk.getVertexPos().X();
	      y1_v = recTrk.getVertexPos().Y();
	      z1_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x1_st1,y1_st1);
	      recTrk.getPositionSt3(x1_st3,y1_st3);

	      chisq1 = recTrk.getChisqVertex();
	      chisq1_upstream = recTrk.getChisqUpstream();
	      chisq1_target = recTrk.getChisqTarget();
	      chisq1_dump = recTrk.getChisqDump();


	      px1_st1 = recTrk.getMomentumVecSt1().X();
	      py1_st1 = recTrk.getMomentumVecSt1().Y();
	      pz1_st1 = recTrk.getMomentumVecSt1().Z();
	      px1_st3 = recTrk.getMomentumVecSt3().X();
	      py1_st3 = recTrk.getMomentumVecSt3().Y();
	      pz1_st3 = recTrk.getMomentumVecSt3().Z();

	      nHits1 = recTrk.getNHits();
	      nHits1St1 = recTrk.getNHitsInStation(1);
	      nHits1St2 = recTrk.getNHitsInStation(2);
	      nHits1St3 = recTrk.getNHitsInStation(3);
	    }


	}

      trackSeparation=fabs(z1_v -z2_v);
 
      for (int i =0; i< rec_mixed->getNDimuons(); i++)
	{
    	  SRecDimuon recDimuon = rec_mixed->getDimuon(i);

          dx = recDimuon.vtx.X();
	  dy = recDimuon.vtx.Y();
	  dz = recDimuon.vtx.Z();

          dpx = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();
          dpy = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();
          dpz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();

          chisq_dimuon = recDimuon.chisq_kf;
   	  chisq_upstream = recDimuon.chisq_upstream;
          chisq_target = recDimuon.chisq_target;
          chisq_dump = recDimuon.chisq_dump;


          pT = recDimuon.pT;
          xT = recDimuon.x2;
	  xB = recDimuon.x1;
          xF = recDimuon.xF;
 	  costh = recDimuon.costh;
	  phi = recDimuon.phi;
          mass = recDimuon.mass;
	}

      result_mix->Fill();          

    }

  fout->cd();
  result_mix->Write();


}

void FinalTree::AnalyzeGMC(char* input_file)
{

  ///Dimuon level info
  TFile* fout = new TFile ("result_gmc/result_mcDY_dump.root","recreate");
  TTree* result_mc= new TTree("result_mc", "The tree for final results");


  ///Dimuon level info

  result_mc->Branch("runID", &runID, "runID/I");
  result_mc->Branch("spillID", &spillID, "spillID/I");
  result_mc->Branch("weight", &weight, "weight/f");
  result_mc->Branch("fpga1", &fpga1, "fpga1/I"); 

  result_mc->Branch("true_mass", &true_mass, "true_mass/f");
  result_mc->Branch("true_pT", &true_pT, "true_pT/f");
  result_mc->Branch("true_xF", &true_xF, "true_xF/f");
  result_mc->Branch("true_costh", &true_costh, "true_costh/f");
  result_mc->Branch("true_phi", &true_phi, "true_phi/f");

  result_mc->Branch("mass", &mass, "mass/f");
  result_mc->Branch("costh", &costh, "costh/f");
  result_mc->Branch("phi", &phi, "phi/f");
  result_mc->Branch("pT", &pT, "pT/f");
  result_mc->Branch("xF", &xF, "xF/f");
  result_mc->Branch("xT", &xT, "xT/f");
  result_mc->Branch("xB", &xB, "xB/f");

  result_mc->Branch("dpx", &dpx, "dpx/f");
  result_mc->Branch("dpy", &dpy, "dpy/f");
  result_mc->Branch("dpz", &dpz, "dpz/f");
  result_mc->Branch("dx", &dx, "dx/f");
  result_mc->Branch("dy", &dy, "dy/f");
  result_mc->Branch("dz", &dz, "dz/f");


  result_mc->Branch("chisq_upstream", &chisq_upstream, "chisq_upstream/f");
  result_mc->Branch("chisq_target", &chisq_target, "chisq_target/f");
  result_mc->Branch("chisq_dump", &chisq_dump, "chisq_dump/f");
  result_mc->Branch("chisq_dimuon", &chisq_dimuon, "chisq_dimuon/f");


  chisq2 =-9999.0; chisq2_upstream =-9999.0; chisq2_target=-9999.0;chisq2_dump =-9999.0;
  chisq1 =-9999.0; chisq1_upstream =-9999.0; chisq1_target=-9999.0;chisq1_dump =-9999.0;


  result_mc->Branch("chisq1", &chisq1, "chisq1/f");
  result_mc->Branch("chisq1_upstream", &chisq1_upstream, "chisq1_upstream/f");
  result_mc->Branch("chisq1_target", &chisq1_target, "chisq1_target/f");
  result_mc->Branch("chisq1_dump", &chisq1_dump, "chisq1_dump/f");

  result_mc->Branch("chisq2", &chisq2, "chisq2/f");
  result_mc->Branch("chisq2_upstream", &chisq2_upstream, "chisq2_upstream/f");
  result_mc->Branch("chisq2_target", &chisq2_target, "chisq2_target/f");
  result_mc->Branch("chisq2_dump", &chisq2_dump, "chisq2_dump/f");


  result_mc->Branch("px1", &px1, "px1/f");
  result_mc->Branch("py1", &py1, "py1/f");
  result_mc->Branch("pz1", &pz1, "pz1/f");
  result_mc->Branch("px2", &px2, "px2/f");
  result_mc->Branch("py2", &py2, "py2/f");
  result_mc->Branch("pz2", &pz2, "pz2/f");


  result_mc->Branch("px1_st1", &px1_st1, "px1_st1/f");
  result_mc->Branch("py1_st1", &py1_st1, "py1_st1/f");
  result_mc->Branch("pz1_st1", &pz1_st1, "pz1_st1/f");
  result_mc->Branch("px2_st1", &px2_st1, "px2_st1/f");
  result_mc->Branch("py2_st1", &py2_st1, "py2_st1/f");
  result_mc->Branch("pz2_st1", &pz2_st1, "pz2_st1/f");


  result_mc->Branch("px1_st3", &px1_st3, "px1_st3/f");
  result_mc->Branch("py1_st3", &py1_st3, "py1_st3/f");
  result_mc->Branch("pz1_st3", &pz1_st3, "pz1_st3/f");
  result_mc->Branch("px2_st3", &px2_st3, "px2_st3/f");
  result_mc->Branch("py2_st3", &py2_st3, "py2_st3/f");
  result_mc->Branch("pz2_st3", &pz2_st3, "pz2_st3/f");

  result_mc->Branch("x1_t", &x1_t, "x1_t/f");
  result_mc->Branch("y1_t", &y1_t, "y1_t/f");
  result_mc->Branch("x1_d", &x1_d, "x1_d/f");
  result_mc->Branch("y1_d", &y1_d, "y1_d/f");
  result_mc->Branch("x1_v", &x1_v, "x1_v/f");
  result_mc->Branch("y1_v", &y1_v, "y1_v/f");
  result_mc->Branch("z1_v", &z1_v, "z1_v/f");

  result_mc->Branch("x1_st1", &x1_st1, "x1_st1/d");
  result_mc->Branch("y1_st1", &y1_st1, "y1_st1/d");
  result_mc->Branch("x1_st3", &x1_st3, "x1_st3/d");
  result_mc->Branch("y1_st3", &y1_st3, "y1_st3/d");


  result_mc->Branch("x2_t", &x2_t, "x2_t/f");
  result_mc->Branch("y2_t", &y2_t, "y2_t/f");
  result_mc->Branch("x2_d", &x2_d, "x2_d/f");
  result_mc->Branch("y2_d", &y2_d, "y2_d/f");
  result_mc->Branch("x2_v", &x2_v, "x2_v/f");
  result_mc->Branch("y2_v", &y2_v, "y2_v/f");
  result_mc->Branch("z2_v", &z2_v, "z2_v/f");

  result_mc->Branch("x2_st1", &x2_st1, "x2_st1/d");
  result_mc->Branch("y2_st1", &y2_st1, "y2_st1/d");
  result_mc->Branch("x2_st3", &x2_st3, "x2_st3/d");
  result_mc->Branch("y2_st3", &y2_st3, "y2_st3/d");

  result_mc->Branch("trackSeparation", &trackSeparation, "trackSeparation/f");

  result_mc->Branch("nHits1", &nHits1, "nHits1/i");
  result_mc->Branch("nHits2", &nHits2, "nHits2/i");

  result_mc->Branch("D1", &D1, "D1/i");
  result_mc->Branch("D2", &D1, "D2/i");
  result_mc->Branch("D3", &D1, "D3/i");

  result_mc->Branch("nHits1St1", &nHits1St1, "nHits1St1/i");
  result_mc->Branch("nHits1St2", &nHits1St2, "nHits1St2/i");
  result_mc->Branch("nHits1St3", &nHits1St3, "nHits1St3/i");
  result_mc->Branch("nHits2St1", &nHits2St1, "nHits2St1/i");
  result_mc->Branch("nHits2St2", &nHits2St2, "nHits2St2/i");
  result_mc->Branch("nHits2St3", &nHits2St3, "nHits2St3/i");

  ///-----
  TFile* file_in = new TFile (input_file, "read");
  TTree *mc_save = (TTree*) file_in->Get("save");
  SRecEvent* mc_rec = new SRecEvent();
  SRawMCEvent *mc_raw = new SRawMCEvent();

  mc_save->SetBranchAddress("recEvent", &mc_rec);
  mc_save->SetBranchAddress("rawEvent", &mc_raw);


  for(int i_evt=0;i_evt<mc_save->GetEntries();i_evt++) 
    {
 
      mc_save->GetEntry(i_evt);

      ///Initializations
      spillID = 0.;
      runID = 0.;
      eventID =0;

      nHits1St1=-999; nHits1St2=-999;nHits1St3=-999;
      nHits2St1=-999; nHits2St2=-999;nHits2St3=-999;

      vtx_x = -999.;
      vtx_y = -999.;
      vtx_z = -999.;
      chisq_dimuon = -999.;
      chisq_upstream = -999.;
      chisq_target = -999.;
      chisq_dump = -999.;

      pT = -999.;
      xT = -999.;
      xB = -999.;
      xF = -999.;
      costh = -999.;
      phi = -999.;
      mass = -999.;

      px1=-9999.0; py1=-9999.0;pz1=-9999.0;
      px2=-9999.0; py2=-9999.0;pz2=-9999.0;

      x2_t =-9999.0; y2_t=-9999.0; x2_d =-9999.0; y2_d=-9999.0;
      x1_t =-9999.0; y1_t=-9999.0; x1_d =-9999.0; y1_d=-9999.0;

      px1_st1 =-9999.0; py1_st1=-9999.0; pz1_st1=-9999.0; px2_st1=-9999.0; py2_st1=-9999.0; pz2_st1=-9999.0;
      px1_st3 =-9999.0; py1_st3=-9999.0; pz1_st3=-9999.0; px2_st3=-9999.0; py2_st3=-9999.0; pz2_st3=-9999.0;

      chisq2 =-9999.0; chisq2_upstream =-9999.0; chisq2_target=-9999.0;chisq2_dump =-9999.0;
      chisq1 =-9999.0; chisq1_upstream =-9999.0; chisq1_target=-9999.0;chisq1_dump =-9999.0;
      nHits1=-999; nHits2=-999;

      float m_muon = 0.1056;
      spillID = mc_raw->getSpillID();
      runID= mc_raw->getRunID();
      eventID= mc_raw->getEventID();

      D1 = mc_raw->getNHitsInD1();
      D2 = mc_raw->getNHitsInD2();
      D3 = mc_raw->getNHitsInD3();


      /// Snippet from Kei to correct trigger bits
      int triggers[10] = {0};
      bool triggered[4] = {false};
      triggered[0] = mc_raw->getNRoadsPosTop() > 0 ? true : false;
      triggered[1] = mc_raw->getNRoadsPosBot() > 0 ? true : false;
      triggered[2] = mc_raw->getNRoadsNegTop() > 0 ? true : false;
      triggered[3] = mc_raw->getNRoadsNegBot() > 0 ? true : false;
      triggers[0] = ( triggered[0] && triggered[3] ) || ( triggered[1] && triggered[2] ) ? true : false;
      triggers[1] = ( triggered[0] && triggered[2] ) || ( triggered[1] && triggered[3] ) ? true : false;
      triggers[2] = ( triggered[0] && triggered[1] ) || ( triggered[2] && triggered[3] ) ? true : false;
      triggers[3] =   triggered[0] || triggered[1]   ||   triggered[2] || triggered[3]   ? true : false;
      mc_raw->setTriggerBits(triggers);

      //Getting the costh and phi events: 
      TLorentzVector mup_mc;
      TLorentzVector mum_mc;
      TLorentzVector dimu_mc;
      TLorentzVector p_sum;
      TLorentzVector p_pos;
      TLorentzVector p_neg;
      mup_mc.SetXYZM(mc_raw->p_vertex[0].Px(),mc_raw->p_vertex[0].Py(),mc_raw->p_vertex[0].Pz(),m_muon);
      mum_mc.SetXYZM(mc_raw->p_vertex[1].Px(),mc_raw->p_vertex[1].Py(),mc_raw->p_vertex[1].Pz(),m_muon);
      //====================================================================================
      // https://e1039-collaboration.github.io/e1039-doc/master/d7/d57/SRecEvent_8cxx_source.html#l00593
      p_pos = mup_mc;
      p_neg = mum_mc;
      dimu_mc = mup_mc + mum_mc;  // dimuon info in the lab frame     
      p_sum = mup_mc + mum_mc;
      Double_t mp = 0.938;
      Double_t ebeam = 120.;
      TLorentzVector p_beam(0., 0., sqrt(ebeam*ebeam - mp*mp), ebeam);
      TLorentzVector p_target(0., 0., 0., mp);
      TLorentzVector p_cms = p_beam + p_target;
      true_mass = p_sum.M();
      true_pT = p_sum.Perp();
      TVector3 bv_cms = p_cms.BoostVector();
      p_sum.Boost(-bv_cms);
      //xF = 2.*p_sum.Pz()/TMath::Sqrt(s)/(1. - mass*mass/s);
      true_costh = 2.*(p_neg.E()*p_pos.Pz() - p_pos.E()*p_neg.Pz())/true_mass/sqrt(true_mass*true_mass + true_pT*true_pT);
      true_phi = atan2(2.*sqrt(true_mass*true_mass + true_pT*true_pT)*(p_neg.X()*p_pos.Y() - p_pos.X()*p_neg.Y()), true_mass*(p_pos.X()*p_pos.X() - p_neg.X()*p_neg.X() + p_pos.Y()*p_pos.Y() - p_neg.Y()*p_neg.Y()));



      fpga1 = (mc_raw->isTriggeredBy(SRawMCEvent::MATRIX1));
      true_mass = mc_raw->mass;
      true_xF = mc_raw->xF;
      true_pT = mc_raw->pT;
      weight = mc_raw->weight;

      //Filling the track level information
      int TrkMult = mc_rec->getNTracks();
      for(int j = 0; j<TrkMult; j++)
	{ 
	  SRecTrack recTrk = mc_rec->getTrack(j);

	  if (recTrk.getCharge()<0)
	    { 
	      x2_t = recTrk.getTargetPos().X();
	      y2_t = recTrk.getTargetPos().Y();
	      x2_d = recTrk.getDumpPos().X();
	      y2_d = recTrk.getDumpPos().Y();
	      x2_v = recTrk.getVertexPos().X();
	      y2_v = recTrk.getVertexPos().Y();
	      z2_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x2_st1,y2_st1);
	      recTrk.getPositionSt3(x2_st3,y2_st3);

	      px2_st1 = recTrk.getMomentumVecSt1().X();
	      py2_st1 = recTrk.getMomentumVecSt1().Y();
	      pz2_st1 = recTrk.getMomentumVecSt1().Z();

	      px2_st3 = recTrk.getMomentumVecSt3().X();
	      py2_st3 = recTrk.getMomentumVecSt3().Y();
	      pz2_st3 = recTrk.getMomentumVecSt3().Z();	

	      chisq2 = recTrk.getChisqVertex();
	      chisq2_upstream = recTrk.getChisqUpstream();
	      chisq2_target = recTrk.getChisqTarget();
	      chisq2_dump = recTrk.getChisqDump();

	      nHits2 = recTrk.getNHits();
	      nHits2St1 = recTrk.getNHitsInStation(1);
	      nHits2St2 = recTrk.getNHitsInStation(2);
	      nHits2St3 = recTrk.getNHitsInStation(3);

	    }
	  if (recTrk.getCharge()>0)
	    {

	      x1_t = recTrk.getTargetPos().X();
	      y1_t = recTrk.getTargetPos().Y();
	      x1_d = recTrk.getDumpPos().X();
	      y1_d = recTrk.getDumpPos().Y();
	      x1_v = recTrk.getVertexPos().X();
	      y1_v = recTrk.getVertexPos().Y();
	      z1_v = recTrk.getVertexPos().Z();

	      recTrk.getPositionSt1(x1_st1,y1_st1);
	      recTrk.getPositionSt3(x1_st3,y1_st3);

	      chisq1 = recTrk.getChisqVertex();
	      chisq1_upstream = recTrk.getChisqUpstream();
	      chisq1_target = recTrk.getChisqTarget();
	      chisq1_dump = recTrk.getChisqDump();


	      nHits1 = recTrk.getNHits(); 
	      px1_st1 = recTrk.getMomentumVecSt1().X();
	      py1_st1 = recTrk.getMomentumVecSt1().Y();
	      pz1_st1 = recTrk.getMomentumVecSt1().Z();

	      px1_st3 = recTrk.getMomentumVecSt3().X();
	      py1_st3 = recTrk.getMomentumVecSt3().Y();
	      pz1_st3 = recTrk.getMomentumVecSt3().Z();



	      nHits1St1 = recTrk.getNHitsInStation(1);
	      nHits1St2 = recTrk.getNHitsInStation(2);
	      nHits1St3 = recTrk.getNHitsInStation(3);
	    }
	}



      trackSeparation=fabs(z1_v -z2_v);
      for (int i =0; i<mc_rec->getNDimuons(); i++)
	{
	  SRecDimuon recDimuon = mc_rec->getDimuon(i);
	  dx = recDimuon.vtx.X();
	  dy = recDimuon.vtx.Y();
	  dz = recDimuon.vtx.Z();
	  dpx = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();
	  dpy = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();
	  dpz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();
	  px1 = recDimuon.p_pos.Vect().X();
	  px2 = recDimuon.p_neg.Vect().X();
	  py1 = recDimuon.p_pos.Vect().Y();
	  py2 = recDimuon.p_neg.Vect().Y();
	  pz1 = recDimuon.p_pos.Vect().Z();
	  pz2 = recDimuon.p_neg.Vect().Z();
	  chisq_dimuon = recDimuon.chisq_kf;
	  chisq_upstream = recDimuon.chisq_upstream;
	  chisq_target = recDimuon.chisq_target;
	  chisq_dump = recDimuon.chisq_dump;
	  pT = recDimuon.pT;
	  xT = recDimuon.x2;
	  xB = recDimuon.x1;
	  xF = recDimuon.xF;
	  costh = recDimuon.costh;
	  phi = recDimuon.phi;
	  mass = recDimuon.mass;

	}
      result_mc->Fill(); 
    }
  fout->cd();
  result_mc->Write();
}
