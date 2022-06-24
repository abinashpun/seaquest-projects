#include <fstream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <SRawEvent.h>
#include <E906DataCatalog.h>
#include "UtilBeam.h"
#include "AnaNim3.h"
using namespace std;

AnaNim3::AnaNim3(const std::string name)
  : AnaBase(name)
  , m_tree(0)
{
  UseRawEvent(true);
  //UseRecEvent(true);
}

AnaNim3::~AnaNim3()
{
  ;
}

void AnaNim3::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();

  m_tree = new TTree("tree", "Created by AnaNim3");
  m_tree->Branch("data", &m_data);

  m_h1_count = new TH1D("h1_count", "Event counts", 20, 0.5, 20.5);

  int     inte_num;
  double* inte_list;
  UtilBeam::ListOfRfValues(inte_num, inte_list);
  m_h1_rfp01 = new TH1D("h1_rfp01", "", inte_num-1, inte_list);
  m_h1_rfp00 = new TH1D("h1_rfp00", "", inte_num-1, inte_list);
  m_h1_rfm01 = new TH1D("h1_rfm01", "", inte_num-1, inte_list);
  m_h1_rfp01->GetXaxis()->SetRangeUser(0, 4000);
  m_h1_rfp00->GetXaxis()->SetRangeUser(0, 4000);
  m_h1_rfm01->GetXaxis()->SetRangeUser(0, 4000);

  m_h1_pot_p01 = new TH1D("h1_pot_p01", "", 1280, 0, 128000);
  m_h1_pot_p00 = new TH1D("h1_pot_p00", "", 1280, 0, 128000);
  m_h1_pot_m01 = new TH1D("h1_pot_m01", "", 1280, 0, 128000);
}

void AnaNim3::End()
{
  OFS() << "Event counts:\n"
        << "  All        = " << (int)m_h1_count->GetBinContent(1) << "\n"
        << "  Spill OK   = " << (int)m_h1_count->GetBinContent(2) << "\n"
        << "  Trigger OK = " << (int)m_h1_count->GetBinContent(3) << "\n";
  for (int ii = 1; ii <= 7; ii++) {
    OFS() << "  Target #" << ii << " = " << (int)m_h1_count->GetBinContent(4+ii) << "\n";
  }
  AnaBase::End();
}

void AnaNim3::AnalyzeEvent()
{
  m_h1_count->Fill(1); // All events

  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  int run_id   = raw->getRunID();
  int spill_id = raw->getSpillID();
  int event_id = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
  if (! e906sc->SpillExists(spill_id)) return;
  m_h1_count->Fill(2); // Spill OK

  if (! raw->isTriggeredBy(SRawEvent::NIM3)) return;
  m_h1_count->Fill(3); // Trigger OK

  int tgt_pos = raw->getTargetPos();
  m_h1_count->Fill(4 + tgt_pos); // Target counts

  //auto beamdaq = e906sc->BeamDaq(spill_id);
  //int inhi_thre = beamdaq->GetInt("Inh_thres");

  int rfp01 = raw->getIntensity( 1);
  int rfp00 = raw->getIntensity( 0);
  int rfm01 = raw->getIntensity(-1);
  m_h1_rfp01->Fill(rfp01);
  m_h1_rfp00->Fill(rfp00);
  m_h1_rfm01->Fill(rfm01);

  double qie_ped = e906sc->GetQIEPedestal(spill_id);
  float rfp01c = rfp01 - qie_ped;
  float rfp00c = rfp00 - qie_ped;
  float rfm01c = rfm01 - qie_ped;

  double coef = UtilBeam::PoTPerQIE(spill_id);
  double pot_p01 = coef * rfp01c;
  double pot_p00 = coef * rfp00c;
  double pot_m01 = coef * rfm01c;
  m_h1_pot_p01->Fill(pot_p01);
  m_h1_pot_p00->Fill(pot_p00);
  m_h1_pot_m01->Fill(pot_m01);

  m_data.run_id   =   run_id;
  m_data.spill_id = spill_id;
  m_data.event_id = event_id;
  m_data.turn_id  = raw->getTurnID();
  m_data.  rf_id  = raw->getRFID();
  m_data.rfp01    = rfp01;
  m_data.rfp00    = rfp00;
  m_data.rfm01    = rfm01;
  m_data.rfp01c   = rfp01c;
  m_data.rfp00c   = rfp00c;
  m_data.rfm01c   = rfm01c;
  m_data.pot_p01  = pot_p01;
  m_data.pot_p00  = pot_p00;
  m_data.pot_m01  = pot_m01;
  m_tree->Fill();
}
