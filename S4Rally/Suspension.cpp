#include "suspension.h"

//�O���O���[�o���ϐ�
SuspensionSpring suspensionSpring;

// �w���p�[�֐��FY�����Ƀx�N�g������]�i�p�x�̓��W�A���j
VECTOR RotateVector(const VECTOR& vec, float angleRad) {
    VECTOR result;
    result.x = vec.x * cosf(angleRad) - vec.z * sinf(angleRad);
    result.y = vec.y;
    result.z = vec.x * sinf(angleRad) + vec.z * cosf(angleRad);
    return result;
}

// �o�C�V�N�����f���Ɋ�Â��ԗ��^���̍X�V
// �O�ւ̃X�e�A�����O�p (carInfo.steeringAngle) �ƌ��݂̑��x����A�ԑ̂̌����ƈʒu�A�e�z�C�[���̐�Έʒu���X�V���܂��B
VOID CarBicycleModelUpdate(float deltaTime)
{
    // CarInit �Őݒ肵���l�Ɋ�Â��z�C�[���z�u�p�����[�^
    const float wheelSpacingX = 150.2f;       // �����i���E�̃z�C�[���ԋ����j
    const float wheelSpacingZ = 250.5f;       // �c���i�O��̃z�C�[���ԋ����j
    float halfTrack = wheelSpacingX / 2.0f;   // �ԗ����E����
    float frontOffset = wheelSpacingZ / 1.7f;   // �O�֎��܂ł̋���
    float rearOffset = -wheelSpacingZ / 2.4f;  // ��֎��܂ł̋���

    // �z�C�[���x�[�X�i�O�֎��ƌ�֎��̋����j
    float wheelBase = frontOffset - rearOffset; // L

    // �O�ւ̃X�e�A�����O�p�i�x�����W�A���j
    float steeringRad = carInfo.steeringAngle/* * (DX_PI / 180.0f)*/;

    // ���݂̎ԑ̌����iyaw�FcarInfo.rotation.y ��x�����W�A���ɕϊ��j
    float currentHeading = car.frontLeft.rotation.y * (DX_PI / 180.0f);

    // �p���x �� = v / L * tan(��)
    float angularVelocity = (fabsf(steeringRad) > 0.0001f) ? (carInfo.currentSpeed / wheelBase * tanf(steeringRad)) : 0.0f;

    // �ԑ̂̌������X�V
    float newHeading = currentHeading + angularVelocity * deltaTime;
    VECTOR direction = VSub(car.frontRight.center, car.rearRight.center); // �O�ւƌ�ւ̕����x�N�g��
    carInfo.rotation.y = atan2f(direction.x, direction.z); // �ԑ̂̉�]�p�x

    // �ԑ̂̒��S�ʒu���X�V�i�i�s�����Ɉړ��j
    carInfo.position.x += carInfo.currentSpeed * sinf(-newHeading);
    carInfo.position.z += carInfo.currentSpeed * cosf(newHeading);

    // �e�z�C�[���̃��[�J���I�t�Z�b�g�����Έʒu���v�Z�i�ԑ̂̌��� newHeading �ɍ��킹�ĉ�]�j
    VECTOR localOffset;
    // �O�֍�
    localOffset = VGet(-halfTrack, 0.0f, frontOffset);
    car.frontLeft.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // �O�։E
    localOffset = VGet(halfTrack, 0.0f, frontOffset);
    car.frontRight.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // ��֍�
    localOffset = VGet(-halfTrack, 0.0f, rearOffset);
    car.rearLeft.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // ��։E
    localOffset = VGet(halfTrack, 0.0f, rearOffset);
    car.rearRight.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));

    car.rearLeft.rotation.y = carInfo.rotation.y;
    car.rearRight.rotation.y = carInfo.rotation.y;

}

