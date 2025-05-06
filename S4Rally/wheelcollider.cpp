//ヘッダファイル読み込み
#include "wheelcollider.h"

// コンストラクタ（デフォルト値を設定）
WheelCollider::WheelCollider()
    : mass(20.0f), radius(35.0f), wheelDampingRate(1.0f), suspensionDistance(0.2f), forceAppPointDistance(0.0f), center({ 0, 0, 0 })
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

        return true; // 当たっている
    }

    return false;
}

bool WheelCollider::GetGroundHit(const VECTOR& wheelPos, VECTOR& hitPos, VECTOR& hitNormal, float& compression)
{
    extern Map map;

    MV1_COLL_RESULT_POLY_DIM hitResult;

    // 車輪の半径（適当に設定）
    const float wheelRadius = 18.0f;

    // マップのモデルハンドルを取得
    int mapHandle = map.modelHandle;

    // 接触判定（球体とマップ）
    hitResult = MV1CollCheck_Sphere(mapHandle, -1, wheelPos, wheelRadius);

    if (hitResult.HitNum > 0) {
        // 最初にヒットしたポリゴンの情報を取得
        hitPos = hitResult.Dim[0].Position[0];  // 0番目の頂点
        hitNormal = hitResult.Dim[0].Normal;

        // 圧縮率（サスペンションの沈み具合）を計算
        float distance = wheelPos.y - hitPos.y;
        compression = distance / wheelRadius;
        return true;
    }

    // 接地してない場合
    compression = 1.0f;
    return false;
}

// 摩擦力を計算する
VECTOR WheelCollider::CarCulateFrictionForce(const VECTOR& velocity, float suspensionCompression, float sideBrakeInput)
{
    extern Car car;

    RigidBody rigidBody;

    VECTOR frictionForce = VGet(0, 0, 0);

    // 地面に接地していなければ摩擦力なし
    if (rigidBody.isGrounded) {
        return frictionForce;
    }

    // --- 速度の成分分解（前後方向、左右方向）
    VECTOR forward = VNorm(VGet(sinf(car.carBodyRotation.y), 0.0f, cosf(car.carBodyRotation.y)));  // 車の前方向
    VECTOR right = VCross(VGet(0, 1, 0), forward); // 車の右方向

    float forwardSpeed = VDot(velocity, forward);  // 前後成分
    float sidewaysSpeed = VDot(velocity, right);   // 横成分

    // --- 縦方向（前後方向）摩擦（エンジンブレーキ的）
    const float rollingResistanceCoef = 0.015f; // 転がり抵抗係数
    VECTOR rollingResistance = VScale(forward, -forwardSpeed * rollingResistanceCoef);

    // --- 横方向摩擦（サイドブレーキ時に大きくする）
    float lateralFrictionCoef = 0.5f; // 通常時の横摩擦係数
    if (sideBrakeInput > 0.5f) {
        lateralFrictionCoef = 2.0f; // サイドブレーキ中は横滑り強める
    }
    VECTOR lateralFriction = VScale(right, -sidewaysSpeed * lateralFrictionCoef);

    // --- 合算
    frictionForce = VAdd(rollingResistance, lateralFriction);

    return frictionForce;
}

// Clamp関数：値を[min, max]の範囲に収める
float WheelCollider::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

//タイヤ位置に球を描画する関数を追加
void WheelCollider::Draw(const VECTOR& wheelPosition)
{
    // タイヤの中心＋オフセット
    VECTOR spherePos = VAdd(wheelPosition, center);

    // 線分のみで球を描く（塗りつぶしなし）
    DrawSphere3D(
        spherePos,       // 球の中心座標
        radius,          // 球の半径
        10,              // 緯線数（細かさ）
        10,              // 経線数（細かさ）
        GetColor(255, 0, 0),  // 色（赤色）
        false            // 塗りつぶさない
    );
}