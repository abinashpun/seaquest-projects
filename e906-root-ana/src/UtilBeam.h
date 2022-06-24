#ifndef _UTIL_BEAM__H_
#define _UTIL_BEAM__H_

/// A set of utility functions about the beam info.
namespace UtilBeam {
  double PoTRaw (const unsigned int spill_id);
  double PoTLive(const unsigned int spill_id);
  double PoTPerQIE(const unsigned int spill_id);
  double QIE2Proton(const int qie_count, const unsigned int spill_id);

  void ListOfRfValues(int& n_value, int*& list_values);
  void ListOfRfValues(int& n_value, double*& list_values);
}; // namespace UtilBeam

#endif /* _UTIL_BEAM__H_ */
