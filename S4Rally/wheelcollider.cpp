//�w�b�_�t�@�C���ǂݍ���
#include "wheelcollider.h"

// �R���X�g���N�^�i�f�t�H���g�l��ݒ�j
WheelCollider::WheelCollider()
    : mass(20.0f), radius(0.35f), wheelDampingRate(1.0f), suspensionDistance(0.2f), forceAppPointDistance(0.0f), center({ 0, 0, 0 })
{
    // �O������̖��C�ݒ�i���j
    forwardFriction = { 1.0f, 1.0f, 2.0f, 0.5f, 1.0f };

    // �������̖��C�ݒ�i���j
    sidewaysFriction = { 1.0f, 1.0f, 2.0f, 0.5f, 1.0f };
}

// �f�X�g���N�^�i���ʂȌ㏈���͂Ȃ��j
WheelCollider::~WheelCollider()
{
    // �����ł͓��Ƀ��\�[�X����Ȃǂ͕s�v
}

// �n�ʂɑ΂��郌�C�L���X�g
bool WheelCollider::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal)
{
    extern Map map;

    // �^�C���̒��S���牺�ɐL�΂����C�Ɠ����ʒu�ŋ���u��
    VECTOR sphereCenter = VAdd(origin, VScale(direction, maxDistance * 0.5f));

    MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(map.modelHandle, -1, sphereCenter, radius);

    if (result.HitNum > 0) {
        //// �ł��߂��q�b�g���擾
        //MV1_COLL_RESULT_POLY hit = MV1CollResultPolyDimGetHitPolyDim(&result, 0);
        //hitPoint = hit.Position[0]; // �O�p�`��1���_�i�ߎ��j
        //hitNormal = hit.Normal;
        //return true;
    }

    return false;
}

// �T�X�y���V�����̒��ݍ��݌v�Z�ƒn�ʃq�b�g���擾
bool WheelCollider::GetGroundHit(const VECTOR& wheelPosition, VECTOR& hitPoint, VECTOR& hitNormal, float& suspensionCompression)
{
    VECTOR rayOrigin = VAdd(wheelPosition, center);  // �z�C�[���̒��S�ʒu
    VECTOR rayDirection = VGet(0, -1, 0);            // �^�������Ƀ��C����

    if (Raycast(rayOrigin, rayDirection, suspensionDistance + radius, hitPoint, hitNormal)) {
        // �z�C�[�����S����n�ʂ܂ł̋������v�Z
        float distanceToGround = VSize(VSub(rayOrigin, hitPoint));

        // �T�X�y���V�����̈��k�����v�Z�i0�`1�j
        suspensionCompression = 1.0f - (distanceToGround - radius) / suspensionDistance;
        suspensionCompression = Clamp(suspensionCompression, 0.0f, 1.0f);
        return true;
    }
    suspensionCompression = 0.0f;
    return false;
}

// ���C�͂��v�Z����
VECTOR WheelCollider::CalculateFrictionForce(const VECTOR& velocity, float suspensionCompression)
{
    VECTOR sideDir = VGet(1, 0, 0);    // �������i���̉E�����j
    VECTOR forwardDir = VGet(0, 0, 1); // �O�����i���̉������j

    // ���x��O��E���E�����ɕ���
    float forwardSpeed = VDot(velocity, forwardDir);
    float sidewaysSpeed = VDot(velocity, sideDir);

    // �O������̖��C��
    float forwardForce = -forwardSpeed * forwardFriction.stiffness * suspensionCompression;

    // �������̖��C��
    float sidewaysForce = -sidewaysSpeed * sidewaysFriction.stiffness * suspensionCompression;

    // ���C�̓x�N�g��������
    VECTOR friction = VAdd(VScale(forwardDir, forwardForce), VScale(sideDir, sidewaysForce));
    return friction;
}

// Clamp�֐��F�l��[min, max]�͈̔͂Ɏ��߂�
float WheelCollider::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
