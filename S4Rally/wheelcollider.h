#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "car.h"
#include "suspension.h"

// 駆動方式の定義
enum DriveType {
    FWD, // 前輪駆動
    RWD, // 後輪駆動
    AWD  // 四輪駆動
};

typedef struct _WheelCollider   // ホイールコライダーの情報
{
    float mass;                        // ホイールの質量
    float radius;                      // ホイールの半径
    float wheelDampingRate;            // 減衰値
    float suspensionDistance;          // サスペンションの最大距離
    float forceAppPointDistance;       // 力の適用ポイント
    VECTOR center;                     // ホイールの中心位置
    VECTOR rotation;                   //ホイールの回転
    float forwardFriction;             // 前進摩擦
    float sidewaysFriction;            // 横方向摩擦

    VECTOR velocity;                   // 現在のホイール速度
    float currentSuspensionCompression;// 現在のサスペンション圧縮率

    VECTOR frontWheelPosition;         //前輪（2輪の中間）の位置
    VECTOR frontWheelRotation;         //前輪（2輪の中間）の回転
    VECTOR rearWheelPosition;          //後輪（2輪の中間）の位置
    VECTOR rearWheelRotation;          //後輪（2輪の中間）の回転

    int wheelModelHandleLeft;          // タイヤモデルのハンドル
    int wheelModelHandleRight;         // タイヤモデルのハンドル

}WheelCollider;

typedef struct _ForwardFriction//前後摩擦の情報
{
    float forwardStiffness;    // 摩擦係数
}ForwardFriction;

typedef struct _SidewaysFriction//左右摩擦の情報
{
    float sidewaysStiffness;    // 摩擦係数
}SidewaysFriction;

// 車両情報
typedef struct _Car {
    WheelCollider frontLeft;
    WheelCollider frontRight;
    WheelCollider rearLeft;
    WheelCollider rearRight;
    DriveType driveType;        // 駆動方式
}Car;

//外部グローバル変数
extern Car car;
extern WheelCollider wheelCollider;
extern ForwardFriction forwardFriction;
extern SidewaysFriction sidewaysFriction;

//外部プロトタイプ宣言
extern VOID CarApplySteering(VOID);// 操舵角度の設定
extern VOID ApplyVehicleDynamics(float deltaTime);
extern VOID ApplyFriction(float deltaTime);

