//ホイールコライダーのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include"DxLib.h"
#include "fps.h"
#include "geometory.h"
#include "controller.h"
#include "key.h"
#include "map.h"

// 駆動方式の定義
enum DriveType {
    FWD, // 前輪駆動
    RWD, // 後輪駆動
    AWD  // 四輪駆動
};

//構造体
typedef struct _SuspensionSpring//サスペンションスプリングの情報
{
    float spring;           // スプリング力
    float damper;           // 減衰力
    float targetPosition;   // サスペンションの残り距離 (0.0 ～ 1.0)
} SuspensionSpring;

typedef struct _WheelCollider   // ホイールコライダーの情報
{
    float mass;                        // ホイールの質量
    float radius;                      // ホイールの半径
    float wheelDampingRate;            // 減衰値
    float suspensionDistance;          // サスペンションの最大距離
    float forceAppPointDistance;       // 力の適用ポイント
    VECTOR center;                     // ホイールの中心位置
    VECTOR rotation;                   //ホイールの回転
    SuspensionSpring suspensionSpring; // サスペンションスプリング
    float forwardFriction;             // 前進摩擦
    float sidewaysFriction;            // 横方向摩擦

    VECTOR velocity;                   // 現在のホイール速度
    float currentSuspensionCompression;// 現在のサスペンション圧縮率

    VECTOR frontWheelPosition;         //前輪（2輪の中間）の位置
    VECTOR frontWheelRotation;         //前輪（2輪の中間）の回転
    VECTOR rearWheelPosition;          //後輪（2輪の中間）の位置
    VECTOR rearWheelRotation;          //後輪（2輪の中間）の回転

    int wheelModelHandleLeft;              // タイヤモデルのハンドル
    int wheelModelHandleRight;              // タイヤモデルのハンドル

} WheelCollider;

typedef struct _ForwardFriction//前後摩擦の情報
{
    float forwardStiffness;             // 摩擦係数
} ForwardFriction;

typedef struct _SidewaysFriction//左右摩擦の情報
{
    float sidewaysStiffness;             // 摩擦係数
} SidewaysFriction;

// 車両情報
typedef struct _Car {
    WheelCollider frontLeft;
    WheelCollider frontRight;
    WheelCollider rearLeft;
    WheelCollider rearRight;
    DriveType driveType; // 駆動方式
} Car;

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
    float drag = 0.1f;               // 移動時の抵抗

    int gear;                        // 現在のギア (1-5)
    float currentSpeed;              // 現在速度 (m/s)
    float maxSpeed;                  // 最大速度 (m/s)
    bool gearChangeLocked;           // ギアチェンジのロック

    // 追加する変数
    float engineRPM;                 // エンジン回転数 (初期値 1000 RPM)
    float idleRPM;                   // アイドリング回転数
    float maxRPM;                    // 最大回転数
    float redlineRPM;                // レッドゾーン開始回転数
    float engineBrakeCoefficient;    // エンジンブレーキの効き

    int carModelHandle;              // 車体モデルハンドル

} CarInfo;


//外部グローバル変数
extern WheelCollider wheelCollider;
extern SuspensionSpring suspensionSpring;
extern ForwardFriction forwardFriction;
extern SidewaysFriction sidewaysFriction;
extern Car car;
extern CarInfo carInfo;

//外部プロトタイプ宣言
extern VOID CarInit(VOID);                              // 車両情報の初期化
extern VOID CarUpdate(VOID);                            // 車両情報の更新
extern VOID CarDraw(VOID);                              // 描画
extern VOID CarSetCamera(VOID);                         // カメラ設定
extern FLOAT CarCulateEngineForce(int gear);            // ギアの変更
extern FLOAT CarCulateRPM(float velocity, int gear);    // RPMの計算関数
extern VOID CarApplySteering(VOID);                     // 操舵角度の設定
extern FLOAT CarApplyAcceleration(float deltaTime);     // アクセルの適用
extern FLOAT CarApplyBraking(float deltaTime);          // ブレーキの適用