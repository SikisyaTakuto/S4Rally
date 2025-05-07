#include "rigidbody.h"

// コンストラクタ
RigidBody::RigidBody()
{
    mass = 1.0f;           // 質量（初期値1.0）
    drag = 0.0f;           // 直線運動の空気抵抗（減衰）係数
    angularDrag = 0.05f;   // 回転運動の空気抵抗（未使用）
    isGravity = true;      // 重力の影響を受けるかどうか
    isGrounded = false;    // 接地しているかどうか
    position = VGet(0, 0, 0); // 初期位置（原点）
    velocity = VGet(0, 0, 0); // 初期速度（停止状態）
    force = VGet(0, 0, 0);    // 加えられた力（初期はなし）
}

// 力を加える関数
void RigidBody::AddForce(VECTOR f)
{
    // 現在の力に新たな力を加算する
    force = VAdd(force, f);
}

// 物理計算の更新処理（毎フレーム呼び出し）
void RigidBody::Update(float deltaTime)
{
    // 質量が0以下なら物理演算をスキップ（静的オブジェクト扱い）
    if (mass <= 0.0f) return;

    // 現在加えられている全ての力を合算
    VECTOR totalForce = force;

    // 重力を適用（isGravityがtrueかつ空中にいる場合）
    if (isGravity && !isGrounded)
    {
        // 重力加速度（地球上なら約9.81m/s^2）を質量分だけ力に換算
        VECTOR gravity = VGet(0, -9.81f * mass, 0);
        totalForce = VAdd(totalForce, gravity);
    }

    // 加速度 = 力 / 質量（ニュートンの第二法則）
    VECTOR acceleration = VScale(totalForce, 1.0f / mass);

    // 現在速度に加速度×時間を加算して、新しい速度を計算
    velocity = VAdd(velocity, VScale(acceleration, deltaTime));

    // 空気抵抗（Drag）を適用（時間に比例して速度を減衰）
    velocity = VScale(velocity, 1.0f / (1.0f + drag * deltaTime));

    // 現在位置に速度×時間を加算して、新しい位置を計算
    position = VAdd(position, VScale(velocity, deltaTime));

    // フレームの最後に加えられた力をリセット（次フレームへ持ち越さない）
    force = VGet(0, 0, 0);
}