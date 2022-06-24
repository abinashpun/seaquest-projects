#include "TreeData.h"
using namespace std;

EventData::EventData()
  : run_id(0)
  , spill_id(0)
  , event_id(0)
  , fpga1(false)
  , fpga2(false)
  , fpga3(false)
  , nim3 (false)
  , rec_status(0)
{
  ;
}

TrackData::TrackData() 
  : charge(0)
{
  ;
}
  
DimuonData::DimuonData()
  : pT(0)
  , x1(0)
  , x2(0)
  , xF(0)
  , costh(0)
  , phi(0)
{
  ;
}

void OccData::Clear()
{
  run_id = spill_id = event_id = target_pos = 0;
  fpga1 = fpga2 = fpga3 = nim3 = 0;
  turn_id = rf_id = rfp00 = 0;
  memset(occ_all   , 0, sizeof(occ_all   ));
  memset(occ_intime, 0, sizeof(occ_intime));
}