//�T�X�y���V�����̏���
VOID ApplySuspension(WheelCollider wheel, VECTOR hitPos, float deltaTime) {
    // �d�͉����x (m/s^2)
    const float gravity = 9.81f;

    // �T�X�y���V�����̏�������
    float restLength = wheel.suspensionDistance;

    // �T�X�y���V�������L�т�ő勗�� (�K�X����)
    float maxExtension = 1.0f;

    // �T�X�y���V�����ɂ��͂̏�����
    float springForce = 0.0f;
    float damperForce = 0.0f;

    // �ԑ̂̍Œ�n�㍂ (�^�C�����a + �]�T)
    float minHeight = wheel.radius + 0.5f;

    // �Ԃ̏d�ʂɊ�Â��׏d���v�Z (�e�z�C�[���ɂ�����d��)
    float wheelLoad = (carInfo.mass * gravity) / 4.0f;

    // �n�ʂƂ̋������擾
    float groundHeight = hitPos.y;

    // �z�C�[���̌��݂�Y���W
    float wheelPosY = wheel.center.y;

    // �T�X�y���V�����̈��k�ʂ��v�Z
    float compression = restLength - (wheelPosY - groundHeight);

    // �T�X�y���V�����̈��k�ʂ� 0 ���傫���ꍇ�̂ݗ͂�K�p
    if (compression > 0.0f) {
        // �t�b�N�̖@���Ɋ�Â��X�v�����O�� F = -k * x (k: �o�l�萔, x: ���k��)
        springForce = -suspensionSpring.spring * compression;

        // ������ F_d = -c * v (c: �����W��, v: �z�C�[����Y�����x)
        damperForce = -suspensionSpring.damper * wheel.velocity.y;

        // �����I�ȗ͂̌v�Z (�o�l�� + ������ + �z�C�[���ɂ�����׏d)
        float totalForce = springForce + damperForce + wheelLoad;

        // �T�X�y���V�����̗͂�K�p (�ԑ̂��㉺�ɓ�����)
        wheel.center.y += (totalForce / carInfo.mass) * deltaTime;
        carInfo.position.y += (totalForce / carInfo.mass) * deltaTime;
    }

    // �T�X�y���V�����̍ő�L�т𒴂��Ȃ��悤�ɐ���
    float maxWheelPosY = groundHeight + restLength + maxExtension;
    if (wheel.center.y > maxWheelPosY) {
        wheel.center.y = maxWheelPosY; // �ő�ʒu�𒴂����琧��
    }
    else {
        // �ߓx�ɒ��ݍ��܂Ȃ��悤�� 0.01f �������� (�T�X�y���V�����̃o�E���h���Č�)
        wheel.center.y -= 0.01f;

        // �z�C�[����Y���W��0�����ɂȂ�Ȃ��悤�ɐ���
        if (wheel.center.y < 0.0f) wheel.center.y = 0.0f;
    }

    // �ԑ̂̍Œፂ�����v�Z (4�ւ̐ڒn�ʂ̕���)
    float carMinHeight = (
        (car.frontLeft.center.y - wheel.radius) +
        (car.frontRight.center.y - wheel.radius) +
        (car.rearLeft.center.y - wheel.radius) +
        (car.rearRight.center.y - wheel.radius)
        ) / 4.0f;

    // �ԑ̂��Œ�n�㍂�������Ȃ��悤�ɒ���
    if (carInfo.position.y < carMinHeight + minHeight) {
        carInfo.position.y = carMinHeight + minHeight;
    }

}

// �ԑ̂̃s�b�`�p�ƃ��[���p���X�V
VOID UpdateCarPitchAndRoll()
{
    // �e�z�C�[���̍���
    float frontLeftHeight = car.frontLeft.center.y;
    float frontRightHeight = car.frontRight.center.y;
    float rearLeftHeight = car.rearLeft.center.y;
    float rearRightHeight = car.rearRight.center.y;

    // �ԗ��̑O��z�C�[���̕��ύ���
    float frontAvgHeight = (frontLeftHeight + frontRightHeight) * 0.5f;
    float rearAvgHeight = (rearLeftHeight + rearRightHeight) * 0.5f;

    // �ԗ��̍��E�z�C�[���̕��ύ���
    float leftAvgHeight = (frontLeftHeight + rearLeftHeight) * 0.5f;
    float rightAvgHeight = (frontRightHeight + rearRightHeight) * 0.5f;

    // �z�C�[���x�[�X�i�O��z�C�[���Ԃ̋����j
    float wheelBase = 250.5f; // ���̒l�i�K�v�ɉ����ĕύX�j

    // �g���b�h���i���E�z�C�[���Ԃ̋����j
    float trackWidth = 150.2f; // ���̒l�i�K�v�ɉ����ĕύX�j

    // �s�b�`�p�̌v�Z�i�O��z�C�[���̍���������j
    float pitchAngle = atan2f(-frontAvgHeight + rearAvgHeight, wheelBase);

    // ���[���p�̌v�Z�i���E�z�C�[���̍���������j
    float rollAngle = atan2f(leftAvgHeight - rightAvgHeight, trackWidth);

    // �ԑ̂̉�]�ɔ��f
    carInfo.rotation.x = pitchAngle; // �s�b�`�iX����]�j
    carInfo.rotation.z = rollAngle;  // ���[���iZ����]�j
}

VOID SuspensionDrow(float deltaTime)
{
    // �e�z�C�[���ɃT�X�y���V������K�p
    ApplySuspension(car.frontLeft, stage.frontLeftHitPos, deltaTime);
    ApplySuspension(car.frontRight, stage.frontRightHitPos, deltaTime);
    ApplySuspension(car.rearLeft, stage.rearLeftHitPos, deltaTime);
    ApplySuspension(car.rearRight, stage.rearRightHitPos, deltaTime);

}