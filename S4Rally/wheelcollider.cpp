//ヘッダファイル読み込み
#include "wheelcollider.h"

// コンストラクタ（デフォルト値を設定）
WheelCollider::WheelCollider()
    : mass(20.0f), radius(0.35f), wheelDampingRate(1.0f), suspensionDistance(0.2f), forceAppPointDistance(0.0f), center({ 0, 0, 0 })
{
    // 前後方向の摩擦設定（仮）
    forwardFriction = { 1.0f, 1.0f, 2.0f, 0.5f, 1.0f };

    // 横方向の摩擦設定（仮）
    sidewaysFriction = { 1.0f, 1.0f, 2.0f, 0.5f, 1.0f };
}

// デストラクタ（特別な後処理はなし）
WheelCollider::~WheelCollider()
{
    // ここでは特にリソース解放などは不要
}

// 地面に対するレイキャスト
bool WheelCollider::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal)
{
    extern Map map;

    // タイヤの中心から下に伸ばすレイと同じ位置で球を置く
    VECTOR sphereCenter = VAdd(origin, VScale(direction, maxDistance * 0.5f));

    MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(map.modelHandle, -1, sphereCenter, radius);

    if (result.HitNum > 0) {
        //// 最も近いヒットを取得
        //MV1_COLL_RESULT_POLY hit = MV1CollResultPolyDimGetHitPolyDim(&result, 0);
        //hitPoint = hit.Position[0]; // 三角形の1頂点（近似）
        //hitNormal = hit.Normal;
        //return true;
    }

    return false;
}

// サスペンションの沈み込み計算と地面ヒット情報取得
bool WheelCollider::GetGroundHit(const VECTOR& wheelPosition, VECTOR& hitPoint, VECTOR& hitNormal, float& suspensionCompression)
{
    VECTOR rayOrigin = VAdd(wheelPosition, center);  // ホイールの中心位置
    VECTOR rayDirection = VGet(0, -1, 0);            // 真下方向にレイ発射

    if (Raycast(rayOrigin, rayDirection, suspensionDistance + radius, hitPoint, hitNormal)) {
        // ホイール中心から地面までの距離を計算
        float distanceToGround = VSize(VSub(rayOrigin, hitPoint));

        // サスペンションの圧縮率を計算（0～1）
        suspensionCompression = 1.0f - (distanceToGround - radius) / suspensionDistance;
        suspensionCompression = Clamp(suspensionCompression, 0.0f, 1.0f);
        return true;
    }
    suspensionCompression = 0.0f;
    return false;
}

// 摩擦力を計算する
VECTOR WheelCollider::CalculateFrictionForce(const VECTOR& velocity, float suspensionCompression)
{
    VECTOR sideDir = VGet(1, 0, 0);    // 横方向（仮の右方向）
    VECTOR forwardDir = VGet(0, 0, 1); // 前方向（仮の奥方向）

    // 速度を前後・左右成分に分解
    float forwardSpeed = VDot(velocity, forwardDir);
    float sidewaysSpeed = VDot(velocity, sideDir);

    // 前後方向の摩擦力
    float forwardForce = -forwardSpeed * forwardFriction.stiffness * suspensionCompression;

    // 横方向の摩擦力
    float sidewaysForce = -sidewaysSpeed * sidewaysFriction.stiffness * suspensionCompression;

    // 摩擦力ベクトルを合成
    VECTOR friction = VAdd(VScale(forwardDir, forwardForce), VScale(sideDir, sidewaysForce));
    return friction;
}

// Clamp関数：値を[min, max]の範囲に収める
float WheelCollider::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
