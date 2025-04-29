#include "transmission.h"

// 初期化
Transmission::Transmission() {
    gear = 1;
    maxGear = 5;
    gearChangeLocked = false;
    mode = Manual;

    float defaultRatios[5] = { 25.0f,18.0f, 13.0f, 10.0f, 8.0f };
    float defaultSpeeds[5] = { 5000.0f, 7000.0f, 9000.0f, 11000.0f, 15000.0f };

    SetGearRatios(defaultRatios, 5);
    SetMaxSpeeds(defaultSpeeds, 5);
}

void Transmission::Update(float velocity, float engineRPM) {
    if (mode == Manual) {
        if (!gearChangeLocked) {
            if (CheckHitKey(KEY_INPUT_E) && gear < maxGear) {
                gear++;
                gearChangeLocked = true;
            }
            if (CheckHitKey(KEY_INPUT_Q) && gear > 1) {
                gear--;
                gearChangeLocked = true;
            }
        }
        if (!CheckHitKey(KEY_INPUT_Q) && !CheckHitKey(KEY_INPUT_E)) {
            gearChangeLocked = false;
        }
    }
    else if (mode == Automatic) {
        // オートマチック時
        if (engineRPM > 8000.0f && gear < maxGear) {
            gear++;
        }
        else if (engineRPM < 2000.0f && gear > 1) {
            gear--;
        }
    }

    // モード切り替え（例えばTabキー）
    if (KeyDown(KEY_INPUT_TAB)) {
        ToggleMode();
    }
}

void Transmission::ToggleMode() {
    if (mode == Manual) {
        mode = Automatic;
    }
    else {
        mode = Manual;
    }
}

void Transmission::ForceShiftUp() {
    if (gear <= maxGear) {
        gear++;
    }
}


// 現在のギア取得
int Transmission::GetGear() const {
    return gear;
}

// 現在のギア比取得
float Transmission::GetGearRatio() const {
    return gearRatios[gear - 1];
}

// エンジン回転数計算
float Transmission::CarculateRPM(float velocity, float wheelRadius) const {
    const float finalDriveRatio = 4.1f; // デファレンシャル比
    float rpm = (velocity / wheelRadius) * GetGearRatio() * finalDriveRatio * 60.0f / (2.0f * DX_PI);
    return rpm;
}

// エンジンフォース計算（ギア比込み）
float Transmission::CarculateEngineForce(float engineForce) const {
    return engineForce * GetGearRatio();
}

// ギア比設定
void Transmission::SetGearRatios(const float* ratios, int size) {
    for (int i = 0; i < size && i < 8; ++i) {
        gearRatios[i] = ratios[i];
    }
    maxGear = size;
}

// 各ギアの最高速設定
void Transmission::SetMaxSpeeds(const float* speeds, int size) {
    for (int i = 0; i < size && i < 8; ++i) {
        maxSpeeds[i] = speeds[i];
    }
}