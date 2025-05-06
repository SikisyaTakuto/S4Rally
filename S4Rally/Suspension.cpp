//�w�b�_�t�@�C���ǂݍ���
#include "suspension.h"

// �R���X�g���N�^�F�e�����o�ϐ���������
SuspensionSpring::SuspensionSpring()
    : spring(35000.0f),    // �X�v�����O�萔�iN/m�j�F�X�v�����O�̍d����\��
    damper(4500.0f),     // �����W���iNs/m�j�F�����̋�����\��
    targetPosition(0.5f),// �ڕW�ʒu�i0.0 �` 1.0�j�F�T�X�y���V�����̒��ԓ_
    restLength(0.3f),    // ���R���im�j�F�X�v�����O���͂��󂯂Ă��Ȃ��Ƃ��̒���
    currentLength(0.3f), // ���݂̒����im�j�F������Ԃł͎��R���Ɠ���
    velocity(0.0f)       // �L�k���x�im/s�j�F������Ԃł͐Î~
{}

// �f�X�g���N�^�F���ʂȏ����͕s�v
SuspensionSpring::~SuspensionSpring() {}

// �T�X�y���V�����ɓ����͂��v�Z����֐�
// deltaTime: �O��̍X�V����̌o�ߎ��ԁi�b�j
// newLength: �V�����X�v�����O�̒����im�j
float SuspensionSpring::CarCulateForce(float deltaTime, float newLength) {
    // �ڕW�ʒu����̕ψʂ��v�Z
    float displacement = restLength * targetPosition - newLength;

    // �t�b�N�̖@���Ɋ�Â��X�v�����O�͂��v�Z
    float springForce = spring * displacement;

    // �����͂��v�Z�i���x�ɔ��j
    float dampingForce = damper * ((newLength - currentLength) / deltaTime);

    // ���݂̒������X�V
    currentLength = newLength;

    // ���́i�X�v�����O�� - �����́j��Ԃ�
    return springForce - dampingForce;
}