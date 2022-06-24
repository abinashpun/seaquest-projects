#include <fstream>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <JobOptsSvc.h>
#include <GeomSvc.h>
#include <SRawEvent.h>
#include <SRecEvent.h>
#include <E906DataCatalog.h>
#include "AnaBase.h"
using namespace std;

AnaBase::AnaBase(const std::string name)
  : m_name(name)
  , m_dir_out(".")
  , m_ds("")
  , m_run_id(0)
  , m_use_raw(false)
  , m_use_rec(false)
  , m_n_evt_max(0)
  , m_verb(0)
  , m_raw(0)
  , m_rec(0)
{
  ;
}

AnaBase::~AnaBase()
{
  if (m_raw) delete m_raw;
  if (m_rec) delete m_rec;
}

/// Function to initialize all variables for the 1st analysis step.
/**
 * Init() of each analysis module should call this function and then initialize its own variables.
 */
void AnaBase::Init()
{
  if (m_dir_out != ".") gSystem->mkdir(m_dir_out.c_str(), true);

  E906DataCatalog* e906dc = E906DataCatalog::instance();
  //e906dc->Verbosity(2);
  if (m_ds     != "") e906dc->PreSetDataset(m_ds);
  if (m_run_id !=  0) e906dc->PreSetRun(m_run_id);

  m_ofs.open((m_dir_out + "/output.txt").c_str());
  m_file_out = new TFile((m_dir_out + "/output.root").c_str(), "RECREATE");
  if (! gSystem->AccessPathName("opts/dataset_6.opts")) { // found
    JobOptsSvc::instance()->init("opts/dataset_6.opts");
  } else {
    JobOptsSvc::instance()->init("$KTRACKER_ROOT/opts/run6.opts");
  }
  GeomSvc::instance()->init();
}

/// Function to finalize all variables for the 1st analysis step.  Should be overloaded by each analysis module.
/**
 * End() of each analysis module should finalize its own variables and then call this function.
 */
void AnaBase::End()
{
  m_ofs.close();

  m_file_out->cd();
  m_file_out->Write();
  m_file_out->Close();
}

/// Function to process one run in the 1st analysis step.
/**
 * Need not be overloaded by each analysis module.
 */
void AnaBase::Analyze(const std::string fn_raw, const std::string fn_rec)
{
  TFile* file_raw;
  TTree* tree_raw;
  unsigned int n_evt_raw = 0;
  if (m_use_raw) {
    file_raw = new TFile(fn_raw.c_str());
    tree_raw = (TTree*)file_raw->Get("save");
    if (! tree_raw) {
      cout << "Cannot find the raw data tree.  Abort." << endl;
      exit(1);
    }
    if (! m_raw) m_raw = new SRawEvent();
    tree_raw->SetBranchAddress("rawEvent", &m_raw);
    n_evt_raw = tree_raw->GetEntries();
  }

  TFile* file_rec;
  TTree* tree_rec;
  unsigned int n_evt_rec = 0;
  if (m_use_rec) {
    file_rec = new TFile(fn_rec.c_str());
    tree_rec = (TTree*)file_rec->Get("save");
    if (! tree_rec) {
      cout << "Cannot find the rec data tree.  Abort." << endl;
      exit(1);
    }
    if (! m_rec) m_rec = new SRecEvent();
    tree_rec->SetBranchAddress("recEvent", &m_rec);
    n_evt_rec = tree_rec->GetEntries();
  }

  unsigned int n_evt_all = 0;
  if (n_evt_raw > 0) {
    n_evt_all = n_evt_raw;
    if (n_evt_rec > 0 && n_evt_rec != n_evt_raw) {
      cout << "Mismatch between n_evt_raw (" << n_evt_raw << ") and n_evt_rec (" << n_evt_rec << ").  Abort." << endl;
      exit(1);
    }
  } else {
    n_evt_all = n_evt_rec;
  }
  int n_evt = m_n_evt_max > 0 && n_evt_all > m_n_evt_max  ?  m_n_evt_max  :  n_evt_all;
  cout << "N of events:  all = " << n_evt_all << ", to be analyzed = " << n_evt << endl;
  for (int i_evt = 0; i_evt < n_evt; i_evt++) {
    if      ((i_evt+1) % 10000 == 0) cout << i_evt+1 << endl;
    else if ((i_evt+1) %  1000 == 0) cout << " . " << flush;
    if (m_use_raw) tree_raw->GetEntry(i_evt);
    if (m_use_rec) tree_rec->GetEntry(i_evt);
    AnalyzeEvent();
  }
  cout << endl;
  if (m_use_raw) file_raw->Close();
  if (m_use_rec) file_rec->Close();
}

/// Function to initialize all variables for the 2nd analysis step.  Must be overloaded by each analysis module.
void AnaBase::InitStep2()
{
  ;
}

/// Function to process one run in the 2nd analysis step.  Must be overloaded by each analysis module.
void AnaBase::AnalyzeStep2(const int run_id, TFile* file_in)
{
  ;
}

/// Function to finalize all variables for the 2nd analysis step.  Must be overloaded by each analysis module.
void AnaBase::EndStep2()
{
  ;
}

/// Function to process one event in the 1st analysis step.  Must be overloaded by each analysis module.
void AnaBase::AnalyzeEvent()
{
  ;
}
