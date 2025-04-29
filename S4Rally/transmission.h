#pragma once

#include "DxLib.h"
#include "controller.h"
#include "key.h"

enum TransmissionMode {
    Manual,
    Automatic
};

class Transmission {
public:
    int gear;
    int maxGear;
    bool gearChangeLocked;
    float gearRatios[8];
    float maxSpeeds[8];

    TransmissionMode mode;

    Transmission();

    void Update(float velocity, float engineRPM);
    void ForceShiftUp();
    int GetGear() const;
    float GetGearRatio() const;

    float CarculateRPM(float velocity, float wheelRadius) const;
    float CarculateEngineForce(float engineForce) const;

    void SetGearRatios(const float* ratios, int size);
    void SetMaxSpeeds(const float* speeds, int size);
    void ToggleMode(); // ÉÇÅ[ÉhêÿÇËë÷Ç¶
};