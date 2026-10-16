#include <cassert>
#include <iostream>
#include "../SmartIrrigation/ControlLogic.h"

int main() {
  // 150 us: ~2.57 cm distance, ~3.73 cm water. 250 us: low tank.
  for (bool error : {false, true})
    for (bool low : {false, true})
      for (bool dry : {false, true})
        for (bool rain : {false, true}) {
          auto s = evaluateIrrigation(dry ? 501 : 500, rain,
                                     error ? NAN : 25.0f, 60.0f, low ? 250 : 150);
          assert(s.systemError == error);
          assert(s.tankLow == low);
          assert(s.motorRunning == (!error && !low && dry && !rain));
        }
  auto missing = evaluateIrrigation(700, false, 25, 60, 0);
  assert(missing.systemError && !missing.motorRunning);
  assert(missing.distanceCm == -1 && missing.waterLevelCm == -1);
  assert(!missing.tankLow);
  assert(evaluateIrrigation(700, false, 25, NAN, 150).systemError);
  auto empty = evaluateIrrigation(700, false, 25, 60, 1000);
  assert(empty.waterLevelCm == 0 && empty.tankLow && !empty.systemError);
  assert(!evaluateIrrigation(700, false, 25, 60, 233).tankLow);
  assert(evaluateIrrigation(700, false, 25, 60, 234).tankLow);
  assert(!evaluateIrrigation(500, false, 25, 60, 150).motorRunning);
  assert(evaluateIrrigation(501, false, 25, 60, 150).motorRunning);
  std::cout << "All 16 decision combinations and boundary/failure checks passed.\n";
}
