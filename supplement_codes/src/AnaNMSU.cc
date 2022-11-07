#include <fstream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <SRawEvent.h>
#include <SRecEvent.h>
#include <GeomSvc.h>
#include <E906DataCatalog.h>
#include "UtilBeam.h"
#include "UtilTdc.h"
#include "UtilHist.h"
#include "AnaNMSU.h"
using namespace std;

AnaNMSU::AnaNMSU(const std::string name)
  : AnaBase(name)
  , m_n_evt_all  (0)
  , m_n_evt_spill(0)
  , m_n_evt_fpga1(0)
  , m_n_evt_tgt  (0)
  ,m_tree(0)
{
  UseRawEvent(true);
  UseRecEvent(true);
}

AnaNMSU::~AnaNMSU()
{
  pos_px.clear();
  pos_py.clear();
  pos_pz.clear();
  pos_E.clear();

  neg_px.clear();
  neg_py.clear();
  neg_pz.clear();
  neg_E.clear();
  dim_M.clear();


}

void AnaNMSU::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();
  //GeomSvc* geom = GeomSvc::instance();

  m_tree = new TTree("tree", "Tree Created by AnaNMSU");
  //m_tree->Branch("data", &m_data);
  m_tree->Branch("run_ID", &run_ID, "run_ID/I");
  m_tree->Branch("spill_ID", &spill_ID, "spill_ID/I");
  m_tree->Branch("event_ID", &event_ID, "event_ID/I");
  m_tree->Branch("TargetPos", &TargetPos, "TargetPos/I");
  m_tree->Branch("rfp00c", &rfp00c, "rfp00c/f");
  m_tree->Branch("pot_p00", &pot_p00, "pot_p00/f");
  m_tree->Branch("liveP", &liveP, "liveP/f");
  m_tree->Branch("fpga1", &fpga1, "fpga1/I");
  m_tree->Branch("fpga2", &fpga2, "fpga2/I");
  m_tree->Branch("fpga3", &fpga3, "fpga3/I");
  m_tree->Branch("fpga4", &fpga4, "fpga4/I");
  m_tree->Branch("fpga5", &fpga5, "fpga5/I");


  m_tree->Branch("DimMult", &DimMult, "DimMult/I");
  m_tree->Branch("TrkMult", &TrkMult, "TrkMult/I");
  m_tree->Branch("PosTrkMult", &PosTrkMult, "PosTrkMult/I");
  m_tree->Branch("NegTrkMult", &NegTrkMult, "NegTrkMult/I");
  m_tree->Branch("occuD1", &occuD1, "occuD1/I");
  m_tree->Branch("occuD2", &occuD2, "occuD2/I");
  m_tree->Branch("occuD3p", &occuD3p, "occuD3p/I");
  m_tree->Branch("occuD3m", &occuD3m, "occuD3m/I");

  m_tree->Branch("pos_tracks",&pos_tracks);
  m_tree->Branch("neg_tracks",&neg_tracks);
/*
  m_tree->Branch("pos_px", &pos_px);
  m_tree->Branch("pos_py", &pos_py);
  m_tree->Branch("pos_pz", &pos_pz);
  m_tree->Branch("pos_E",  &pos_E) ;

  m_tree->Branch("neg_px", &neg_px);
  m_tree->Branch("neg_py", &neg_py);
  m_tree->Branch("neg_pz", &neg_pz);
  m_tree->Branch("neg_E",  &neg_E);
*/
  m_tree->Branch("dim_M", &dim_M);

  m_tree->Branch("dim_vtx_z", &dim_vtx_z, "dim_vtx_z/f");

  

}

void AnaNMSU::End()
{
  OFS() << "Event counts:\n"
        << "  All    = " << m_n_evt_all << "\n"
        << "  Spill  = " << m_n_evt_spill << "\n"
        << "  FPGA1  = " << m_n_evt_fpga1 << "\n"
        << "  Target = " << m_n_evt_tgt << "\n";
  AnaBase::End();
}

