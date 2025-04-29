#include "rigidbody.h"

// �R���X�g���N�^
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

// �͂�������
void RigidBody::AddForce(VECTOR f)
{
    force = VAdd(force, f);
}

// ���t���[���X�V
void RigidBody::Update(float deltaTime)
{
    if (mass <= 0.0f) return;

    VECTOR totalForce = force;

    // �d�͂�������
    if (isGravity)
    {
        VECTOR gravity = VGet(0, -9.81f * mass, 0); // �d�͉����x
        totalForce = VAdd(totalForce, gravity);
    }

    // �����x = �� / ����
    VECTOR acceleration = VScale(totalForce, 1.0f / mass);

    // ���x���X�V
    velocity = VAdd(velocity, VScale(acceleration, deltaTime));

    // ��C��R��������
    velocity = VScale(velocity, 1.0f / (1.0f + drag * deltaTime));

    // �ʒu���X�V
    position = VAdd(position, VScale(velocity, deltaTime));

    // �͂����Z�b�g
    force = VGet(0, 0, 0);
}