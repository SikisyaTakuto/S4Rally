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

    void Init();  // 車両の初期化関数
    void Update(float deltaTime); // 毎フレームの更新処理
    void Draw();   // 車両モデルの描画処理

    // 車体のコリジョンチェック（壁などに衝突しているか判定）
    bool CheckBodyCollision();

    // レイキャストを飛ばして衝突判定を行う
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    // 車体の現在位置を取得
    VECTOR GetPosition() const { return carBodyPosition; }

    // 車体の現在回転を取得
    VECTOR GetRotation() const { return carBodyRotation; }

private:

    // ホイールの位置種別
    enum WheelPosition
    {
        FrontLeft,   // 前左
        FrontRight,  // 前右
        RearLeft,    // 後左
        RearRight,   // 後右
        WheelNum     // ホイール数
    };

    int wheelModelHandles[WheelNum];    // 各ホイールモデルのハンドル
    VECTOR wheelPositions[WheelNum];    // 各ホイールのワールド座標位置
    VECTOR wheelRotations[WheelNum];    // 各ホイールの回転（オイラー角）

    VECTOR wheelOffsets[WheelNum];      // 車体中心から各ホイールまでのオフセット位置

    float motorForce;       // エンジンの駆動力（アクセルによる力）
    float steeringAngle;    // 現在のステアリング角度
    float accelInput;       // アクセル入力値（通常0.0?1.0）
    float steerInput;       // ステアリング入力値（-1.0?1.0）
    float sideBrakeInput;   // サイドブレーキ入力値（0.0 or 1.0）

    // 車体前面の始点・終点（レイキャスト用）
    VECTOR frontStart;
    VECTOR frontEnd;

    // 車体後面の始点・終点（レイキャスト用）
    VECTOR backStart;
    VECTOR backEnd;

    // 車体左面の始点・終点（レイキャスト用）
    VECTOR leftStart;
    VECTOR leftEnd;

    // 車体右面の始点・終点（レイキャスト用）
    VECTOR rightStart;
    VECTOR rightEnd;

    // 各方向への衝突検知フラグ
    bool hitFront;
    bool hitBack;
    bool hitLeft;
    bool hitRight;

    // 減速用レイキャストの始点・終点（前方）
    VECTOR frontStartDeceleration;
    VECTOR frontEndDeceleration;

    // 減速用レイキャストの始点・終点（後方）
    VECTOR backStartDeceleration;
    VECTOR backEndDeceleration;

    // 減速用レイキャストの始点・終点（左側）
    VECTOR leftStartDeceleration;
    VECTOR leftEndDeceleration;

    // 減速用レイキャストの始点・終点（右側）
    VECTOR rightStartDeceleration;
    VECTOR rightEndDeceleration;

    // 減速判定用のヒットフラグ
    bool hitFrontDeceleration;
    bool hitBackDeceleration;
    bool hitLeftDeceleration;
    bool hitRightDeceleration;

    // 車体のバウンディングボックスの頂点リスト（8頂点）
    VECTOR boxVertices[8];

    // 車体のバウンディングボックスサイズ設定
    float boxHalfWidth = 100.0f;   // 半幅（左右方向）
    float boxHalfLength = 160.0f;  // 半長さ（前後方向）
    float boxHeight = 50.0f;       // 高さ（上下方向）
};