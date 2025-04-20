#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "car.h"

// 車両情報構造体
typedef struct _EngineInfo {
    float engineRPM;                 // エンジン回転数 (初期値 1000 RPM)
    float idleRPM;                   // アイドリング回転数
    float maxRPM;                    // 最大回転数
    float redlineRPM;                // レッドゾーン開始回転数
    float engineBrakeCoefficient;    // エンジンブレーキの効き

    int carModelHandle;              // 車体モデルハンドル

}EngineInfo;

extern EngineInfo engineInfo;

//外部プロトタイプ宣言
extern FLOAT CarCulateRPM(float velocity, int gear);    // RPMの計算関数
extern VOID UpdateEngineRPM(VOID);