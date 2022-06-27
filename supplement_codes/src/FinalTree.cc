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
  TFile* fout = new TFile (Form("result_all/result_run%i.root",run_id),"recreate");
  TTree* result= new TTree("result", "The tree for final results");
  /// Run and event level info
  result->Branch("runN", &runN, "runN/I");
  result->Branch("spillID", &spillID, "spillID/I");
  result->Branch("occuD1", &occuD1, "occuD1/I");///DC occupancy at st. 1
  result->Branch("occuD2", &occuD2, "occuD2/I"); ///DC occupancy at st. 2
  result->Branch("occuD3m", &occuD3m, "occuD3m/I"); ///DC occupancy at st. 3m
  result->Branch("occuD3p", &occuD3p, "occuD3p/I"); ///DC occupancy at st. 3p
  result->Branch("rfp00c", &rfp00c, "rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result->Branch("pot_p00", &pot_p00, "pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result->Branch("liveP", &liveP, "liveP/f");
  result->Branch("TargetPos", &TargetPos, "TargetPos/I"); /// Position of target


  ///Dimuon level info
  result->Branch("dim_mass", &dim_mass, "dim_mass/f");
  result->Branch("dim_costh", &dim_costh, "dim_costh/f");
  result->Branch("dim_phi", &dim_phi, "dim_phi/f");
  result->Branch("dim_pT", &dim_pT, "dim_pT/f");
  result->Branch("dim_xF", &dim_xF, "dim_xF/f");
  result->Branch("dim_xT", &dim_xT, "dim_xT/f");
  result->Branch("dim_xB", &dim_xB, "dim_xB/f");
  result->Branch("dim_px", &dim_px, "dim_px/f");
  result->Branch("dim_py", &dim_py, "dim_py/f");
  result->Branch("dim_pz", &dim_pz, "dim_pz/f");
  result->Branch("dim_vtx_x", &dim_vtx_x, "dim_vtx_x/f");
  result->Branch("dim_vtx_y", &dim_vtx_y, "dim_vtx_y/f");
  result->Branch("dim_vtx_z", &dim_vtx_z, "dim_vtx_z/f");
  result->Branch("dim_chisq_upstream", &dim_chisq_upstream, "dim_chisq_upstream/f");
  result->Branch("dim_chisq_target", &dim_chisq_target, "dim_chisq_target/f");
  result->Branch("dim_chisq_dump", &dim_chisq_dump, "dim_chisq_dump/f");
  result->Branch("dim_chisq_vtx", &dim_chisq_vtx, "dim_chisq_vtx/f");
 
  ///Trk level info
  result->Branch("ptrk_vtx_z", &ptrk_vtx_z, "ptrk_vtx_z/f");
  result->Branch("ptrk_x_t", &ptrk_x_t, "ptrk_x_t/f");
  result->Branch("ptrk_y_t", &ptrk_y_t, "ptrk_y_t/f");
  result->Branch("ptrk_x_d", &ptrk_x_d, "ptrk_x_d/f");
  result->Branch("ptrk_y_d", &ptrk_y_d, "ptrk_y_d/f");

  result->Branch("ptrk_x_st1", &ptrk_x_st1, "ptrk_x_st1/d");
  result->Branch("ptrk_y_st1", &ptrk_y_st1, "ptrk_y_st1/d");

  result->Branch("ptrk_px_st1", &ptrk_px_st1, "ptrk_px_st1/d");
  result->Branch("ptrk_py_st1", &ptrk_py_st1, "ptrk_py_st1/d");
  result->Branch("ptrk_pz_st1", &ptrk_pz_st1, "ptrk_pz_st1/d");

  result->Branch("ptrk_x_st3", &ptrk_x_st3, "ptrk_x_st3/d");
  result->Branch("ptrk_y_st3", &ptrk_y_st3, "ptrk_y_st3/d");

  result->Branch("ptrk_px_st3", &ptrk_px_st3, "ptrk_px_st3/d");
  result->Branch("ptrk_py_st3", &ptrk_py_st3, "ptrk_py_st3/d");
  result->Branch("ptrk_pz_st3", &ptrk_pz_st3, "ptrk_pz_st3/d");

  result->Branch("ptrk_chisq_upstream", &ptrk_chisq_upstream, "ptrk_chisq_upstream/f");
  result->Branch("ptrk_chisq_target", &ptrk_chisq_target, "ptrk_chisq_target/f");
  result->Branch("ptrk_chisq_dump", &ptrk_chisq_dump, "ptrk_chisq_dump/f");
  result->Branch("ptrk_chisq_vtx", &ptrk_chisq_vtx, "ptrk_chisq_vtx/d");

  result->Branch("ptrk_nHits", &ptrk_nHits, "ptrk_nHits/I");


  result->Branch("ntrk_vtx_z", &ntrk_vtx_z, "ntrk_vtx_z/f");
  result->Branch("ntrk_x_t", &ptrk_x_t, "ntrk_x_t/f");
  result->Branch("ntrk_y_t", &ntrk_y_t, "ntrk_y_t/f");
  result->Branch("ntrk_x_d", &ptrk_x_d, "ntrk_x_d/f");
  result->Branch("ntrk_y_d", &ntrk_y_d, "ntrk_y_d/f");

  result->Branch("ntrk_x_st1", &ntrk_x_st1, "ntrk_x_st1/d");
  result->Branch("ntrk_y_st1", &ntrk_y_st1, "ntrk_y_st1/d");

  result->Branch("ntrk_px_st1", &ntrk_px_st1, "ntrk_px_st1/d");
  result->Branch("ntrk_py_st1", &ntrk_py_st1, "ntrk_py_st1/d");
  result->Branch("ntrk_pz_st1", &ntrk_pz_st1, "ntrk_pz_st1/d");

  result->Branch("ntrk_x_st3", &ntrk_x_st3, "ntrk_x_st3/d");
  result->Branch("ntrk_y_st3", &ntrk_y_st3, "ntrk_y_st3/d");
 
  result->Branch("ntrk_px_st3", &ntrk_px_st3, "ntrk_px_st3/d");
  result->Branch("ntrk_py_st3", &ntrk_py_st3, "ntrk_py_st3/d");
  result->Branch("ntrk_pz_st3", &ntrk_pz_st3, "ntrk_pz_st3/d");

  result->Branch("ntrk_chisq_upstream", &ntrk_chisq_upstream, "ntrk_chisq_upstream/f");
  result->Branch("ntrk_chisq_target", &ntrk_chisq_target, "ntrk_chisq_target/f");
  result->Branch("ntrk_chisq_dump", &ntrk_chisq_dump, "ntrk_chisq_dump/f");
  result->Branch("ntrk_chisq_vtx", &ntrk_chisq_vtx, "ntrk_chisq_vtx/f");
  result->Branch("ntrk_nHits", &ntrk_nHits, "ntrk_nHits/I");

  ///-----

  TTree *unmixed = (TTree*) file_in->Get("save_sorted");
  SRecEvent* rec_unmixed = new SRecEvent();
  int occuD1_, occuD2_, occuD3p_, occuD3m_,TargetPos_;
  float rfp00c_, pot_p00_, liveP_;

  unmixed->SetBranchAddress("recEvent", &rec_unmixed);
  unmixed->SetBranchAddress("occuD1", &occuD1_);
  unmixed->SetBranchAddress("occuD2", &occuD2_);
  unmixed->SetBranchAddress("occuD3p", &occuD3p_);
  unmixed->SetBranchAddress("occuD3m", &occuD3m_);
  unmixed->SetBranchAddress("TargetPos", &TargetPos_);
  unmixed->SetBranchAddress("rfp00c", &rfp00c_);
  unmixed->SetBranchAddress("pot_p00", &pot_p00_);
  unmixed->SetBranchAddress("liveP", &liveP_);

  ///
  for(int i_evt=0;i_evt<unmixed->GetEntries();i_evt++) 
    {
      unmixed->GetEntry(i_evt);

      if (rec_unmixed->getNDimuons()<1) continue;

      ///Initializations
      spillID = 0.;
      runN = 0.;
      occuD1 = -9999;
      occuD2 =-9999;
      occuD3p = -9999;
      occuD3m = -9999;
      TargetPos = -9999;
      rfp00c = -9999;
      pot_p00 = -9999;
      liveP = -9999;
 
      dim_vtx_x = 0.;
      dim_vtx_y = 0.;
      dim_vtx_z = 0.;
      dim_chisq_vtx = 0.;
      dim_chisq_upstream = 0.;
      dim_chisq_target = 0.;
      dim_chisq_dump = 0.;


      dim_pT = 0.;
      dim_xT = 0.;
      dim_xB = 0.;
      dim_xF = 0.;
      dim_costh = 0.;
      dim_phi = 0.;
      dim_mass = 0.;
      ntrk_pz_st3 = -99999.;

      runN= rec_unmixed->getRunID();
      occuD1 = occuD1_;
      occuD2 = occuD2_;
      occuD3p = occuD3p_;
      occuD3m = occuD3m_;
      TargetPos = TargetPos_;
      rfp00c = rfp00c_;
      pot_p00 = pot_p00_;
      liveP = liveP_;

      

      int TrkMult = rec_unmixed->getNTracks();
      //cout << "Track multiplicity: "<<TrkMult<<endl;
      //double xpos_st1, xneg_st1,ypos_st1, yneg_st1;

      for(int j = 0; j<TrkMult; j++)
	{
	  SRecTrack recTrk = rec_unmixed->getTrack(j);
	    
	  if (recTrk.getCharge()<0)
	    { 
	      ntrk_x_t = recTrk.getTargetPos().X();
	      ntrk_y_t = recTrk.getTargetPos().Y();
	      ntrk_x_d = recTrk.getDumpPos().X();
	      ntrk_y_d = recTrk.getDumpPos().Y();
	      //double x,y; 	
	      recTrk.getPositionSt1(ntrk_x_st1,ntrk_y_st1);
	      recTrk.getMomentumSt1(ntrk_px_st1,ntrk_py_st1,ntrk_pz_st1);
	      recTrk.getPositionSt3(ntrk_x_st3, ntrk_y_st3);
	      recTrk.getMomentumSt3(ntrk_px_st3,ntrk_py_st3,ntrk_pz_st3);
	      ntrk_chisq_vtx = recTrk.getChisqVertex();
	      ntrk_vtx_z =  recTrk.getZVertex();
	      ntrk_chisq_upstream = recTrk.getChisqUpstream();
	      ntrk_chisq_target = recTrk.getChisqTarget();
	      ntrk_chisq_dump = recTrk.getChisqDump();
	      ntrk_nHits = recTrk.getNHits();
	    }
	  if (recTrk.getCharge()>0)
	    {
	      ptrk_x_t = recTrk.getTargetPos().X();
	      ptrk_y_t = recTrk.getTargetPos().Y();
	      ptrk_x_d = recTrk.getDumpPos().X();  
	      ptrk_y_d = recTrk.getDumpPos().Y();
	      recTrk.getPositionSt1(ptrk_x_st1, ptrk_y_st1);
	      recTrk.getMomentumSt1(ptrk_px_st1,ptrk_py_st1,ptrk_pz_st1);
	      recTrk.getPositionSt3(ptrk_x_st3, ptrk_y_st3);
	      recTrk.getMomentumSt3(ptrk_px_st3,ptrk_py_st3,ptrk_pz_st3);
	      ptrk_chisq_vtx = recTrk.getChisqVertex();
	      ptrk_vtx_z =  recTrk.getZVertex();
	      ptrk_chisq_upstream = recTrk.getChisqUpstream();
	      ptrk_chisq_target = recTrk.getChisqTarget();
	      ptrk_chisq_dump = recTrk.getChisqDump();
	      ptrk_nHits = recTrk.getNHits();
	    }
         
	    
	}
 
      //if(fabs(xpos_st1+xneg_st1)>13.) continue;
 
      for (int i =0; i<rec_unmixed->getNDimuons(); i++)
	{
    	  SRecDimuon recDimuon = rec_unmixed->getDimuon(i);

          dim_vtx_x = recDimuon.vtx.X();
	  dim_vtx_y = recDimuon.vtx.Y();
	  dim_vtx_z = recDimuon.vtx.Z();

	  dim_px = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();
	  dim_py = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();
	  dim_pz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();

          dim_chisq_vtx = recDimuon.chisq_kf;
   	  dim_chisq_upstream = recDimuon.chisq_upstream;
          dim_chisq_target = recDimuon.chisq_target;
          dim_chisq_dump = recDimuon.chisq_dump;

          //double neg_z_vtx = recDimuon.vtx_neg.Z();
          //double pos_z_vtx = recDimuon.vtx_pos.Z();
          dim_pT = recDimuon.pT;
          dim_xT = recDimuon.x1;
	  dim_xB = recDimuon.x2;
          dim_xF = recDimuon.xF;
 	  dim_costh = recDimuon.costh;
	  dim_phi = recDimuon.phi;
          dim_mass = recDimuon.mass;
          

	}
      result->Fill(); 



     
    }

  fout->cd();
  result->Write();


  ///============================================================================================
  ///------------------------------------mixed---------------------------------
  ///


  int ptrk_TargetPos, ptrk_occuD1,  ptrk_occuD2,  ptrk_occuD3m, ptrk_occuD3p;
  float ptrk_rfp00c, ptrk_pot_p00, ptrk_liveP; 



  int ntrk_TargetPos, ntrk_occuD1,  ntrk_occuD2,  ntrk_occuD3m, ntrk_occuD3p;
  float ntrk_rfp00c, ntrk_pot_p00, ntrk_liveP; 

  TTree* result_mix= new TTree("result_mix", "The tree for final results");
  /// Run and event level info
  result_mix->Branch("runN", &runN, "runN/I");
  result_mix->Branch("spillID", &spillID, "spillID/I");
  result_mix->Branch("ptrk_occuD1", &ptrk_occuD1, "ptrk_occuD1/I");///DC occupancy at st. 1
  result_mix->Branch("ptrk_occuD2", &ptrk_occuD2, "ptrk_occuD2/I"); ///DC occupancy at st. 2
  result_mix->Branch("ptrk_occuD3m", &ptrk_occuD3m, "ptrk_occuD3m/I"); ///DC occupancy at st. 3m
  result_mix->Branch("ptrk_occuD3p", &ptrk_occuD3p, "ptrk_occuD3p/I"); ///DC occupancy at st. 3p
  result_mix->Branch("ptrk_rfp00c", &ptrk_rfp00c, "ptrk_rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result_mix->Branch("ptrk_pot_p00", &ptrk_pot_p00, "ptrk_pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result_mix->Branch("ptrk_liveP", &ptrk_liveP, "ptrk_liveP/f");
  result_mix->Branch("ptrk_TargetPos", &ptrk_TargetPos, "ptrk_TargetPos/I"); /// Position of target

  result_mix->Branch("ntrk_occuD1", &ntrk_occuD1, "ntrk_occuD1/I");///DC occupancy at st. 1
  result_mix->Branch("ntrk_occuD2", &ntrk_occuD2, "ntrk_occuD2/I"); ///DC occupancy at st. 2
  result_mix->Branch("ntrk_occuD3m", &ntrk_occuD3m, "ntrk_occuD3m/I"); ///DC occupancy at st. 3m
  result_mix->Branch("ntrk_occuD3p", &ntrk_occuD3p, "ntrk_occuD3p/I"); ///DC occupancy at st. 3p
  result_mix->Branch("ntrk_rfp00c", &ntrk_rfp00c, "ntrk_rfp00c/f"); ///Pedestal corrected QIE count corresponding to triggered bucket
  result_mix->Branch("ntrk_pot_p00", &ntrk_pot_p00, "ntrk_pot_p00/f"); /// Proton number corresponding to triggered bucket (obtained from rfp00c)
  result_mix->Branch("ntrk_liveP", &ntrk_liveP, "ntrk_liveP/f");
  result_mix->Branch("ntrk_TargetPos", &ntrk_TargetPos, "ntrk_TargetPos/I"); /// Position of target

  ///Dimuon level info
  result_mix->Branch("dim_mass", &dim_mass, "dim_mass/f");
  result_mix->Branch("dim_costh", &dim_costh, "dim_costh/f");
  result_mix->Branch("dim_phi", &dim_phi, "dim_phi/f");
  result_mix->Branch("dim_pT", &dim_pT, "dim_pT/f");
  result_mix->Branch("dim_xF", &dim_xF, "dim_xF/f");
  result_mix->Branch("dim_xT", &dim_xT, "dim_xT/f");
  result_mix->Branch("dim_xB", &dim_xB, "dim_xB/f");
  result_mix->Branch("dim_px", &dim_px, "dim_px/f");
  result_mix->Branch("dim_py", &dim_py, "dim_py/f");
  result_mix->Branch("dim_pz", &dim_pz, "dim_pz/f");
  result_mix->Branch("dim_vtx_x", &dim_vtx_x, "dim_vtx_x/f");
  result_mix->Branch("dim_vtx_y", &dim_vtx_y, "dim_vtx_y/f");
  result_mix->Branch("dim_vtx_z", &dim_vtx_z, "dim_vtx_z/f");
  result_mix->Branch("dim_chisq_upstream", &dim_chisq_upstream, "dim_chisq_upstream/f");
  result_mix->Branch("dim_chisq_target", &dim_chisq_target, "dim_chisq_target/f");
  result_mix->Branch("dim_chisq_dump", &dim_chisq_dump, "dim_chisq_dump/f");
  result_mix->Branch("dim_chisq_vtx", &dim_chisq_vtx, "dim_chisq_vtx/f");



  ///Trk level info
  //
 
  result_mix->Branch("ptrk_vtx_z", &ptrk_vtx_z, "ptrk_vtx_z/f");
  result_mix->Branch("ptrk_x_t", &ptrk_x_t, "ptrk_x_t/f");
  result_mix->Branch("ptrk_y_t", &ptrk_y_t, "ptrk_y_t/f");
  result_mix->Branch("ptrk_x_d", &ptrk_x_d, "ptrk_x_d/f");
  result_mix->Branch("ptrk_y_d", &ptrk_y_d, "ptrk_y_d/f");

  result_mix->Branch("ptrk_x_st1", &ptrk_x_st1, "ptrk_x_st1/d");
  result_mix->Branch("ptrk_y_st1", &ptrk_y_st1, "ptrk_y_st1/d");

  result_mix->Branch("ptrk_px_st1", &ptrk_px_st1, "ptrk_px_st1/d");
  result_mix->Branch("ptrk_py_st1", &ptrk_py_st1, "ptrk_py_st1/d");
  result_mix->Branch("ptrk_pz_st1", &ptrk_pz_st1, "ptrk_pz_st1/d");

  result_mix->Branch("ptrk_x_st3", &ptrk_x_st3, "ptrk_x_st3/d");
  result_mix->Branch("ptrk_y_st3", &ptrk_y_st3, "ptrk_y_st3/d");

  result_mix->Branch("ptrk_px_st3", &ptrk_px_st3, "ptrk_px_st3/d");
  result_mix->Branch("ptrk_py_st3", &ptrk_py_st3, "ptrk_py_st3/d");
  result_mix->Branch("ptrk_pz_st3", &ptrk_pz_st3, "ptrk_pz_st3/d");

  result_mix->Branch("ptrk_chisq_upstream", &ptrk_chisq_upstream, "ptrk_chisq_upstream/f");
  result_mix->Branch("ptrk_chisq_target", &ptrk_chisq_target, "ptrk_chisq_target/f");
  result_mix->Branch("ptrk_chisq_dump", &ptrk_chisq_dump, "ptrk_chisq_dump/f");
  result_mix->Branch("ptrk_chisq_vtx", &ptrk_chisq_vtx, "ptrk_chisq_vtx/f");
  result_mix->Branch("ptrk_nHits", &ptrk_nHits, "ptrk_nHits/I");


  result_mix->Branch("ntrk_vtx_z", &ntrk_vtx_z, "ntrk_vtx_z/f");
  result_mix->Branch("ntrk_x_t", &ntrk_x_t, "ntrk_x_t/f");
  result_mix->Branch("ntrk_y_t", &ntrk_y_t, "ntrk_y_t/f");
  result_mix->Branch("ntrk_x_d", &ntrk_x_d, "ntrk_x_d/f");
  result_mix->Branch("ntrk_y_d", &ntrk_y_d, "ntrk_y_d/f");
  result_mix->Branch("ntrk_x_st1", &ntrk_x_st1, "ntrk_x_st1/d");
  result_mix->Branch("ntrk_y_st1", &ntrk_y_st1, "ntrk_y_st1/d");

  result_mix->Branch("ntrk_px_st1", &ntrk_px_st1, "ntrk_px_st1/d");
  result_mix->Branch("ntrk_py_st1", &ntrk_py_st1, "ntrk_py_st1/d");
  result_mix->Branch("ntrk_pz_st1", &ntrk_pz_st1, "ntrk_pz_st1/d");

  result_mix->Branch("ntrk_x_st3", &ntrk_x_st3, "ntrk_x_st3/d");
  result_mix->Branch("ntrk_y_st3", &ntrk_y_st3, "ntrk_y_st3/d");

  result_mix->Branch("ntrk_px_st3", &ntrk_px_st3, "ntrk_px_st3/d");
  result_mix->Branch("ntrk_py_st3", &ntrk_py_st3, "ntrk_py_st3/d");
  result_mix->Branch("ntrk_pz_st3", &ntrk_pz_st3, "ntrk_pz_st3/d");

  result_mix->Branch("ntrk_chisq_upstream", &ntrk_chisq_upstream, "ntrk_chisq_upstream/f");
  result_mix->Branch("ntrk_chisq_target", &ntrk_chisq_target, "ntrk_chisq_target/f");
  result_mix->Branch("ntrk_chisq_dump", &ntrk_chisq_dump, "ntrk_chisq_dump/f");
  result_mix->Branch("ntrk_chisq_vtx", &ntrk_chisq_vtx, "ntrk_chisq_vtx/f");

  result_mix->Branch("ntrk_nHits", &ntrk_nHits, "ntrk_nHits/I");

  ///Read mix tree

  TTree *mixed = (TTree*) file_in->Get("save_mix");

  SRecEvent* rec_mixed = new SRecEvent();

  mixed->SetBranchAddress("recEvent", &rec_mixed);

  int plus_occuD1, plus_occuD2, plus_occuD3p, plus_occuD3m,plus_TargetPos;
  float plus_rfp00c, plus_pot_p00, plus_liveP;

  int minus_occuD1, minus_occuD2, minus_occuD3p, minus_occuD3m,minus_TargetPos;
  float minus_rfp00c, minus_pot_p00, minus_liveP;
 
  mixed->SetBranchAddress("plus_occuD1", &plus_occuD1);
  mixed->SetBranchAddress("plus_occuD2", &plus_occuD2);
  mixed->SetBranchAddress("plus_occuD3p", &plus_occuD3p);
  mixed->SetBranchAddress("plus_occuD3m", &plus_occuD3m);
  mixed->SetBranchAddress("plus_TargetPos", &plus_TargetPos);
  mixed->SetBranchAddress("plus_rfp00c", &plus_rfp00c);
  mixed->SetBranchAddress("plus_pot_p00", &plus_pot_p00);
  mixed->SetBranchAddress("plus_liveP", &plus_liveP);

  mixed->SetBranchAddress("minus_occuD1", &minus_occuD1);
  mixed->SetBranchAddress("minus_occuD2", &minus_occuD2);
  mixed->SetBranchAddress("minus_occuD3p", &minus_occuD3p);
  mixed->SetBranchAddress("minus_occuD3m", &minus_occuD3m);
  mixed->SetBranchAddress("minus_TargetPos", &minus_TargetPos);
  mixed->SetBranchAddress("minus_rfp00c", &minus_rfp00c);
  mixed->SetBranchAddress("minus_pot_p00", &minus_pot_p00);
  mixed->SetBranchAddress("minus_liveP", &minus_liveP);





  ///
  for(int i_evt=0;i_evt<mixed->GetEntries();i_evt++) 
    {
	
      mixed->GetEntry(i_evt);
      if (rec_mixed->getNDimuons()<1) continue;

      ptrk_occuD1 = -9999;
      ptrk_occuD2 =-9999;
      ptrk_occuD3p = -9999;
      ptrk_occuD3m = -9999;
      ptrk_TargetPos = -9999;
      ptrk_rfp00c = -9999;
      ptrk_pot_p00 = -9999;
      ptrk_liveP = -9999;
      ptrk_occuD1 = -9999;
      ptrk_occuD2 =-9999;
      ptrk_occuD3p = -9999;
      ptrk_occuD3m = -9999;
      ptrk_TargetPos = -9999;
      ptrk_rfp00c = -9999;
      ptrk_pot_p00 = -9999;
      ptrk_liveP = -9999;

      dim_vtx_x = 0.;
      dim_vtx_y = 0.;
      dim_vtx_z = 0.;
      dim_chisq_vtx = 0.;
      dim_chisq_upstream = 0.;
      dim_chisq_target = 0.;
      dim_chisq_dump = 0.;


      dim_pT = 0.;
      dim_xT = 0.;
      dim_xB = 0.;
      dim_xF = 0.;
      dim_costh = 0.;
      dim_phi = 0.;
      dim_mass = 0.;

      runN= rec_mixed->getRunID();

      ptrk_occuD1 = plus_occuD1;
      ptrk_occuD2 = plus_occuD2;
      ptrk_occuD3p = plus_occuD3p;
      ptrk_occuD3m = plus_occuD3m;
      ptrk_TargetPos = plus_TargetPos;
      ptrk_rfp00c = plus_rfp00c;
      ptrk_pot_p00 = plus_pot_p00;
      ptrk_liveP = plus_liveP;


      ntrk_occuD1 = minus_occuD1;
      ntrk_occuD2 = minus_occuD2;
      ntrk_occuD3p = minus_occuD3p;
      ntrk_occuD3m = minus_occuD3m;
      ntrk_TargetPos = minus_TargetPos;
      ntrk_rfp00c = minus_rfp00c;
      ntrk_pot_p00 = minus_pot_p00;
      ntrk_liveP = minus_liveP;


      double xpos_st1, xneg_st1,ypos_st1, yneg_st1;

      for(int j = 0; j<rec_mixed->getNTracks(); j++)
	{
	  SRecTrack recTrk = rec_mixed->getTrack(j);
	    
	  if (recTrk.getCharge()>0)
	    {
	      ptrk_x_t = recTrk.getTargetPos().X();
	      ptrk_y_t = recTrk.getTargetPos().Y();
	      ptrk_x_d = recTrk.getDumpPos().X();
	      ptrk_y_d = recTrk.getDumpPos().Y();
	      recTrk.getPositionSt1(xpos_st1, ypos_st1);
	      recTrk.getMomentumSt1(ptrk_px_st1,ptrk_py_st1,ptrk_pz_st1);
	      recTrk.getPositionSt3(ptrk_x_st3, ptrk_y_st3);
	      recTrk.getMomentumSt3(ptrk_px_st3,ptrk_py_st3,ptrk_pz_st3);
	      ptrk_chisq_vtx = recTrk.getChisqVertex();
	      ptrk_vtx_z =  recTrk.getZVertex();
	      ptrk_chisq_upstream = recTrk.getChisqUpstream();
	      ptrk_chisq_target = recTrk.getChisqTarget();
	      ptrk_chisq_dump = recTrk.getChisqDump();
	      ptrk_nHits = recTrk.getNHits();	        
	    }
	  if (recTrk.getCharge()<0)
	    {

	      ntrk_x_t = recTrk.getTargetPos().X();
	      ntrk_y_t = recTrk.getTargetPos().Y();
	      ntrk_x_d = recTrk.getDumpPos().X();
	      ntrk_y_d = recTrk.getDumpPos().Y();
	      recTrk.getPositionSt1(xneg_st1, yneg_st1);
	      recTrk.getMomentumSt1(ntrk_px_st1,ntrk_py_st1,ntrk_pz_st1);
	      recTrk.getPositionSt3(ntrk_x_st3, ntrk_y_st3);
	      recTrk.getMomentumSt3(ntrk_px_st3,ntrk_py_st3,ntrk_pz_st3);
	      ntrk_chisq_vtx = recTrk.getChisqVertex();
	      ntrk_vtx_z =  recTrk.getZVertex();
	      ntrk_chisq_upstream = recTrk.getChisqUpstream();
	      ntrk_chisq_target = recTrk.getChisqTarget();
	      ntrk_chisq_dump = recTrk.getChisqDump();
	      ntrk_nHits = recTrk.getNHits();
	    }
	}

 
      for (int i =0; i< rec_mixed->getNDimuons(); i++)
	{
    	  SRecDimuon recDimuon = rec_mixed->getDimuon(i);

          dim_vtx_x = recDimuon.vtx.X();
	  dim_vtx_y = recDimuon.vtx.Y();
	  dim_vtx_z = recDimuon.vtx.Z();

          dim_px = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();;
          dim_py = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();;
          dim_pz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();;

          dim_chisq_vtx = recDimuon.chisq_kf;
   	  dim_chisq_upstream = recDimuon.chisq_upstream;
          dim_chisq_target = recDimuon.chisq_target;
          dim_chisq_dump = recDimuon.chisq_dump;

          dim_pT = recDimuon.pT;
          dim_xT = recDimuon.x1;
	  dim_xB = recDimuon.x2;
          dim_xF = recDimuon.xF;
 	  dim_costh = recDimuon.costh;
	  dim_phi = recDimuon.phi;
          dim_mass = recDimuon.mass;
	 

	}

      result_mix->Fill();          

    }

  fout->cd();
  result_mix->Write();


}


void FinalTree::AnalyzeGMC(char* input_file)
{

  ///Dimuon level info
  float dim_mass, dim_costh, dim_phi, dim_xF, dim_xT,  dim_xB, dim_pT, dim_vtx_x,  dim_vtx_y, dim_vtx_z;
  float dim_px, dim_py,  dim_pz;
  float dim_chisq_target, dim_chisq_dump, dim_chisq_vtx, dim_chisq_upstream;


  TFile* fout = new TFile ("result_gmc/result_mcDY_dump.root","recreate");
  TTree* result_mc= new TTree("result_mc", "The tree for final results");


  ///Dimuon level info

  result_mc->Branch("weight", &weight, "weight/f");
  result_mc->Branch("fpga1", &fpga1, "fpga1/I"); 

  result_mc->Branch("true_dim_mass", &true_dim_mass, "true_dim_mass/f");
  result_mc->Branch("true_dim_pT", &true_dim_pT, "true_dim_pT/f");
  result_mc->Branch("true_dim_xF", &true_dim_xF, "true_dim_xF/f");
  // result_mc->Branch("true_dim_xT", &true_dim_xT, "true_dim_xT/f");


  result_mc->Branch("dim_mass", &dim_mass, "dim_mass/f");
  result_mc->Branch("dim_costh", &dim_costh, "dim_costh/f");
  result_mc->Branch("dim_phi", &dim_phi, "dim_phi/f");
  result_mc->Branch("dim_pT", &dim_pT, "dim_pT/f");
  result_mc->Branch("dim_xF", &dim_xF, "dim_xF/f");
  result_mc->Branch("dim_xT", &dim_xT, "dim_xT/f");
  result_mc->Branch("dim_xB", &dim_xB, "dim_xB/f");
  result_mc->Branch("dim_px", &dim_px, "dim_px/f");
  result_mc->Branch("dim_py", &dim_py, "dim_py/f");
  result_mc->Branch("dim_pz", &dim_pz, "dim_pz/f");
  result_mc->Branch("dim_vtx_x", &dim_vtx_x, "dim_vtx_x/f");
  result_mc->Branch("dim_vtx_y", &dim_vtx_y, "dim_vtx_y/f");
  result_mc->Branch("dim_vtx_z", &dim_vtx_z, "dim_vtx_z/f");
  result_mc->Branch("dim_chisq_upstream", &dim_chisq_upstream, "dim_chisq_upstream/f");
  result_mc->Branch("dim_chisq_target", &dim_chisq_target, "dim_chisq_target/f");
  result_mc->Branch("dim_chisq_dump", &dim_chisq_dump, "dim_chisq_dump/f");
  result_mc->Branch("dim_chisq_vtx", &dim_chisq_vtx, "dim_chisq_vtx/f");
 
  ///-----
  // TFile *file_in = TFile::Open("vertex_mc_drellyan_dump_M027_S001_messy.root");
  TFile* file_in = new TFile (input_file, "read");
  TTree *mc_save = (TTree*) file_in->Get("save");
  SRecEvent* mc_rec = new SRecEvent();
  SRawMCEvent *mc_raw = new SRawMCEvent();

  mc_save->SetBranchAddress("recEvent", &mc_rec);
  mc_save->SetBranchAddress("rawEvent", &mc_raw);

  cout<<" No. of entries: "<<mc_save->GetEntries()<<endl;
  ///
  for(int i_evt=0;i_evt<mc_save->GetEntries();i_evt++) 
    {
      //	mc_save->LoadBaskets(999999999999);
      mc_save->GetEntry(i_evt);
      if(mc_rec->getNDimuons()<1) continue; 

      dim_vtx_x = 0.;
      dim_vtx_y = 0.;
      dim_vtx_z = 0.;
      dim_chisq_vtx = 0.;
      dim_chisq_upstream = 0.;
      dim_chisq_target = 0.;
      dim_chisq_dump = 0.;


      dim_pT = 0.;
      dim_xT = 0.;
      dim_xB = 0.;
      dim_xF = 0.;
      dim_costh = 0.;
      dim_phi = 0.;
      dim_mass = 0.;

      //if(fabs(xpos_st1+xneg_st1)>13.) continue;
      //cout<<" weight: "<< mc_raw->weight<<endl;
      fpga1 = (mc_raw->isTriggeredBy(SRawMCEvent::MATRIX1));
      true_dim_mass = mc_raw->mass;
      true_dim_xF = mc_raw->xF;
      true_dim_pT = mc_raw->pT;
      weight = mc_raw->weight;
 
      for (int i =0; i<mc_rec->getNDimuons(); i++)
	{
    	  SRecDimuon recDimuon = mc_rec->getDimuon(i);

          dim_vtx_x = recDimuon.vtx.X();
	  dim_vtx_y = recDimuon.vtx.Y();
	  dim_vtx_z = recDimuon.vtx.Z();

	  dim_px = recDimuon.p_pos.Vect().X()+recDimuon.p_neg.Vect().X();;
	  dim_py = recDimuon.p_pos.Vect().Y()+recDimuon.p_neg.Vect().Y();;
	  dim_pz = recDimuon.p_pos.Vect().Z()+recDimuon.p_neg.Vect().Z();;

          dim_chisq_vtx = recDimuon.chisq_kf;
   	  dim_chisq_upstream = recDimuon.chisq_upstream;
          dim_chisq_target = recDimuon.chisq_target;
          dim_chisq_dump = recDimuon.chisq_dump;

          dim_pT = recDimuon.pT;
          dim_xT = recDimuon.x1;
	  dim_xB = recDimuon.x2;
          dim_xF = recDimuon.xF;
 	  dim_costh = recDimuon.costh;
	  dim_phi = recDimuon.phi;
          dim_mass = recDimuon.mass;
          

	}
      result_mc->Fill(); 
     
    }
  fout->cd();
  result_mc->Write();
}
