#pragma once
#include <math.h>

// Defaults documented in the supplied project explanation. Calibrate locally.
constexpr int DRY_THRESHOLD = 500;
constexpr float TANK_HEIGHT_CM = 6.3f;
constexpr float LOW_WATER_CM = 2.3f;

struct IrrigationState {
  float distanceCm;
  float waterLevelCm;
  bool systemError;
  bool tankLow;
  bool soilDry;
  bool rainDetected;
  bool motorRunning;
};

inline IrrigationState evaluateIrrigation(int soil, bool rain, float temperature,
                                         float humidity, unsigned long echoUs) {
  IrrigationState s = {};
  s.distanceCm = echoUs == 0 ? -1.0f : echoUs * 0.0343f / 2.0f;
  s.waterLevelCm = echoUs == 0 ? -1.0f : TANK_HEIGHT_CM - s.distanceCm;
  if (echoUs != 0 && s.waterLevelCm < 0) s.waterLevelCm = 0;
  s.systemError = isnan(temperature) || isnan(humidity) || s.distanceCm < 0;
  s.tankLow = s.distanceCm >= 0 && s.waterLevelCm <= LOW_WATER_CM;
  s.soilDry = soil > DRY_THRESHOLD;
  s.rainDetected = rain;
  s.motorRunning = !s.systemError && !s.tankLow && s.soilDry && !rain;
  return s;
}
