#include <fstream>
#include <sstream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <GeomSvc.h>
#include <E906DataCatalog.h>
#include "UtilHist.h"
#include "AnaChamHit.h"
using namespace std;

AnaChamHit::AnaChamHit(const std::string name)
  : AnaBase(name)
{
  ;
}

AnaChamHit::~AnaChamHit()
{
  ;
}

void AnaChamHit::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();

  m_h1_count = new TH1D("h1_count", "Event counts", 20, 0.5, 20.5);

  ostringstream oss;
  oss << setfill('0');

  GeomSvc* geom = GeomSvc::instance();
  for (int det_id = DET_ID_LO; det_id <= DET_ID_HI; det_id++) {
    string det_name = geom->getDetectorName(det_id);
    int n_ele = geom->getPlaneNElements(det_id);

    const double DT = 4.0/9.0; // 4/9 ns = 1 TDC count
    int time_n;
    double time_lo;
    double time_hi;
    if (det_name.substr(0, 2) == "D0" ||
        det_name.substr(0, 2) == "D1"   ) { // 1700-1800 ns
      time_n  = 1000;
      time_lo = 3400.5*DT; 
      time_hi = 4400.5*DT;
    } else if (det_name.substr(0, 2) == "D2") { // 1450-1710 ns
      time_n  = 1400;
      time_lo = 2800.5*DT; 
      time_hi = 4200.5*DT;
    } else if (det_name.substr(0, 3) == "D3p") { // 1360-1580 ns
      time_n  = 1200;
      time_lo = 2700.5*DT; 
      time_hi = 3900.5*DT;
    } else { // D3m, 1490-1700 ns
      time_n  = 1200;
      time_lo = 3000.5*DT; 
      time_hi = 4200.5*DT;
    }

    oss.str("");    
    oss << "h2_time_" << setw(2) << det_id << "_" << det_name;
    m_h2_time[det_id] = new TH2D(oss.str().c_str(), "", time_n, time_lo, time_hi,  n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << det_name << ";TDC time (ns);Element ID";
    m_h2_time[det_id]->SetTitle(oss.str().c_str());

    oss.str("");    
    oss << "h2_time_int_" << setw(2) << det_id << "_" << det_name;
    m_h2_time_int[det_id] = new TH2D(oss.str().c_str(), "", time_n, time_lo, time_hi,  n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << det_name << ";TDC time (ns);Element ID";
    m_h2_time_int[det_id]->SetTitle(oss.str().c_str());
  }
}

void AnaChamHit::End()
{
  ostringstream oss;
  oss << setfill('0');
  for (int det_id = DET_ID_LO; det_id <= DET_ID_HI; det_id++) {
    oss.str("");
    oss << setw(2) << det_id << "_" << m_h2_time[det_id]->GetTitle();
    TH1* h1_all = m_h2_time    [det_id]->ProjectionX("h1_all");
    TH1* h1_int = m_h2_time_int[det_id]->ProjectionX("h1_int");
    DrawOnePlane(oss.str().c_str(), h1_all, h1_int);
    delete h1_all;
    delete h1_int;
  }
  
  OFS() << "Event counts:\n"
        << "  All        = " << (int)m_h1_count->GetBinContent(1) << "\n"
        << "  Spill OK   = " << (int)m_h1_count->GetBinContent(2) << "\n"
        << "  Trigger OK = " << (int)m_h1_count->GetBinContent(3) << "\n";
  for (int ii = 1; ii <= 7; ii++) {
    OFS() << "  Target #" << ii << " = " << (int)m_h1_count->GetBinContent(4+ii) << "\n";
  }
  AnaBase::End();
}

void AnaChamHit::AnalyzeEvent()
{
  m_h1_count->Fill(1); // All events

  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  //int run_id   = raw->getRunID();
  int spill_id = raw->getSpillID();
  //int event_id = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
  if (! e906sc->SpillExists(spill_id)) return;
  m_h1_count->Fill(2); // Spill OK

  bool trig_fpga1 = raw->isTriggeredBy(SRawEvent::MATRIX1);
  //bool trig_nim3  = raw->isTriggeredBy(SRawEvent::NIM3);
  if (! trig_fpga1) return;
  m_h1_count->Fill(3); // Trigger OK

  int tgt_pos = raw->getTargetPos();
  m_h1_count->Fill(4 + tgt_pos); // Target counts

  ///
  /// Hit info
  ///
  int n_hit = raw->getNHitsAll();
  for (int i_hit = 0; i_hit < n_hit; i_hit++) {
    Hit hit = raw->getHit(i_hit);
    short det_id = hit.detectorID;
    if (det_id < DET_ID_LO || det_id > DET_ID_HI) continue;

    short ele_id     = hit.elementID;
    float tdc_time   = hit.tdcTime;
    //float drift_dist = hit.driftDistance;
    bool  in_time    = hit.isInTime();

    m_h2_time[det_id]->Fill(tdc_time, ele_id);
    if (in_time) {
      m_h2_time_int[det_id]->Fill(tdc_time, ele_id);
    }
  }
}

void AnaChamHit::DrawOnePlane(const char* name, TH1* h1_all, TH1* h1_int)
{
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  UtilHist::AutoSetRange(h1_all);
  UtilHist::AutoSetRange(h1_int);
  h1_all->SetLineWidth(2);
  h1_int->SetLineColor(kRed);
  h1_all->Draw();
  h1_int->Draw("same");

  ostringstream oss;
  oss << GetOutputDir() << "/h1_time_" << name << ".png";
  c1->SaveAs(oss.str().c_str());
  delete c1;
}

void AnaChamHit::InitStep2()
{
  ;
}

void AnaChamHit::AnalyzeStep2(const int run_id, TFile* file_in)
{
  ;
}

void AnaChamHit::EndStep2()
{
  ;
}
