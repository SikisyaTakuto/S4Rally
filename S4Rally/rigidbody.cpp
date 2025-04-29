#include "rigidbody.h"

// コンストラクタ
RigidBody::RigidBody()
{
    mass = 1.0f;
    drag = 0.0f;
    angularDrag = 0.05f;
    isGravity = true;
    position = VGet(0, 0, 0);
    velocity = VGet(0, 0, 0);
    force = VGet(0, 0, 0);
}

// 力を加える
void RigidBody::AddForce(VECTOR f)
{
    force = VAdd(force, f);
}

// 毎フレーム更新
void RigidBody::Update(float deltaTime)
{
    if (mass <= 0.0f) return;

    VECTOR totalForce = force;

    // 重力をかける
    if (isGravity)
    {
        VECTOR gravity = VGet(0, -9.81f * mass, 0); // 重力加速度
        totalForce = VAdd(totalForce, gravity);
    }

    // 加速度 = 力 / 質量
    VECTOR acceleration = VScale(totalForce, 1.0f / mass);

    // 速度を更新
    velocity = VAdd(velocity, VScale(acceleration, deltaTime));

    // 空気抵抗をかける
    velocity = VScale(velocity, 1.0f / (1.0f + drag * deltaTime));

    // 位置を更新
    position = VAdd(position, VScale(velocity, deltaTime));

    // 力をリセット
    force = VGet(0, 0, 0);
}