#include <fstream>
#include <sstream>
#include <iomanip>
#include <TSystem.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <SRawEvent.h>
#include <E906DataCatalog.h>
#include "UtilBeam.h"
#include "UtilTarget.h"
#include "UtilTdc.h"
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

  double qie_ped = e906sc->GetQIEPedestal(spill_id);
  float rfp01c = rfp01 - qie_ped;
  float rfp00c = rfp00 - qie_ped;
  float rfm01c = rfm01 - qie_ped;

  double coef = UtilBeam::PoTPerQIE(spill_id);
  double pot_p01 = coef * rfp01c;
  double pot_p00 = coef * rfp00c;
  double pot_m01 = coef * rfm01c;

  int occ_D0, occ_D1, occ_D2, occ_D3;
  UtilTdc::GetOccupancy(raw, true, occ_D0, occ_D1, occ_D2, occ_D3);

  m_data.run_id   =   run_id;
  m_data.spill_id = spill_id;
  m_data.tgt_pos  = tgt_pos ;
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
  m_data.occ_D0   = occ_D0;
  m_data.occ_D1   = occ_D1;
  m_data.occ_D2   = occ_D2;
  m_data.occ_D3   = occ_D3;
  m_tree->Fill();
}

void AnaNim3::InitStep2()
{
  gSystem->mkdir("result", true);
  m_ofs_inte.open("result/inte.txt");
  m_file_out2 = new TFile("result/result.root", "RECREATE");

  int     inte_num;
  double* inte_list;
  UtilBeam::ListOfRfValues(inte_num, inte_list);
  
  ostringstream oss;
  for (int tgt = 0; tgt < 8; tgt++) {
    string name_tgt = UtilTarget::GetName(tgt);

    oss.str("");
    oss << "h2_turn_rf_tgt" << tgt;
    m_h2_turn_rf[tgt] = new TH2D(oss.str().c_str(), "", 369, 0.5, 369000.5,  588, 0.5, 588.5);
    oss.str("");
    oss << name_tgt << ";Turn ID;RF ID";
    m_h2_turn_rf[tgt]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_rfp00_tgt" << tgt;
    m_h1_rfp00[tgt] = new TH1D(oss.str().c_str(), "", inte_num-1, inte_list);
    oss.str("");
    oss << name_tgt << ";RF+00;N of NIM3 events";
    m_h1_rfp00[tgt]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_rfp00c_tgt" << tgt;
    m_h1_rfp00c[tgt] = new TH1D(oss.str().c_str(), "", inte_num-1, inte_list);
    oss.str("");
    oss << name_tgt << ";RF+00 - Pedestal;N of NIM3 events";
    m_h1_rfp00c[tgt]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_pot_p00_tgt" << tgt;
    m_h1_pot_p00[tgt] = new TH1D(oss.str().c_str(), "", 500, 0, 500e3);
    oss.str("");
    oss << name_tgt << ";PoT+00;N of NIM3 events";
    m_h1_pot_p00[tgt]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_occ_D1_tgt" << tgt;
    m_h1_occ_D1[tgt] = new TH1D(oss.str().c_str(), "", 100, 0, 600);
    oss.str("");
    oss << name_tgt << ";Occupancy D1;N of NIM3 events";
    m_h1_occ_D1[tgt]->SetTitle(oss.str().c_str());
  }
}

void AnaNim3::AnalyzeStep2(const int run_id, TFile* file_in)
{
  TTree* tree_in = (TTree*)file_in->Get("tree");
  if (! tree_in) {
    cout << "    Cannot get TTree." << endl;
    return;
  }
  static Nim3Data* data = 0;
  if (! data) data = new Nim3Data();
  tree_in->SetBranchAddress("data", &data);

  int n_evt = tree_in->GetEntries();
  for (int i_evt = 0; i_evt < n_evt; i_evt++) {
    tree_in->GetEntry(i_evt);
    int tgt_pos = data->tgt_pos;
    int turn_id = data->turn_id;
    int   rf_id = data->  rf_id;
    if (tgt_pos < 1 || tgt_pos > 7) {
      cout << "WARNING: tgt_pos = " << tgt_pos << ".  Skip." << endl;
      continue;
    }
    m_h2_turn_rf[tgt_pos]->Fill(turn_id, rf_id);
    m_h2_turn_rf[0      ]->Fill(turn_id, rf_id);

    if (turn_id < 50e3 || turn_id > 350e3 || rf_id < 100 || rf_id > 560) continue; // doc1801

    m_h1_rfp00[tgt_pos]->Fill(data->rfp00);
    m_h1_rfp00[0      ]->Fill(data->rfp00);

    m_h1_rfp00c[tgt_pos]->Fill(data->rfp00c);
    m_h1_rfp00c[0      ]->Fill(data->rfp00c);

    m_h1_pot_p00[tgt_pos]->Fill(data->pot_p00);
    m_h1_pot_p00[0      ]->Fill(data->pot_p00);

    m_h1_occ_D1[tgt_pos]->Fill(data->occ_D1);
    m_h1_occ_D1[0      ]->Fill(data->occ_D1);
  }
}

