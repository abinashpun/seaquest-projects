#include <fstream>
#include <sstream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <GeomSvc.h>
#include <E906DataCatalog.h>
#include "AnaOccupancy.h"
using namespace std;

const char* AnaOccupancy::NAME_TRIG[AnaOccupancy::N_TRIG] = { "fpga1", "nim3" };

AnaOccupancy::AnaOccupancy(const std::string name)
  : AnaBase(name)
{
  UseRawEvent(true);
  //UseRecEvent(true);
}

AnaOccupancy::~AnaOccupancy()
{
  ;
}

void AnaOccupancy::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();
  GeomSvc* geom = GeomSvc::instance();

  ostringstream oss;
  oss << setfill('0');

  h1_count = new TH1D("h1_count", "Event counts", 10, 0.5, 10.5);

  for (int i_pl = 1; i_pl <= N_PL; i_pl++) {
    string name = geom->getDetectorName  (i_pl);
    int   n_ele = geom->getPlaneNElements(i_pl);
    if (Verbosity() > 0) cout << "  " << i_pl << " " << name << " " << n_ele << endl;
    for (int tgt = 1; tgt <= N_TGT; tgt++) {
      for (int trig = 0; trig < N_TRIG; trig++) {
        oss.str("");
        oss << "h1_occ_all_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
        h1_occ_all[tgt][trig][i_pl] = new TH1D(oss.str().c_str(), "", n_ele+1, -0.5, n_ele+0.5);

        oss.str("");
        oss << "h1_occ_intime_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
        h1_occ_intime[tgt][trig][i_pl] = new TH1D(oss.str().c_str(), "", n_ele+1, -0.5, n_ele+0.5);

        oss.str("");
        oss << "Target " << tgt << " | " << NAME_TRIG[trig] << " | " << name << ";Occupancy;N of events";
        h1_occ_all   [tgt][trig][i_pl]->SetTitle(oss.str().c_str());
        h1_occ_intime[tgt][trig][i_pl]->SetTitle(oss.str().c_str());
      }
    }
  }
}

void AnaOccupancy::End()
{
  OFS() << "Event counts:\n"
        << "  All       = " << (int)h1_count->GetBinContent(1) << "\n"
        << "  Spill OK  = " << (int)h1_count->GetBinContent(2) << "\n"
        << "  Target OK = " << (int)h1_count->GetBinContent(3) << "\n";
  AnaBase::End();
}

void AnaOccupancy::AnalyzeEvent()
{
  h1_count->Fill(1); // All events

  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  //int run_id   = raw->getRunID();
  int spill_id = raw->getSpillID();
  //int event_id = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
  if (! e906sc->SpillExists(spill_id)) return;
  h1_count->Fill(2); // Spill OK

  int tgt_pos = raw->getTargetPos();
  if (tgt_pos < 1 || tgt_pos > 3) return;
  h1_count->Fill(3); // Target OK

  bool fpga1 = raw->isTriggeredBy(SRawEvent::MATRIX1);
  bool nim3  = raw->isTriggeredBy(SRawEvent::NIM3);
  if (!fpga1 && !nim3) return;
  if (fpga1) h1_count->Fill(4);
  if (nim3 ) h1_count->Fill(5);

  //int turn_id = raw->getTurnID();
  //int rf_id   = raw->getRFID();
  //int rfp00   = raw->getIntensity(0);

  int n_hits_all   [N_PL+1];
  int n_hits_intime[N_PL+1];
  memset(n_hits_all   , 0, sizeof(n_hits_all   ));
  memset(n_hits_intime, 0, sizeof(n_hits_intime));
  for (int i_hit = 0; i_hit < raw->getNHitsAll(); i_hit++) {
    Hit hit = raw->getHit(i_hit);
    short det_id  = hit.detectorID;
    //short ele_id  = hit.elementID;
    n_hits_all[det_id]++;
    if (hit.isInTime()) n_hits_intime[det_id]++;
  }

  bool triggered[N_TRIG] = { fpga1, nim3 };
  for (int i_pl = 1; i_pl <= N_PL; i_pl++) {
    for (int trig = 0; trig < N_TRIG; trig++) {
      if (triggered[trig]) {
        h1_occ_all   [tgt_pos][trig][i_pl]->Fill(n_hits_all   [i_pl]);
        h1_occ_intime[tgt_pos][trig][i_pl]->Fill(n_hits_intime[i_pl]);
      }
    }
  }
}

