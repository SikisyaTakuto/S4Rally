#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "wheelcollider.h"
#include "fps.h"
#include "car.h"
#include "map.h"

//構造体
typedef struct _SuspensionSpring//サスペンションスプリングの情報
{
    float spring;           // スプリング力
    float damper;           // 減衰力
    float targetPosition;   // サスペンションの残り距離 (0.0 ～ 1.0)
}SuspensionSpring;

//外部グローバル変数
extern SuspensionSpring suspensionSpring;

//外部プロトタイプ宣言
extern VOID CarBicycleModelUpdate(float deltaTime);                             
//extern VOID ApplySuspension(WheelCollider wheel, VECTOR hitPos, float deltaTime);
extern VOID UpdateCarPitchAndRoll(VOID);               
extern VOID SuspensionDrow(float deltaTime);