//ホイールコライダーのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "fps.h"
#include "geometory.h"
#include "controller.h"
#include "key.h"
#include "map.h"
#include "camera.h"
#include "suspension.h"
#include "wheelcollider.h"
#include "engine.h"
#include "transmission.h"

// 車両情報構造体
typedef struct _CarInfo {
    float engineForse;               //エンジンの力 (N)
    float ballast;                   // バラスト重量
    float acceleration;              // 加速度
    float brakeForce;                // ブレーキ力
    float turboMultiplier;           // ターボ倍率
    bool turboEnabled;               // ターボの有効化
    float steeringAngle;             // 操舵角度
    float maxSteeringAngle;          // 最大操舵角度
    float minSteeringAngle;          // 最小操舵角度
    VECTOR position;                 // 車両の位置
    VECTOR rotation;                 // 車両の回転

    float mass = 1.0f;               // 質量
    float drag = 0.1f;               // 移動時の抵抗]
    float currentSpeed;              // 現在速度 (m/s)
    float maxSpeed;                  // 最大速度 (m/s)

    int carModelHandle;              // 車体モデルハンドル

} CarInfo;

extern CarInfo carInfo;

//外部プロトタイプ宣言
extern VOID CarInit(VOID);                              // 車両情報の初期化
extern VOID CarUpdate(VOID);                            // 車両情報の更新
extern VOID CarDraw(VOID);                              // 描画
extern FLOAT CarCulateEngineForce(int gear);            // ギアの変更
extern FLOAT CarApplyAcceleration(float deltaTime);     // アクセルの適用
extern FLOAT CarApplyBraking(float deltaTime);          // ブレーキの適用