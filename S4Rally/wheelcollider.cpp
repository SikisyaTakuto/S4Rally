//�w�b�_�t�@�C���ǂݍ���
#include "wheelcollider.h"

// �R���X�g���N�^�i�f�t�H���g�l��ݒ�j
WheelCollider::WheelCollider()
    : mass(20.0f), radius(35.0f), wheelDampingRate(1.0f), suspensionDistance(0.2f), forceAppPointDistance(0.0f), center({ 0, 0, 0 })
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

        return true; // �������Ă���
    }

    return false;
}

bool WheelCollider::GetGroundHit(const VECTOR& wheelPos, VECTOR& hitPos, VECTOR& hitNormal, float& compression)
{
    extern Map map;

    MV1_COLL_RESULT_POLY_DIM hitResult;

    // �ԗւ̔��a�i�K���ɐݒ�j
    const float wheelRadius = 18.0f;

    // �}�b�v�̃��f���n���h�����擾
    int mapHandle = map.modelHandle;

    // �ڐG����i���̂ƃ}�b�v�j
    hitResult = MV1CollCheck_Sphere(mapHandle, -1, wheelPos, wheelRadius);

    if (hitResult.HitNum > 0) {
        // �ŏ��Ƀq�b�g�����|���S���̏����擾
        hitPos = hitResult.Dim[0].Position[0];  // 0�Ԗڂ̒��_
        hitNormal = hitResult.Dim[0].Normal;

        // ���k���i�T�X�y���V�����̒��݋�j���v�Z
        float distance = wheelPos.y - hitPos.y;
        compression = distance / wheelRadius;
        return true;
    }

    // �ڒn���ĂȂ��ꍇ
    compression = 1.0f;
    return false;
}

// ���C�͂��v�Z����
VECTOR WheelCollider::CarCulateFrictionForce(const VECTOR& velocity, float suspensionCompression, float sideBrakeInput)
{
    extern Car car;

    RigidBody rigidBody;

    VECTOR frictionForce = VGet(0, 0, 0);

    // �n�ʂɐڒn���Ă��Ȃ���Ζ��C�͂Ȃ�
    if (rigidBody.isGrounded) {
        return frictionForce;
    }

    // --- ���x�̐��������i�O������A���E�����j
    VECTOR forward = VNorm(VGet(sinf(car.carBodyRotation.y), 0.0f, cosf(car.carBodyRotation.y)));  // �Ԃ̑O����
    VECTOR right = VCross(VGet(0, 1, 0), forward); // �Ԃ̉E����

    float forwardSpeed = VDot(velocity, forward);  // �O�㐬��
    float sidewaysSpeed = VDot(velocity, right);   // ������

    // --- �c�����i�O������j���C�i�G���W���u���[�L�I�j
    const float rollingResistanceCoef = 0.015f; // �]�����R�W��
    VECTOR rollingResistance = VScale(forward, -forwardSpeed * rollingResistanceCoef);

    // --- ���������C�i�T�C�h�u���[�L���ɑ傫������j
    float lateralFrictionCoef = 0.5f; // �ʏ펞�̉����C�W��
    if (sideBrakeInput > 0.5f) {
        lateralFrictionCoef = 2.0f; // �T�C�h�u���[�L���͉����苭�߂�
    }
    VECTOR lateralFriction = VScale(right, -sidewaysSpeed * lateralFrictionCoef);

    // --- ���Z
    frictionForce = VAdd(rollingResistance, lateralFriction);

    return frictionForce;
}

// Clamp�֐��F�l��[min, max]�͈̔͂Ɏ��߂�
float WheelCollider::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

//�^�C���ʒu�ɋ���`�悷��֐���ǉ�
void WheelCollider::Draw(const VECTOR& wheelPosition)
{
    // �^�C���̒��S�{�I�t�Z�b�g
    VECTOR spherePos = VAdd(wheelPosition, center);

    // �����݂̂ŋ���`���i�h��Ԃ��Ȃ��j
    DrawSphere3D(
        spherePos,       // ���̒��S���W
        radius,          // ���̔��a
        10,              // �ܐ����i�ׂ����j
        10,              // �o�����i�ׂ����j
        GetColor(255, 0, 0),  // �F�i�ԐF�j
        false            // �h��Ԃ��Ȃ�
    );
}