void AnaNim3::EndStep2()
{
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  gErrorIgnoreLevel = 1111;

  ostringstream oss;
  for (int tgt = 0; tgt < 8; tgt++) {
    gStyle->SetOptStat(0);
    c1->SetLogy(false);

    m_h2_turn_rf[tgt]->Draw("colz");
    oss.str("");
    oss << "result/" << m_h2_turn_rf[tgt]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());

    gStyle->SetOptStat(111110);

    TH1* h1_turn = m_h2_turn_rf[tgt]->ProjectionX("h1_turn");
    h1_turn->Draw();
    oss.str("");
    oss << "result/h1_turn_id_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());
    delete h1_turn;

    TH1* h1_rf = m_h2_turn_rf[tgt]->ProjectionY("h1_rf");
    h1_rf->Draw();
    oss.str("");
    oss << "result/h1_rf_id_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());
    delete h1_rf;

    c1->SetLogy(true);

    m_h1_rfp00[tgt]->GetXaxis()->SetRangeUser(0, 30e3);
    m_h1_rfp00[tgt]->Draw();
    oss.str("");
    oss << "result/h1_rfp00_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());

    m_h1_rfp00c[tgt]->GetXaxis()->SetRangeUser(0, 30e3);
    m_h1_rfp00c[tgt]->Draw();
    oss.str("");
    oss << "result/h1_rfp00c_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());

    m_h1_pot_p00[tgt]->Draw();
    oss.str("");
    oss << "result/h1_pot_p00_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());

    m_h1_occ_D1[tgt]->Draw();
    oss.str("");
    oss << "result/h1_occ_D1_tgt" << tgt << ".png";
    c1->SaveAs(oss.str().c_str());
  }

  gStyle->SetOptStat(0);
  c1->SetLogy(true);

  const int N_INTE = 3;
  const double INTE_EDGE[N_INTE+1] = { 0, 20e3, 50e3, 80e3 };

  THStack hs("hs", ";Beam Intensity (protons/bucket);Total protons (%/bin)");
  TLegend leg(0.7, 0.7, 0.9, 0.9);
  for (int tgt = 1; tgt <= 4; tgt++) {
    oss.str("");
    oss << "h1_pot_tot_tgt" << tgt;
    TH1* h1 = (TH1*)m_h1_pot_p00[tgt]->Clone(oss.str().c_str());
    h1->Rebin(10);
    h1->Sumw2();
    int n_bin = h1->GetNbinsX();
    for (int i_bin = 1; i_bin <= n_bin; i_bin++) {
      double cent = h1->GetBinCenter(i_bin);
      h1->SetBinContent(i_bin, cent * h1->GetBinContent(i_bin));
      h1->SetBinError  (i_bin, cent * h1->GetBinError  (i_bin));
    }
    h1->Scale( 100 / h1->Integral() );
    h1->UseCurrentStyle();
    h1->SetLineColor  (tgt);
    h1->SetMarkerColor(tgt);
    hs.Add(h1, "E1");
    leg.AddEntry(h1, UtilTarget::GetName(tgt).c_str(), "l");

    m_ofs_inte << tgt;
    for (int i_inte = 0; i_inte < N_INTE; i_inte++) {
      int bin_lo = h1->FindBin(INTE_EDGE[i_inte  ]);
      int bin_hi = h1->FindBin(INTE_EDGE[i_inte+1]);
      double err;
      double tot = h1->IntegralAndError(bin_lo, bin_hi, err);
      m_ofs_inte << "\t" << tot << "\t" << err;
    }
    m_ofs_inte << endl;

  }
  hs.Draw("nostack");
  leg.Draw();
  c1->SaveAs("result/h1_pot_tot.png");

  m_file_out2->Write();
  m_file_out2->Close();

  m_ofs_inte.close();
}
