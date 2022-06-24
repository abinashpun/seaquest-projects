#ifndef _UTIL_HIST__H_
#define _UTIL_HIST__H_
class TH1;
class TH2;
class TH3;

/// A set of utility functions about the handling of histograms.
namespace UtilHist {
  /// Find the lowest and highest bins ("bin_lo" and "bin_hi") out of non-empty bins of "h1".
  bool FindFilledRange(TH1* h1, int& bin_lo, int& bin_hi);

  /// Adjust the axis range via "h1->GetXaxis()->SetRange(bin_lo, bin_hi)" to zoom up non-empty bins.
  bool AutoSetRange (TH1* h1, const int margin_lo=5, const int margin_hi=5);
  bool AutoSetRangeX(TH2* h2, const int margin_lo=5, const int margin_hi=5);
  bool AutoSetRangeY(TH2* h2, const int margin_lo=5, const int margin_hi=5);
};

#endif /* _UTIL_HIST__H_ */
