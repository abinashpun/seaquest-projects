/// ROOT macro common to AnaStep1.C and AnaStep2.C
{
  ///
  /// Choose (i.e. uncomment) only one of the analysis modules.
  ///
  //auto ana = new AnaOccupancy();
  //auto ana = new AnaHitMatrix();
  auto ana = new AnaHodoTime();
  //auto ana = new AnaChamHit();
  //auto ana = new AnaNim3();
}
