#include <fstream>
#include <sstream>
#include <iomanip>
#include <TSystem.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <GeomSvc.h>
#include <E906DataCatalog.h>
#include "AnaHitTree.h"
using namespace std;

AnaHitTree::AnaHitTree(const std::string name)
  : AnaBase(name)
{
  UseRawEvent(true);
  //UseRecEvent(true);
}

AnaHitTree::~AnaHitTree()
{
  ;
}

void AnaHitTree::Init()
{
  if (Verbosity() > 0) cout << "AnaHitTree::Init():" << endl;
  AnaBase::Init();

  h1_count = new TH1D("h1_count", "Event counts", 10, 0.5, 10.5);
  //SetUpTree(m_tree_fpga1, "fpga1");
  SetUpTree(m_tree_nim3,  "nim3" );
}

void AnaHitTree::SetUpTree(TTree*& tree, const std::string label)
{
  ostringstream oss;
  oss << "tree_" << label;
  tree = new TTree(oss.str().c_str(), "Created by AnaHitTree");

  tree->Branch("run_id"      , &m_run_id      ,       "run_id/I");
  tree->Branch("spill_id"    , &m_spill_id    ,     "spill_id/I");
  tree->Branch("event_id"    , &m_event_id    ,     "event_id/I");
  tree->Branch("target_pos"  , &m_target_pos  ,   "target_pos/I");
  tree->Branch("rf_intensity", &m_rf_intensity, "rf_intensity/I");
  tree->Branch("n_hits"      , &m_n_hits      ,             "n_hits/S");
  tree->Branch("det_id"      , &m_det_id      ,     "det_id[n_hits]/S");
  tree->Branch("ele_id"      , &m_ele_id      ,     "ele_id[n_hits]/S");
  tree->Branch("drift_dist"  , &m_drift_dist  , "drift_dist[n_hits]/F");
}

void AnaHitTree::End()
{
  OFS() << "Event counts:\n"
        << "  All       = " << (int)h1_count->GetBinContent(1) << "\n"
        << "  Spill OK  = " << (int)h1_count->GetBinContent(2) << "\n"
        << "  Target OK = " << (int)h1_count->GetBinContent(3) << "\n"
        << "  FPGA1     = " << (int)h1_count->GetBinContent(4) << "\n"
        << "  NIM3      = " << (int)h1_count->GetBinContent(5) << "\n";
  AnaBase::End();
}

void AnaHitTree::AnalyzeEvent()
{
  h1_count->Fill(1); // All events

  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  m_run_id   = raw->getRunID();
  m_spill_id = raw->getSpillID();
  m_event_id = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
  if (! e906sc->SpillExists(m_spill_id)) return;
  h1_count->Fill(2); // Spill OK

  m_target_pos = raw->getTargetPos();
  //if (m_target_pos != 1 && m_target_pos != 3) return;
  h1_count->Fill(3); // Target OK

  bool fpga1 = raw->isTriggeredBy(SRawEvent::MATRIX1);
  bool nim3  = raw->isTriggeredBy(SRawEvent::NIM3);
  if (!nim3) return; // if (!fpga1 && !nim3) return;
  if (fpga1) h1_count->Fill(4);
  if (nim3 ) h1_count->Fill(5);

  //int turn_id = raw->getTurnID();
  //int rf_id   = raw->getRFID();
  //int rfp00   = raw->getIntensity(0);
  m_rf_intensity = raw->getIntensity(0);

  int n_hit = raw->getNHitsAll();
  if (n_hit >= 10000) { 
    cout << "!!WARNING!!  N of hits = " << n_hit << " at run " << m_run_id << ", event " << m_event_id << endl;
    return;
  }

  GeomSvc* geom = GeomSvc::instance();

  m_n_hits = 0;
  for (int i_hit = 0; i_hit < n_hit; i_hit++) {
    Hit hit = raw->getHit(i_hit);
    short det_id = hit.detectorID;
    short ele_id = hit.elementID;
    if (det_id == 0 || det_id > N_DET) continue; // chamber only
    if (7 <= det_id && det_id <= 12) continue; // exclude D1
    if (! hit.isInTime()) continue; // in-time hit only
    m_det_id    [m_n_hits] = det_id;
    m_ele_id    [m_n_hits] = ele_id;
    m_drift_dist[m_n_hits] = geom->getDriftDistance(det_id, hit.tdcTime);
    m_n_hits++;
  }

  //if (fpga1) m_tree_fpga1->Fill();
  if (nim3 ) m_tree_nim3 ->Fill();
}

void AnaHitTree::InitStep2()
{
  gSystem->mkdir("result", true);
  m_file_out2  = new TFile("result/result.root", "RECREATE");
  m_h2_det_ele = new TH2D("h2_det_ele", ";Element ID;Detector ID", N_ELE, 0.5, N_ELE+0.5, N_DET, 0.5, N_DET+0.5);
  m_h2_det_dd  = new TH2D("h2_det_dd" , ";Drift dist (cm);Detector ID",    100, 0.0, 1.2, N_DET, 0.5, N_DET+0.5);
}

void AnaHitTree::AnalyzeStep2(const int run_id, TFile* file_in)
{
  TTree* tree = (TTree*)file_in->Get("tree_nim3"); // or "tree_fpga1"
  if (! tree) return;
  tree->SetBranchAddress("n_hits"    , &m_n_hits);
  tree->SetBranchAddress("n_det_id"  ,  m_det_id);
  tree->SetBranchAddress("n_ele_id"  ,  m_ele_id);
  tree->SetBranchAddress("drift_dist",  m_drift_dist);

  int n_evt = tree->GetEntries();
  if (n_evt > 1000) n_evt = 1000; // Only a part of events is enough
  for (int i_evt = 0; i_evt < n_evt; i_evt++) {
    tree->GetEntry(i_evt);
    for (int i_hit = 0; i_hit < m_n_hits; i_hit++) {
      m_h2_det_ele->Fill(m_ele_id    [i_hit], m_det_id[i_hit]);
      m_h2_det_dd ->Fill(m_drift_dist[i_hit], m_det_id[i_hit]);
    }
  }
}

void AnaHitTree::EndStep2()
{
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  gStyle->SetOptStat(0000);
  gErrorIgnoreLevel = 1111;

  m_h2_det_ele->Draw("colz");
  c1->SaveAs("result/h2_det_ele.png");

  m_h2_det_dd ->Draw("colz");
  c1->SaveAs("result/h2_det_dd.png");
  
  m_file_out2->Write();
  m_file_out2->Close();

  delete c1;
}
