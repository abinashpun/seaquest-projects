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
#include "AnaDimuon.h"
using namespace std;

AnaDimuon::AnaDimuon(const std::string name)
  : AnaBase(name)
  , m_n_evt_all  (0)
  , m_n_evt_spill(0)
  , m_n_evt_fpga1(0)
  , m_n_evt_tgt  (0)
{
  //UseRawEvent(true);
  //UseRecEvent(true);
}

AnaDimuon::~AnaDimuon()
{
  ;
}

void AnaDimuon::Init()
{
  if (Verbosity() > 0) cout << "AnaOccupancy::Init():" << endl;
  AnaBase::Init();
  //GeomSvc* geom = GeomSvc::instance();

  m_tree = new TTree("tree", "Created by AnaDimuon");
  m_tree->Branch("evt"     , &m_evt);
  m_tree->Branch("trk_reco", &m_trk_reco);
  m_tree->Branch("dim_reco", &m_dim_reco);
}

void AnaDimuon::End()
{
  OFS() << "Event counts:\n"
        << "  All    = " << m_n_evt_all << "\n"
        << "  Spill  = " << m_n_evt_spill << "\n"
        << "  FPGA1  = " << m_n_evt_fpga1 << "\n"
        << "  Target = " << m_n_evt_tgt << "\n";
  AnaBase::End();
}

void AnaDimuon::AnalyzeEvent()
{
  m_n_evt_all++;

  ///
  /// Event info
  ///
  SRawEvent* raw = GetRawEvent();
  //SRecEvent* rec = GetRecEvent();
  int run_id   = raw->getRunID();
  int spill_id = raw->getSpillID();
  int event_id = raw->getEventID();
  auto e906sc = E906DataCatalog::instance();
  if (! e906sc->SpillExists(spill_id)) return;
  m_n_evt_spill++;

  if (! raw->isTriggeredBy(SRawEvent::MATRIX1)) return;
  m_n_evt_fpga1++;

  int tgt_pos = raw->getTargetPos();
  if (tgt_pos < 1 || tgt_pos > 3) return;
  m_n_evt_fpga1++;

  //int turn_id = raw->getTurnID();
  //int rf_id   = raw->getRFID();
  //int rfp00   = raw->getIntensity(0);

  ///
  /// Dimuon info
  ///
  // ...to be implemented...

  ///
  /// Fill the output tree
  ///
  m_evt.run_id   = run_id;
  m_evt.spill_id = spill_id;
  m_evt.event_id = event_id;
  m_tree->Fill();
}
