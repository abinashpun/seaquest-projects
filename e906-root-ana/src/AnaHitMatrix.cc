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
#include "AnaHitMatrix.h"
using namespace std;

AnaHitMatrix::AnaHitMatrix(const std::string name)
  : AnaBase(name)
{
  UseRawEvent(true);
  //UseRecEvent(true);
}

AnaHitMatrix::~AnaHitMatrix()
{
  ;
}

void AnaHitMatrix::Init()
{
  if (Verbosity() > 0) cout << "AnaHitMatrix::Init():" << endl;
  AnaBase::Init();

  //GeomSvc* geom = GeomSvc::instance();
  //cout << "Check the number of elements/plane:" << endl;
  //for (int i_pl = 1; i_pl <= N_DET; i_pl++) {
  //  string name = geom->getDetectorName  (i_pl);
  //  int   n_ele = geom->getPlaneNElements(i_pl);
  //  cout << "  " << setw(2) << i_pl << " " << setw(5) << name << " : " << n_ele << endl;
  //}

  h1_count = new TH1D("h1_count", "Event counts", 10, 0.5, 10.5);
  //SetUpTree(m_tree_fpga1, "fpga1");
  SetUpTree(m_tree_nim3,  "nim3" );
}

void AnaHitMatrix::SetUpTree(TTree*& tree, const std::string label)
{
  ostringstream oss;
  oss << "tree_" << label;
  tree = new TTree(oss.str().c_str(), "Created by AnaHitMatrix");

  tree->Branch("run_id"      , &m_run_id      ,       "run_id/I");
  tree->Branch("spill_id"    , &m_spill_id    ,     "spill_id/I");
  tree->Branch("event_id"    , &m_event_id    ,     "event_id/I");
  tree->Branch("target_pos"  , &m_target_pos  ,   "target_pos/I");
  tree->Branch("rf_intensity", &m_rf_intensity, "rf_intensity/I");

  oss.str("");
  oss << "hit_matrix[" << N_DET << "][" << N_ELE << "]/O";
  tree->Branch("hit_matrix", &m_hit_matrix, oss.str().c_str());
}

void AnaHitMatrix::End()
{
  OFS() << "Event counts:\n"
        << "  All       = " << (int)h1_count->GetBinContent(1) << "\n"
        << "  Spill OK  = " << (int)h1_count->GetBinContent(2) << "\n"
        << "  Target OK = " << (int)h1_count->GetBinContent(3) << "\n"
        << "  FPGA1     = " << (int)h1_count->GetBinContent(4) << "\n"
        << "  NIM3      = " << (int)h1_count->GetBinContent(5) << "\n";
  AnaBase::End();
}

void AnaHitMatrix::AnalyzeEvent()
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

  memset(m_hit_matrix, 0, sizeof(m_hit_matrix));

  for (int i_hit = 0; i_hit < raw->getNHitsAll(); i_hit++) {
    Hit hit = raw->getHit(i_hit);
    short det_id = hit.detectorID;
    short ele_id = hit.elementID;
    if (det_id == 0 || det_id > N_DET) continue; // chamber only
    if (7 <= det_id && det_id <= 12) continue; // exclude D1
    if (! hit.isInTime()) continue; // in-time hit only
    m_hit_matrix[det_id-1][ele_id-1] = true;
  }

  //if (fpga1) m_tree_fpga1->Fill();
  if (nim3 ) m_tree_nim3 ->Fill();
}

void AnaHitMatrix::InitStep2()
{
  gSystem->mkdir("result", true);
  gStyle->SetOptStat(0000);
}

void AnaHitMatrix::AnalyzeStep2(const int run_id, TFile* file_in)
{
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  TH2* h2_hit_mat = new TH2D("h2_hit_mat", ";Detector ID - 1;Element ID - 1;Has hit?", N_DET, -0.5, N_DET-0.5, N_ELE, -0.5, N_ELE-0.5);

  h1_count = (TH1*)file_in->Get("h1_count");
  TTree* tree = (TTree*)file_in->Get("tree_nim3"); // or "tree_fpga1"
  tree->SetBranchAddress("target_pos"  , &m_target_pos);
  tree->SetBranchAddress("rf_intensity", &m_rf_intensity);
  tree->SetBranchAddress("hit_matrix"  ,  m_hit_matrix);
  
  for (int i_evt = 0; i_evt < tree->GetEntries(); i_evt++) {
    if (i_evt >= 10) break;
    tree->GetEntry(i_evt);
    
    h2_hit_mat->Reset();
    for (int i_det = 0; i_det < 30; i_det++) {
      for (int i_ele = 0; i_ele < 201; i_ele++) {
        h2_hit_mat->Fill(i_det, i_ele, m_hit_matrix[i_det][i_ele]);
      }
    }
    
    ostringstream oss;
    oss << "Target Pos = " << m_target_pos << ", RFp00 = " << m_rf_intensity;
    h2_hit_mat->SetTitle(oss.str().c_str());
    h2_hit_mat->Draw("colz");
    
    oss.str("");
    oss << "result/h2_hit_mat_run" << run_id << "_evt" << i_evt << ".png";
    c1->SaveAs(oss.str().c_str());
  }

  delete h2_hit_mat;
  delete c1;
}

void AnaHitMatrix::EndStep2()
{
  ;
}
