#include <cassert>
#include "UtilTarget.h"
using namespace std;

/// Return the name for the given target position.
string UtilTarget::GetName(const unsigned int tgt_pos)
{
  assert(0 <= tgt_pos && tgt_pos <= 7);
  const string NAME[8] = { "All", "LH2", "Empty", "LD2", "None", "Fe", "C", "W" };
  return NAME[tgt_pos];
}