void AnaNMSU::AnalyzeEvent()
{
  //GeomSvc* p_geomSvc = GeomSvc::instance();
  m_n_evt_all++;
  ResetValues();
  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  SRecEvent* rec = GetRecEvent();
  run_ID   = raw->getRunID();
  spill_ID = raw->getSpillID();
  event_ID = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
//  cout<<"eventID: "<<event_id<<endl;

  //if(rec->getTargetPos() < 1 || raw->getTargetPos() > 7) continue;

  if (! e906sc->SpillExists(spill_ID)) return;
  m_n_evt_spill++;

  if (raw->isTriggeredBy(SRawEvent::MATRIX1)) m_n_evt_fpga1++;
  else return;
  
  fpga1 = (raw->isTriggeredBy(SRawEvent::MATRIX1));
  fpga2 = (raw->isTriggeredBy(SRawEvent::MATRIX2));
  fpga3 = (raw->isTriggeredBy(SRawEvent::MATRIX3));
  fpga4 = (raw->isTriggeredBy(SRawEvent::MATRIX4));
  if(fpga1<1) cout<<"fpga1: "<<fpga1<<" EventId: "<<event_ID<<endl;

  //int turn_id = raw->getTurnID();
  //int rf_id   = raw->getRFID();
  
  liveP =  UtilBeam::PoTLive(spill_ID);
  TargetPos = raw->getTargetPos();

  int rfp00 = raw->getIntensity( 0);
  double qie_ped = e906sc->GetQIEPedestal(spill_ID);
  rfp00c = rfp00 - qie_ped;
  double coef = UtilBeam::PoTPerQIE(spill_ID);
  pot_p00 = coef * rfp00c;

 
  occuD1 = raw->getNHitsInD1();
  occuD2 = raw->getNHitsInD2();
  occuD3p = raw->getNHitsInD3p();
  occuD3m = raw->getNHitsInD3m();
  ///
  /// Track Info
  ///
  TrkMult = rec->getNTracks();
  ///
  ///
  ///
  for (int i =0; i< TrkMult; i++)
  { 
   
    SRecTrack recTrack = rec->getTrack(i);
    if (recTrack.getCharge()>0)
    {
      pos_tracks.push_back(rec->getTrack(i));

      pos_px.push_back(recTrack.getMomentumVertex().Px());
      pos_py.push_back(recTrack.getMomentumVertex().Py());
      pos_pz.push_back(recTrack.getMomentumVertex().Pz());
      pos_E.push_back(recTrack.getMomentumVertex().E());
      ++PosTrkMult;
    }

    if (recTrack.getCharge()<0)
    {
       neg_tracks.push_back(rec->getTrack(i));

       neg_px.push_back(recTrack.getMomentumVertex().Px());
       neg_py.push_back(recTrack.getMomentumVertex().Py());
       neg_pz.push_back(recTrack.getMomentumVertex().Pz());
       neg_E.push_back(recTrack.getMomentumVertex().E());
      ++NegTrkMult;
    }
    
  }

  ///
  /// Dimuon info
  ///

  DimMult = rec->getNDimuons();


  for (int i =0; i< DimMult; i++)
  {
    SRecDimuon recDimuon = rec->getDimuon(i);
    //cout << "mass of the dimuon: "<< recDimuon.mass<< endl;
    dim_M.push_back(recDimuon.mass);
  }
   

  ///
  /// Fill the output tree
  m_tree->Fill();
}
int AnaNMSU::ResetValues()
{
  fpga1=0;
  fpga2=0;
  fpga3=0;
  fpga4=0;
  
  DimMult=0;
  occuD1=0;
  occuD2=0;
  occuD3p=0;
  occuD3m=0;
  TrkMult=0;
  PosTrkMult=0;
  NegTrkMult=0;
  TargetPos=-999;
  
  pos_tracks.clear();
  neg_tracks.clear();

  pos_px.clear();
  pos_py.clear();
  pos_pz.clear();
  pos_E.clear();

  neg_px.clear();
  neg_py.clear();
  neg_pz.clear();
  neg_E.clear();
  dim_M.clear();
  
  return 0;
}