void AnaOccupancy::InitStep2()
{
  gSystem->mkdir("result", true);
  
  m_ofs_run_ng.open("result/list_run_ng.txt");
  m_file_out2 = new TFile("result/result.root", "RECREATE");
}

void AnaOccupancy::AnalyzeStep2(const int run_id, TFile* file_in)
{
  h1_count = (TH1*)file_in->Get("h1_count");
  if (!h1_count) {
    m_ofs_run_ng << run_id << "\t2\n";
    return;
  }
  double n_evt = h1_count->GetBinContent(3);
  if (n_evt == 0) {
    m_ofs_run_ng << run_id << "\t3\n";
    return;
  } else if (n_evt < 1000) {
    m_ofs_run_ng << run_id << "\t4\n";
    return;
  }
  
  static bool need_init = true;
  ostringstream oss;
  oss << setfill('0');
  for (int tgt = 1; tgt <= N_TGT; tgt++) {
    for (int trig = 0; trig < N_TRIG; trig++) {
      for (int i_pl = 1; i_pl <= N_PL; i_pl++) {
        oss.str("");
        oss << "h1_occ_all_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
        TH1* h1_occ_all = (TH1*)file_in->Get(oss.str().c_str());
        oss.str("");
        oss << "h1_occ_intime_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
        TH1* h1_occ_intime = (TH1*)file_in->Get(oss.str().c_str());
        if (!h1_occ_all || !h1_occ_intime) continue;
        
        if (need_init) {
          m_file_out2->cd();
          m_h1_occ_all_sum   [tgt][trig][i_pl] = (TH1*)h1_occ_all   ->Clone();
          m_h1_occ_intime_sum[tgt][trig][i_pl] = (TH1*)h1_occ_intime->Clone();
          m_gr_occ_all       [tgt][trig][i_pl] = new TGraphErrors();
          m_gr_occ_intime    [tgt][trig][i_pl] = new TGraphErrors();
          m_gr_occ_rms_all   [tgt][trig][i_pl] = new TGraphErrors();
          m_gr_occ_rms_intime[tgt][trig][i_pl] = new TGraphErrors();
          
          string title = h1_occ_all->GetTitle();
          oss.str("");
          oss << "gr_occ_all_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
          m_gr_occ_all[tgt][trig][i_pl]->SetName(oss.str().c_str());
          oss.str("");
          oss << "gr_occ_intime_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
          m_gr_occ_intime[tgt][trig][i_pl]->SetName(oss.str().c_str());
          
          oss.str("");
          oss << "gr_occ_rms_all_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
          m_gr_occ_rms_all[tgt][trig][i_pl]->SetName(oss.str().c_str());
          oss.str("");
          oss << "gr_occ_rms_intime_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl;
          m_gr_occ_rms_intime[tgt][trig][i_pl]->SetName(oss.str().c_str());
          
          oss.str("");
          oss << title << ";Run ID;Mean occupance";
          m_gr_occ_all       [tgt][trig][i_pl]->SetTitle(oss.str().c_str());
          m_gr_occ_intime    [tgt][trig][i_pl]->SetTitle(oss.str().c_str());
          m_gr_occ_rms_all   [tgt][trig][i_pl]->SetTitle(oss.str().c_str());
          m_gr_occ_rms_intime[tgt][trig][i_pl]->SetTitle(oss.str().c_str());
        } else {
          m_h1_occ_all_sum   [tgt][trig][i_pl]->Add(h1_occ_all);
          m_h1_occ_intime_sum[tgt][trig][i_pl]->Add(h1_occ_intime);
        }
        
        int n_pt = m_gr_occ_all[tgt][trig][i_pl]->GetN();
        m_gr_occ_all       [tgt][trig][i_pl]->SetPoint(n_pt, run_id, h1_occ_all   ->GetMean());
        m_gr_occ_intime    [tgt][trig][i_pl]->SetPoint(n_pt, run_id, h1_occ_intime->GetMean());
        m_gr_occ_rms_all   [tgt][trig][i_pl]->SetPoint(n_pt, run_id, h1_occ_all   ->GetMean());
        m_gr_occ_rms_intime[tgt][trig][i_pl]->SetPoint(n_pt, run_id, h1_occ_intime->GetMean());
        m_gr_occ_all       [tgt][trig][i_pl]->SetPointError(n_pt, 0, h1_occ_all   ->GetMeanError());
        m_gr_occ_intime    [tgt][trig][i_pl]->SetPointError(n_pt, 0, h1_occ_intime->GetMeanError());
        m_gr_occ_rms_all   [tgt][trig][i_pl]->SetPointError(n_pt, 0, h1_occ_all   ->GetRMS());
        m_gr_occ_rms_intime[tgt][trig][i_pl]->SetPointError(n_pt, 0, h1_occ_intime->GetRMS());
      }
    }
  }
  need_init = false;
}

