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
