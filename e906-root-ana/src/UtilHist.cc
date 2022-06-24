#include <iomanip>
#include <TH1D.h>
#include <TH2D.h>
#include "UtilHist.h"
using namespace std;

bool UtilHist::FindFilledRange(TH1* h1, int& bin_lo, int& bin_hi)
{
  bin_lo = bin_hi = 0;
  if (h1->Integral() == 0) return false;
  int nn = h1->GetNbinsX();
  bin_lo = 1;
  while (h1->GetBinContent(bin_lo) == 0) bin_lo++;
  bin_hi = nn;
  while (h1->GetBinContent(bin_hi) == 0) bin_hi--;
  return true;
}

bool UtilHist::AutoSetRange(TH1* h1, const int margin_lo, const int margin_hi)
{
  int nn = h1->GetNbinsX();
  int bin_lo, bin_hi;
  if (! FindFilledRange(h1, bin_lo, bin_hi)) return false;
  bin_lo -= margin_lo;
  bin_hi += margin_hi;
  if (bin_lo <  1) bin_lo =  1;
  if (bin_hi > nn) bin_hi = nn;
  h1->GetXaxis()->SetRange(bin_lo, bin_hi);
  return true;
}

bool UtilHist::AutoSetRangeX(TH2* h2, const int margin_lo, const int margin_hi)
{
  TH1* h1 = h2->ProjectionX("h1_auto_set_range_x");
  int nn = h1->GetNbinsX();
  int bin_lo, bin_hi;
  if (! FindFilledRange(h1, bin_lo, bin_hi)) return false;
  delete h1;
  bin_lo -= margin_lo;
  bin_hi += margin_hi;
  if (bin_lo <  1) bin_lo =  1;
  if (bin_hi > nn) bin_hi = nn;
  h2->GetXaxis()->SetRange(bin_lo, bin_hi);
  return true;
}

bool UtilHist::AutoSetRangeY(TH2* h2, const int margin_lo, const int margin_hi)
{
  TH1* h1 = h2->ProjectionY("h1_auto_set_range_y");
  int nn = h1->GetNbinsX();
  int bin_lo, bin_hi;
  if (! FindFilledRange(h1, bin_lo, bin_hi)) return false;
  delete h1;
  bin_lo -= margin_lo;
  bin_hi += margin_hi;
  if (bin_lo <  1) bin_lo =  1;
  if (bin_hi > nn) bin_hi = nn;
  h2->GetYaxis()->SetRange(bin_lo, bin_hi);
  return true;
}
