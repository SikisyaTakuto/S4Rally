#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "suspension.h"
#include "wheelcollider.h"
#include "engine.h"
#include "car.h"

// 車両情報構造体
typedef struct _TransmissionInfo {

    int gear;               // 現在のギア (1-5)
    int maxGear;
    bool gearChangeLocked;  // ギアチェンジのロック
    float gearRatios[8];     // ギア比
    float maxSpeeds[8];      // 各ギアの最大速度 (m/s)

} TransmissionInfo;

extern TransmissionInfo transmissionInfo;

extern FLOAT  CarCulateRPM(float velocity, int gear);
extern FLOAT CarCulateEngineForce(int gear);
extern VOID ChangeGear(VOID);