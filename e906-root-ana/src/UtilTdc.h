#ifndef _UTIL_TDC__H_
#define _UTIL_TDC__H_
class SRawEvent;

/// A set of utility functions about the TDC data.
namespace UtilTdc {
  static const int N_DET = 54;

  void FindTaiwanTdcBinning(int& n_bin, double& time_lo, double& time_hi);

  void GetOccupancy(SRawEvent* raw, const bool in_time, int occ_det[N_DET+1]);
  void GetOccupancy(SRawEvent* raw, const bool in_time, int& occ_D0, int& occ_D1, int& occ_D2, int& occ_D3);
}; // namespace UtilTdc

#endif /* _UTIL_TDC__H_ */
