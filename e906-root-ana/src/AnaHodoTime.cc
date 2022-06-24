#include <fstream>
#include <sstream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <GeomSvc.h>
#include <E906DataCatalog.h>
#include "UtilHist.h"
#include "AnaHodoTime.h"
using namespace std;

AnaHodoTime::AnaHodoTime(const std::string name)
  : AnaBase(name)
{
  UseRawEvent(true);
  //UseRecEvent(true);
}

AnaHodoTime::~AnaHodoTime()
{
  ;
}

void AnaHodoTime::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();
  //GeomSvc* geom = GeomSvc::instance();

  m_h1_count = new TH1D("h1_count", "Event counts", 20, 0.5, 20.5);

  const double DT = 4.0/9.0; // 4/9 ns = 1 TDC count
  m_h1_time_h1t     = new TH1D("h1_time_h1t"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h2t     = new TH1D("h1_time_h2t"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h3t     = new TH1D("h1_time_h3t"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h4t     = new TH1D("h1_time_h4t"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h1t_int = new TH1D("h1_time_h1t_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h2t_int = new TH1D("h1_time_h2t_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h3t_int = new TH1D("h1_time_h3t_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h4t_int = new TH1D("h1_time_h4t_int", "", 2000, 1000.5*DT, 3000.5*DT);

  m_h1_time_h1b     = new TH1D("h1_time_h1b"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h2b     = new TH1D("h1_time_h2b"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h3b     = new TH1D("h1_time_h3b"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h4b     = new TH1D("h1_time_h4b"    , "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h1b_int = new TH1D("h1_time_h1b_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h2b_int = new TH1D("h1_time_h2b_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h3b_int = new TH1D("h1_time_h3b_int", "", 2000, 1000.5*DT, 3000.5*DT);
  m_h1_time_h4b_int = new TH1D("h1_time_h4b_int", "", 2000, 1000.5*DT, 3000.5*DT);

  m_h1_time_h1t_tr     = new TH1D("h1_time_h1t_tr"    , "", 1000, 500, 1500);
  m_h1_time_h2t_tr     = new TH1D("h1_time_h2t_tr"    , "", 1000, 500, 1500);
  m_h1_time_h3t_tr     = new TH1D("h1_time_h3t_tr"    , "", 1000, 500, 1500);
  m_h1_time_h4t_tr     = new TH1D("h1_time_h4t_tr"    , "", 1000, 500, 1500);
  m_h1_time_h1t_tr_int = new TH1D("h1_time_h1t_tr_int", "", 1000, 500, 1500);
  m_h1_time_h2t_tr_int = new TH1D("h1_time_h2t_tr_int", "", 1000, 500, 1500);
  m_h1_time_h3t_tr_int = new TH1D("h1_time_h3t_tr_int", "", 1000, 500, 1500);
  m_h1_time_h4t_tr_int = new TH1D("h1_time_h4t_tr_int", "", 1000, 500, 1500);

  m_h1_time_h1b_tr     = new TH1D("h1_time_h1b_tr"    , "", 1000, 500, 1500);
  m_h1_time_h2b_tr     = new TH1D("h1_time_h2b_tr"    , "", 1000, 500, 1500);
  m_h1_time_h3b_tr     = new TH1D("h1_time_h3b_tr"    , "", 1000, 500, 1500);
  m_h1_time_h4b_tr     = new TH1D("h1_time_h4b_tr"    , "", 1000, 500, 1500);
  m_h1_time_h1b_tr_int = new TH1D("h1_time_h1b_tr_int", "", 1000, 500, 1500);
  m_h1_time_h2b_tr_int = new TH1D("h1_time_h2b_tr_int", "", 1000, 500, 1500);
  m_h1_time_h3b_tr_int = new TH1D("h1_time_h3b_tr_int", "", 1000, 500, 1500);
  m_h1_time_h4b_tr_int = new TH1D("h1_time_h4b_tr_int", "", 1000, 500, 1500);
}

void AnaHodoTime::End()
{
  DrawOnePlane("h1t", m_h1_time_h1t, m_h1_time_h1t_int);
  DrawOnePlane("h2t", m_h1_time_h2t, m_h1_time_h2t_int);
  DrawOnePlane("h3t", m_h1_time_h3t, m_h1_time_h3t_int);
  DrawOnePlane("h4t", m_h1_time_h4t, m_h1_time_h4t_int);
  
  DrawOnePlane("h1b", m_h1_time_h1b, m_h1_time_h1b_int);
  DrawOnePlane("h2b", m_h1_time_h2b, m_h1_time_h2b_int);
  DrawOnePlane("h3b", m_h1_time_h3b, m_h1_time_h3b_int);
  DrawOnePlane("h4b", m_h1_time_h4b, m_h1_time_h4b_int);
  
  DrawOnePlane("h1t_tr", m_h1_time_h1t_tr, m_h1_time_h1t_tr_int);
  DrawOnePlane("h2t_tr", m_h1_time_h2t_tr, m_h1_time_h2t_tr_int);
  DrawOnePlane("h3t_tr", m_h1_time_h3t_tr, m_h1_time_h3t_tr_int);
  DrawOnePlane("h4t_tr", m_h1_time_h4t_tr, m_h1_time_h4t_tr_int);
  
  DrawOnePlane("h1b_tr", m_h1_time_h1b_tr, m_h1_time_h1b_tr_int);
  DrawOnePlane("h2b_tr", m_h1_time_h2b_tr, m_h1_time_h2b_tr_int);
  DrawOnePlane("h3b_tr", m_h1_time_h3b_tr, m_h1_time_h3b_tr_int);
  DrawOnePlane("h4b_tr", m_h1_time_h4b_tr, m_h1_time_h4b_tr_int);

  OFS() << "Event counts:\n"
        << "  All        = " << (int)m_h1_count->GetBinContent(1) << "\n"
        << "  Spill OK   = " << (int)m_h1_count->GetBinContent(2) << "\n"
        << "  Trigger OK = " << (int)m_h1_count->GetBinContent(3) << "\n";
  for (int ii = 1; ii <= 7; ii++) {
    OFS() << "  Target #" << ii << " = " << (int)m_h1_count->GetBinContent(4+ii) << "\n";
  }
  AnaBase::End();
}

void AnaHodoTime::AnalyzeEvent()
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
  static int id_h1t = 0;
  static int id_h1b = 0;
  static int id_h2t = 0;
  static int id_h2b = 0;
  static int id_h3t = 0;
  static int id_h3b = 0;
  static int id_h4t = 0;
  static int id_h4b = 0;
  if (id_h1t == 0) {
    GeomSvc* geom = GeomSvc::instance();
    id_h1t = geom->getDetectorID("H1T");
    id_h1b = geom->getDetectorID("H1B");
    id_h2t = geom->getDetectorID("H2T");
    id_h2b = geom->getDetectorID("H2B");
    id_h3t = geom->getDetectorID("H3T");
    id_h3b = geom->getDetectorID("H3B");
    id_h4t = geom->getDetectorID("H4T");
    id_h4b = geom->getDetectorID("H4B");
    cout << "IDs: " << id_h1t << " " << id_h1b << " " << id_h2t << " " << id_h2b << " " << id_h3t << " " << id_h3b << " " << id_h4t << " " << id_h4b << endl;
    if (id_h1t == 0) {
      cout << "!!ERROR!!  Failed at getting det IDs.  Abort." << endl;
      exit(1);
    }
  }

  int n_hit = raw->getNHitsAll();
  for (int i_hit = 0; i_hit < n_hit; i_hit++) {
    Hit hit = raw->getHit(i_hit);
    short det_id     = hit.detectorID;
    //short ele_id     = hit.elementID;
    float tdc_time   = hit.tdcTime;
    //float drift_dist = hit.driftDistance;
    bool  in_time    = hit.isInTime();

    if      (det_id == id_h1t) m_h1_time_h1t->Fill(tdc_time);
    else if (det_id == id_h2t) m_h1_time_h2t->Fill(tdc_time);
    else if (det_id == id_h3t) m_h1_time_h3t->Fill(tdc_time);
    else if (det_id == id_h4t) m_h1_time_h4t->Fill(tdc_time);
    else if (det_id == id_h1b) m_h1_time_h1b->Fill(tdc_time);
    else if (det_id == id_h2b) m_h1_time_h2b->Fill(tdc_time);
    else if (det_id == id_h3b) m_h1_time_h3b->Fill(tdc_time);
    else if (det_id == id_h4b) m_h1_time_h4b->Fill(tdc_time);
    if (in_time) {
      if      (det_id == id_h1t) m_h1_time_h1t_int->Fill(tdc_time);
      else if (det_id == id_h2t) m_h1_time_h2t_int->Fill(tdc_time);
      else if (det_id == id_h3t) m_h1_time_h3t_int->Fill(tdc_time);
      else if (det_id == id_h4t) m_h1_time_h4t_int->Fill(tdc_time);
      else if (det_id == id_h1b) m_h1_time_h1b_int->Fill(tdc_time);
      else if (det_id == id_h2b) m_h1_time_h2b_int->Fill(tdc_time);
      else if (det_id == id_h3b) m_h1_time_h3b_int->Fill(tdc_time);
      else if (det_id == id_h4b) m_h1_time_h4b_int->Fill(tdc_time);
    }
  }

  ///
  /// Trigger hit info
  ///
  int n_tr_hit = raw->getNTriggerHits();
  for (int i_hit = 0; i_hit < n_tr_hit; i_hit++) {
    Hit hit = raw->getTriggerHit(i_hit);
    short det_id     = hit.detectorID;
    //short ele_id     = hit.elementID;
    float tdc_time   = hit.tdcTime;
    bool  in_time    = hit.isInTime();

    if      (det_id == id_h1t) m_h1_time_h1t_tr->Fill(tdc_time);
    else if (det_id == id_h2t) m_h1_time_h2t_tr->Fill(tdc_time);
    else if (det_id == id_h3t) m_h1_time_h3t_tr->Fill(tdc_time);
    else if (det_id == id_h4t) m_h1_time_h4t_tr->Fill(tdc_time);
    else if (det_id == id_h1b) m_h1_time_h1b_tr->Fill(tdc_time);
    else if (det_id == id_h2b) m_h1_time_h2b_tr->Fill(tdc_time);
    else if (det_id == id_h3b) m_h1_time_h3b_tr->Fill(tdc_time);
    else if (det_id == id_h4b) m_h1_time_h4b_tr->Fill(tdc_time);
    if (in_time) {
      if      (det_id == id_h1t) m_h1_time_h1t_tr_int->Fill(tdc_time);
      else if (det_id == id_h2t) m_h1_time_h2t_tr_int->Fill(tdc_time);
      else if (det_id == id_h3t) m_h1_time_h3t_tr_int->Fill(tdc_time);
      else if (det_id == id_h4t) m_h1_time_h4t_tr_int->Fill(tdc_time);
      else if (det_id == id_h1b) m_h1_time_h1b_tr_int->Fill(tdc_time);
      else if (det_id == id_h2b) m_h1_time_h2b_tr_int->Fill(tdc_time);
      else if (det_id == id_h3b) m_h1_time_h3b_tr_int->Fill(tdc_time);
      else if (det_id == id_h4b) m_h1_time_h4b_tr_int->Fill(tdc_time);
    }
  }
}

void AnaHodoTime::DrawOnePlane(const char* name, TH1* h1_all, TH1* h1_int)
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

void AnaHodoTime::InitStep2()
{
  gSystem->mkdir("result", true);
  m_ofs_run_ng.open("result/list_run_ng.txt");
  m_ofs_det_ng.open("result/list_det_ng.txt");
  m_file_out2 = new TFile("result/result.root", "RECREATE");
}

void AnaHodoTime::AnalyzeStep2(const int run_id, TFile* file_in)
{
  m_h1_count = (TH1*)file_in->Get("h1_count");
  if (!m_h1_count) {
    m_ofs_run_ng << run_id << "\t2\n";
    return;
  }
  double n_evt = m_h1_count->GetBinContent(3);
  if (n_evt == 0) {
    m_ofs_run_ng << run_id << "\t3\n";
    return;
  } else if (n_evt < 1000) {
    m_ofs_run_ng << run_id << "\t4\n";
    return;
  }
  
  AnaOneHodoHist(run_id, file_in, "h1t"   , m_gr_h1t   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h2t"   , m_gr_h2t   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h3t"   , m_gr_h3t   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h4t"   , m_gr_h4t   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h1t_tr", m_gr_h1t_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h2t_tr", m_gr_h2t_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h3t_tr", m_gr_h3t_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h4t_tr", m_gr_h4t_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h1b"   , m_gr_h1b   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h2b"   , m_gr_h2b   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h3b"   , m_gr_h3b   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h4b"   , m_gr_h4b   , m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h1b_tr", m_gr_h1b_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h2b_tr", m_gr_h2b_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h3b_tr", m_gr_h3b_tr, m_ofs_det_ng);
  AnaOneHodoHist(run_id, file_in, "h4b_tr", m_gr_h4b_tr, m_ofs_det_ng);
}

void AnaHodoTime::EndStep2()
{
  m_ofs_run_ng.close();
  m_ofs_det_ng.close();

  m_file_out2->cd();
  DrawOneHodoGraph("h1t"   , m_gr_h1t   );
  DrawOneHodoGraph("h2t"   , m_gr_h2t   );
  DrawOneHodoGraph("h3t"   , m_gr_h3t   );
  DrawOneHodoGraph("h4t"   , m_gr_h4t   );
  DrawOneHodoGraph("h1t_tr", m_gr_h1t_tr);
  DrawOneHodoGraph("h2t_tr", m_gr_h2t_tr);
  DrawOneHodoGraph("h3t_tr", m_gr_h3t_tr);
  DrawOneHodoGraph("h4t_tr", m_gr_h4t_tr);
  DrawOneHodoGraph("h1b"   , m_gr_h1b   );
  DrawOneHodoGraph("h2b"   , m_gr_h2b   );
  DrawOneHodoGraph("h3b"   , m_gr_h3b   );
  DrawOneHodoGraph("h4b"   , m_gr_h4b   );
  DrawOneHodoGraph("h1b_tr", m_gr_h1b_tr);
  DrawOneHodoGraph("h2b_tr", m_gr_h2b_tr);
  DrawOneHodoGraph("h3b_tr", m_gr_h3b_tr);
  DrawOneHodoGraph("h4b_tr", m_gr_h4b_tr);

  m_file_out2->Write();
  m_file_out2->Close();
}

void AnaHodoTime::AnaOneHodoHist(const int run_id, TFile* file, const char* name, TGraph (&gr)[5], std::ostream& os_det_ng)
{
  ostringstream oss;
  oss << "h1_time_" << name;
  TH1* h1_all = (TH1*)file->Get(oss.str().c_str());
  oss << "_int";
  TH1* h1_int = (TH1*)file->Get(oss.str().c_str());
  if (! h1_all || ! h1_int) {
    cout << "ERROR:  Cannot find h1_all/h1_int for " << name << ".  Abort." << endl;
    exit(1);
  }

  double time_max_all = 0;
  double time_max     = 0;
  double time_lo      = 0;
  double time_hi      = 0;
  if (h1_all->Integral() < 100) {
    gr[0].SetPoint(gr[0].GetN(), run_id, 1); // Few all-time hits
  } else if (h1_int->Integral() < 100) {
    gr[0].SetPoint(gr[0].GetN(), run_id, 2); // Few in-time hits
  } else {
    int bin_max_all = h1_all->GetMaximumBin();
    int bin_max     = h1_int->GetMaximumBin();
    int bin_lo, bin_hi;
    UtilHist::FindFilledRange(h1_int, bin_lo, bin_hi);
    time_max_all = h1_all->GetBinCenter(bin_max_all);
    time_max     = h1_int->GetBinCenter(bin_max);
    time_lo      = h1_int->GetBinLowEdge(bin_lo);
    time_hi      = h1_int->GetBinLowEdge(bin_hi+1);

    int status = 0; // OK by default
    if (fabs(time_max_all - time_max) > 0.1) status = 3;
    else if (fabs(time_max - (time_lo + time_hi)/2) > 5) status = 4;
    else if (fabs(time_max - (time_lo + time_hi)/2) > 1) status = 5;
    if (status != 0) os_det_ng << run_id << "\t" << name << "\t" << status << "\n";

    gr[0].SetPoint(gr[0].GetN(), run_id, status);
    gr[1].SetPoint(gr[1].GetN(), run_id, time_max_all - time_max);
    gr[2].SetPoint(gr[2].GetN(), run_id, time_max);
    gr[3].SetPoint(gr[3].GetN(), run_id, time_lo );
    gr[4].SetPoint(gr[4].GetN(), run_id, time_hi );
  }
}

void AnaHodoTime::DrawOneHodoGraph(const char* name, TGraph (&gr)[5])
{
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  ostringstream oss;

  const char* gr_name[5] = { "status", "diff", "peak", "low", "high" };
  for (int ii = 0; ii < 5; ii++) {
    oss.str("");
    oss << "gr_" << gr_name[ii] << "_" << name;
    gr[ii].SetName(oss.str().c_str());
    oss.str("");
    oss << name << ";Run ID;" << gr_name[ii];
    gr[ii].SetTitle(oss.str().c_str());
    gr[ii].SetMarkerStyle(7);
  }

  gr[0].SetMarkerColor(kRed);
  gr[0].Draw("AP");
  oss.str("");
  oss << "result/gr_status_" << name << ".png";
  c1->SaveAs(oss.str().c_str());

  gr[1].SetMarkerColor(kRed);
  gr[1].Draw("AP");
  oss.str("");
  oss << "result/gr_diff_" << name << ".png";
  c1->SaveAs(oss.str().c_str());

  gr[2].SetMarkerColor(kRed);
  gr[3].SetMarkerColor(kGreen);
  gr[4].SetMarkerColor(kBlue);

  TMultiGraph mg;
  oss.str("");
  oss << name << ";Run ID;TDC time (ns)";
  mg.SetTitle(oss.str().c_str());
  mg.Add(&gr[2],"P");
  mg.Add(&gr[3],"P");
  mg.Add(&gr[4],"P");
  mg.Draw("A");

  oss.str("");
  oss << "result/gr_peak_" << name << ".png";
  c1->SaveAs(oss.str().c_str());

  for (int ii = 0; ii < 5; ii++) gr[ii].Write();

  delete c1;
}
