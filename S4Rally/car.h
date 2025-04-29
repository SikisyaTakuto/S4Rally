// Car.h
#pragma once

// ヘッダファイル読み込み
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
#include "rigidbody.h"
#include <type_traits>
#include <algorithm>
#include <vector> 

class Car  : public RigidBody, public SuspensionSpring,public Engine,public Transmission
{
public:

    Car(); // コンストラクタ
    ~Car(); // デストラクタ

    void Init();  // 初期化
    void Update(float deltaTime); // 更新
    void Draw();   // 描画

    bool CheckBodyCollision();

    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    VECTOR GetPosition() const { return carBodyPosition; }
    VECTOR GetRotation() const { return carBodyRotation; }

private:

    enum WheelPosition
    {
        FrontLeft,
        FrontRight,
        RearLeft,
        RearRight,
        WheelNum
    };

    int carModelHandle;        // 車体モデルハンドル
    VECTOR carBodyPosition;    // 車体の位置
    VECTOR carBodyRotation;    // 車体の回転

    int wheelModelHandles[WheelNum];    // ホイールモデルハンドル
    VECTOR wheelPositions[WheelNum];    // ホイールの位置
    VECTOR wheelRotations[WheelNum];    // ホイールの回転

    VECTOR wheelOffsets[WheelNum]; // 車体からのオフセット位置（左右前後）

    float motorForce;      // 駆動力
    float steeringAngle;   // ハンドル角度
    float accelInput;
    float steerInput;
};