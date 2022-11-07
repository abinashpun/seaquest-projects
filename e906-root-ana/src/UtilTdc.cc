#include <SRawEvent.h>
#include "UtilTdc.h"
using namespace std;

/// Find a proper set of "n_bin", "time_lo" and "time_hi".
/**
 * One TDC count of the Taiwan TDC corresponds to 4/9 nsec.
 * Therefore the bin width has to be "i * 4/9" with "i" integer.
 */
void UtilTdc::FindTaiwanTdcBinning(int& n_bin, double& time_lo, double& time_hi)
{
  const double DT = 4.0/9.0; // ns
  if (n_bin <= 0) return;
  if (time_hi == 0) {
    time_hi = time_lo + n_bin * DT;
  } else {
    int n_dt = 1;
    while (n_dt * DT * n_bin < time_hi - time_lo) n_dt++;
    n_bin      = (int)((time_hi - time_lo) / (n_dt * DT));
    int bin_ce = (int)((time_hi + time_lo) / 2 / DT);
    time_lo = DT * (bin_ce - n_bin / 2.0);
    time_hi = DT * (bin_ce + n_bin / 2.0);
  }
}

/// Get the occupancy of each detector (plane).
/**
 * Usage: 
 * @code
 * int occ_det[UtilTdc::N_DET+1];
 * UtilTdc::GetOccupancy(raw, true, occ_det);
 * @endcode
 */
void UtilTdc::GetOccupancy(SRawEvent* raw, const bool in_time, int occ_det[N_DET+1])
{
  memset(occ_det, 0, sizeof(int[N_DET+1]));
  for (int i_hit = 0; i_hit < raw->getNHitsAll(); i_hit++) {
    Hit hit = raw->getHit(i_hit);
    if (in_time && (!hit.isInTime())) continue;
    occ_det[hit.detectorID]++;
  }
}

/// Get the occupancy of each station.
/**
 * Usage: 
 * @code
 * int occ_D0, occ_D1, occ_D2, occ_D3;
 * UtilTdc::GetOccupancy(raw, true, occ_D0, occ_D1, occ_D2, occ_D3);
 * @endcode
 */
void UtilTdc::GetOccupancy(SRawEvent* raw, const bool in_time, int& occ_D0, int& occ_D1, int& occ_D2, int& occ_D3)
{
  int occ_det[N_DET+1];
  GetOccupancy(raw, in_time, occ_det);

  occ_D0 = occ_D1 = occ_D2 = occ_D3 = 0;
  for (int i =  1; i <=  6; i++) occ_D0 += occ_det[i];
  for (int i =  7; i <= 12; i++) occ_D1 += occ_det[i];
  for (int i = 13; i <= 18; i++) occ_D2 += occ_det[i];
  for (int i = 19; i <= 30; i++) occ_D3 += occ_det[i];
}
