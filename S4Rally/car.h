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
#include "rigidbody.h"
#include <type_traits>
#include <algorithm>
#include <vector> 

class Car
{
public:

    int carModelHandle;        // 車体モデルハンドル
    VECTOR carBodyPosition;    // 車体の位置
    VECTOR carBodyRotation;    // 車体の回転

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

    int wheelModelHandles[WheelNum];    // ホイールモデルハンドル
    VECTOR wheelPositions[WheelNum];    // ホイールの位置
    VECTOR wheelRotations[WheelNum];    // ホイールの回転

    VECTOR wheelOffsets[WheelNum]; // 車体からのオフセット位置（左右前後）

    float motorForce;      // 駆動力
    float steeringAngle;   // ハンドル角度
    float accelInput;
    float steerInput;
    float sideBrakeInput;  // サイドブレーキ入力（押してると1.0、押してなければ0.0）

    VECTOR frontStart;
    VECTOR frontEnd ;

    VECTOR backStart;
    VECTOR backEnd;

    VECTOR leftStart;
    VECTOR leftEnd;

    VECTOR rightStart;
    VECTOR rightEnd;

    bool hitFront;
    bool hitBack;
    bool hitLeft;
    bool hitRight;



    VECTOR frontStartDeceleration;
    VECTOR frontEndDeceleration;

    VECTOR backStartDeceleration;
    VECTOR backEndDeceleration;

    VECTOR leftStartDeceleration;
    VECTOR leftEndDeceleration;

    VECTOR rightStartDeceleration;
    VECTOR rightEndDeceleration;

    bool hitFrontDeceleration;
    bool hitBackDeceleration;
    bool hitLeftDeceleration;
    bool hitRightDeceleration;



    VECTOR boxVertices[8]; // 車体Boxの頂点リスト
    float boxHalfWidth = 100.0f;  // 左右方向 半幅
    float boxHalfLength = 160.0f; // 前後方向 半長さ
    float boxHeight = 50.0f;   // 高さ
};