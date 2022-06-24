#include <E906DataCatalog.h>
#include "UtilBeam.h"
using namespace std;

/// Return the raw number of PoT.
double UtilBeam::PoTRaw(const unsigned int spill_id)
{
  auto e906sc = E906DataCatalog::instance();
  auto beam   = e906sc->Beam(spill_id);
  if (! beam) return 0;
  return beam->GetDouble("S:G2SEM");
}

/// Return the live number of PoT.
/**
 * The QIE pedestal is _not_ subtracted at present.
 */
double UtilBeam::PoTLive(const unsigned int spill_id)
{
  auto e906sc  = E906DataCatalog::instance();
  auto beam    = e906sc->Beam   (spill_id);
  auto beamdaq = e906sc->BeamDaq(spill_id);
  if (! beam || ! beamdaq) return 0;

  double g2sem = beam   ->GetDouble("S:G2SEM");
  double qie   = beamdaq->GetDouble("QIEsum");
  double inhi  = beamdaq->GetDouble("inhibit_block_sum");
  double busy  = beamdaq->GetDouble("trigger_sum_no_inhibit");
  return g2sem * (qie - inhi - busy) / qie;
}

/// Return PoT/QIEsum.
/**
 * The QIE pedestal is subtracted from QIEsum.
 */
double UtilBeam::PoTPerQIE(const unsigned int spill_id)
{
  const int TURNS_PER_SPILL  = 369000;
  const int BUCKETS_PER_TURN = 588;

  auto e906sc  = E906DataCatalog::instance();
  auto beam    = e906sc->Beam   (spill_id);
  auto beamdaq = e906sc->BeamDaq(spill_id);
  if (! beam || ! beamdaq) return 0;

  double g2sem   = beam   ->GetDouble("S:G2SEM");
  double qie_sum = beamdaq->GetDouble("QIEsum");
  double qie_ped = e906sc->GetQIEPedestal(spill_id);

  return g2sem / (qie_sum - qie_ped * TURNS_PER_SPILL * BUCKETS_PER_TURN);
}

/// Convert the QIE count to the number of protons.
/**
 * This function does *not* subtract the QIE pedestal from `qie_count`.
 */
double UtilBeam::QIE2Proton(const int qie_count, const unsigned int spill_id)
{
  return qie_count * PoTPerQIE(spill_id);
}

/// Make a list of QIE RF+nn values.
/**
 * The possible values for QIE RF+nn are not evenly distributed.
 * This function just fills `list_values` with a list of all values.
 * The values are taken from "float_2_linrom-2.xls" in DocDB 537.
 */
void UtilBeam::ListOfRfValues(int& n_value, int*& list_values)
{
  static int idx = 0;
  static int list[256];
  if (idx == 0) {
    int val = 0;
    list[idx++] = val;
    for (int i = 0; i < 16; i++) { val +=    1; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val +=    2; list[idx++] = val; }
    for (int i = 0; i < 21; i++) { val +=    4; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val +=    8; list[idx++] = val; }
    for (int i = 0; i < 15; i++) { val +=   16; list[idx++] = val; }
    for (int i = 0; i <  1; i++) { val +=   31; list[idx++] = val; }
    for (int i = 0; i <  4; i++) { val +=   16; list[idx++] = val; }
    for (int i = 0; i < 21; i++) { val +=   32; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val +=   64; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val +=  128; list[idx++] = val; }
    for (int i = 0; i < 21; i++) { val +=  256; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val +=  512; list[idx++] = val; }
    for (int i = 0; i < 20; i++) { val += 1024; list[idx++] = val; }
    for (int i = 0; i < 21; i++) { val += 2048; list[idx++] = val; }
    for (int i = 0; i <  6; i++) { val += 4096; list[idx++] = val; }
  }
  n_value = idx;
  list_values = list;
}

/// Make a list of QIE RF+nn values.
/**
 * This function uses an array of "double", instead of "int".
 * It is suitable for the constructors of the TH1 classes.
 * @code
 * int num_inte;
 * double* list_inte;
 * UtilBeam::ListOfRfValues(num_inte, list_inte);
 * TH1* h1_rf_inte = new TH1D("h1_rf_inte", "", num_inte - 1, list_inte);
 * @endcode
 */
void UtilBeam::ListOfRfValues(int& n_value, double*& list_values)
{
  static int idx = 0;
  static double list[256];
  if (idx == 0) {
    int* list_int;
    ListOfRfValues(idx, list_int);
    for (int i = 0; i < idx; i++) list[i] = list_int[i];
  }
  n_value = idx;
  list_values = list;
}
