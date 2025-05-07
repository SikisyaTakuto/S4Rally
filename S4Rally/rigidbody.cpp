#include "rigidbody.h"

// �R���X�g���N�^
RigidBody::RigidBody()
{
    mass = 1.0f;           // ���ʁi�����l1.0�j
    drag = 0.0f;           // �����^���̋�C��R�i�����j�W��
    angularDrag = 0.05f;   // ��]�^���̋�C��R�i���g�p�j
    isGravity = true;      // �d�͂̉e�����󂯂邩�ǂ���
    isGrounded = false;    // �ڒn���Ă��邩�ǂ���
    position = VGet(0, 0, 0); // �����ʒu�i���_�j
    velocity = VGet(0, 0, 0); // �������x�i��~��ԁj
    force = VGet(0, 0, 0);    // ������ꂽ�́i�����͂Ȃ��j
}

// �͂�������֐�
void RigidBody::AddForce(VECTOR f)
{
    // ���݂̗͂ɐV���ȗ͂����Z����
    force = VAdd(force, f);
}

// �����v�Z�̍X�V�����i���t���[���Ăяo���j
void RigidBody::Update(float deltaTime)
{
    // ���ʂ�0�ȉ��Ȃ畨�����Z���X�L�b�v�i�ÓI�I�u�W�F�N�g�����j
    if (mass <= 0.0f) return;

    // ���݉������Ă���S�Ă̗͂����Z
    VECTOR totalForce = force;

    // �d�͂�K�p�iisGravity��true���󒆂ɂ���ꍇ�j
    if (isGravity && !isGrounded)
    {
        // �d�͉����x�i�n����Ȃ��9.81m/s^2�j�����ʕ������͂Ɋ��Z
        VECTOR gravity = VGet(0, -9.81f * mass, 0);
        totalForce = VAdd(totalForce, gravity);
    }

    // �����x = �� / ���ʁi�j���[�g���̑��@���j
    VECTOR acceleration = VScale(totalForce, 1.0f / mass);

    // ���ݑ��x�ɉ����x�~���Ԃ����Z���āA�V�������x���v�Z
    velocity = VAdd(velocity, VScale(acceleration, deltaTime));

    // ��C��R�iDrag�j��K�p�i���Ԃɔ�Ⴕ�đ��x�������j
    velocity = VScale(velocity, 1.0f / (1.0f + drag * deltaTime));

    // ���݈ʒu�ɑ��x�~���Ԃ����Z���āA�V�����ʒu���v�Z
    position = VAdd(position, VScale(velocity, deltaTime));

    // �t���[���̍Ō�ɉ�����ꂽ�͂����Z�b�g�i���t���[���֎����z���Ȃ��j
    force = VGet(0, 0, 0);
}