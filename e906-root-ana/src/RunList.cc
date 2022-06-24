#include <sstream>
#include <TSystem.h>
#include <TFile.h>
#include <E906DataCatalog.h>
#include "RunList.h"
using namespace std;

RunList::RunList(const std::string ds)
  : m_env ("gpvm")
  , m_ds  (ds)
  , m_verb(0)
  , m_file(0)
{
  auto dc = E906DataCatalog::instance();
  dc->ListRun(ds, &m_list_run);
  if (m_list_run.size() == 0) {
    cout << "RunList finds no run for dataset '" << ds << "'.  Abort." << endl;
    exit(1);
  }
  dc->PreSetDataset(ds);
  //dc->Verbosity(2);
}

RunList::~RunList()
{
  if (m_file) delete m_file;
}

int RunList::OpenRunFile(const std::string dir_base, const int index, int& run_id, TFile*& file)
{
  run_id = m_list_run[index];
  if (Verbosity() > 0) cout << "  Run " << run_id << " (" << index+1 << "/" << m_list_run.size() << ")" << endl;

  auto dc = E906DataCatalog::instance();
  dc->PreSetRun(run_id);
  string run6d = dc->GetRun6d(run_id);

  ostringstream oss;
  oss << dir_base << "/" << run6d.substr(0, 2) << "/" << run6d.substr(2, 2) << "/" << run6d.substr(4, 2);
  if (m_env == "gpvm") oss << "/out";
  oss << "/output.root";

  string fname = oss.str();
  if (gSystem->AccessPathName(fname.c_str()) ) { // Not found
    if (Verbosity() > 1) cout << "    No output file.  Skip." << endl;
    return 1;
  }

  if (m_file) delete m_file;
  m_file = file = new TFile(fname.c_str());
  if (! m_file->IsOpen()) {
    if (Verbosity() > 1) cout << "    Cannot open '" << fname << "'.  Skip." << endl;
    return 2;
  }
  return 0;
}