void AnaOccupancy::EndStep2()
{
  m_ofs_run_ng.close();

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  //gStyle->SetOptStat(0000);
  gErrorIgnoreLevel = 1111;
  for (int tgt = 1; tgt <= N_TGT; tgt++) {
    cout << "Target " << tgt << endl;
    for (int trig = 0; trig < N_TRIG; trig++) {
      cout << "  Trigger " << trig << endl;
      for (int i_pl = 1; i_pl <= N_PL; i_pl++) {
        cout << "    Plane " << i_pl << endl;
        TH1* h1_all    = m_h1_occ_all_sum   [tgt][trig][i_pl];
        TH1* h1_intime = m_h1_occ_intime_sum[tgt][trig][i_pl];
        TGraphErrors* gr_all        = m_gr_occ_all       [tgt][trig][i_pl];
        TGraphErrors* gr_intime     = m_gr_occ_intime    [tgt][trig][i_pl];
        TGraphErrors* gr_rms_all    = m_gr_occ_rms_all   [tgt][trig][i_pl];
        TGraphErrors* gr_rms_intime = m_gr_occ_rms_intime[tgt][trig][i_pl];
        if (! h1_all) continue;
        
        h1_all->SetLineColor(kRed);
        h1_intime->Draw();
        h1_all   ->Draw("same");
        ostringstream oss;
        oss << setfill('0')
            << "result/h1_occ_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl << ".png";
        c1->SaveAs(oss.str().c_str());

        double occ_min = h1_all->GetXaxis()->GetXmin();
        double occ_max = h1_all->GetXaxis()->GetXmax();
        gr_all->GetYaxis()->SetRangeUser(occ_min, occ_max);
        gr_all->SetLineColor(kRed);
        gr_all->SetMarkerColor(kRed);
        gr_all       ->SetMarkerStyle(7);
        gr_intime    ->SetMarkerStyle(7);
        //gr_rms_all   ->SetFillStyle(7);
        //gr_rms_intime->SetFillStyle(7);
        gr_rms_all   ->SetFillColorAlpha(kRed  , 0.5);
        gr_rms_intime->SetFillColorAlpha(kBlack, 0.5);
        gr_all       ->Draw("AP");
        gr_intime    ->Draw("P");
        gr_rms_all   ->Draw("3");
        gr_rms_intime->Draw("3");
        oss.str("");
        oss << "result/gr_occ_tgt" << tgt << "_" << NAME_TRIG[trig] << "_pl" << setw(2) << i_pl << ".png";
        c1->SaveAs(oss.str().c_str());
      }
    }
  }

  m_file_out2->Write();
  m_file_out2->Close();
}
