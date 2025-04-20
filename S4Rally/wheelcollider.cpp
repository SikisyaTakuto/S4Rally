#include "wheelcollider.h"

//�O���O���[�o���ϐ�
WheelCollider wheelCollider;
ForwardFriction forwardFriction;
SidewaysFriction sidewaysFriction;
Car car;

//���Ǌp�x�̐ݒ�
VOID CarApplySteering(VOID)
{
    // �t���[���Ԃ̎�
    float deltaTime = fps.Deltatime;

    // �X�e�B�b�N���͂̎擾�i���E�j
    float axisX = GetAxisX();  // -1 (��) �` 1 (�E)

    // �L�[���͂ŃX�e�A�����O�p�x��ύX
    if ((CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) && !stage.lockLeft) {
        carInfo.steeringAngle += carInfo.minSteeringAngle * deltaTime; // ����]
        // �ŏ��p�x�𑝉��A�ő�p�x������
        carInfo.minSteeringAngle -= 50.0f * deltaTime; // ���X�ɍŏ��p�x�𑝉�
        carInfo.maxSteeringAngle -= 50.0f * deltaTime; // ���X�ɍő�p�x������
    }

    if ((CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) && !stage.lockRight) {
        carInfo.steeringAngle += carInfo.maxSteeringAngle * deltaTime; // �E��]
        // �ŏ��p�x�������A�ő�p�x�𑝉�
        carInfo.minSteeringAngle += 50.0f * deltaTime; // ���X�ɍŏ��p�x������
        carInfo.maxSteeringAngle += 50.0f * deltaTime; // ���X�ɍő�p�x�𑝉�
    }

    // �X�e�A�����O�p�x�̐��� 
    float maxSteeringRad = carInfo.maxSteeringAngle * (DX_PI / 180.0f);
    float minSteeringRad = carInfo.minSteeringAngle * (DX_PI / 180.0f);

    float steeringAngleRad = carInfo.steeringAngle * (DX_PI / 180.0f);

    if (steeringAngleRad > maxSteeringRad) {
        steeringAngleRad = maxSteeringRad;
    }
    if (steeringAngleRad < minSteeringRad) {
        steeringAngleRad = minSteeringRad;
    }

    carInfo.steeringAngle = steeringAngleRad * (180.0f / DX_PI); // ���W�A������x�ɕϊ�

    //�ԑ̂���������猳�ɖ߂�
    if (carInfo.steeringAngle <= -360 || carInfo.steeringAngle >= 360)
    {
        carInfo.steeringAngle = 0;
        carInfo.minSteeringAngle = -50.0f;
        carInfo.maxSteeringAngle = 50.0f;
    }

    // �O�ւ̊p�x���X�V
    car.frontLeft.rotation.y = carInfo.steeringAngle;
    car.frontRight.rotation.y = carInfo.steeringAngle;

}

// ���[���[�����g�ƃX���b�v�A���O�����l��
VOID ApplyVehicleDynamics(float deltaTime)
{
    float wheelBase = 2.5f; // �z�C�[���x�[�X�i���l�j
    float yawMoment = 0.0f;

    // �X�e�B�b�N���͂̎擾�i���E�j
    float axisX = GetAxisX();  // -1 (��) �` 1 (�E)

    // �ߋ��̃X�e�A�����O���͂�ۑ�
    static float prevSteering = 0.0f;
    static bool flickInitiated = false; // �t���b�N�J�n����
    static float flickTimer = 0.0f; // �t���b�N�̎�������

    // �X���b�v�A���O���̌v�Z
    float slipAngleFront = atan2(carInfo.currentSpeed * sin(carInfo.steeringAngle * (DX_PI / 180.0f)), carInfo.currentSpeed);
    float slipAngleRear = atan2(carInfo.currentSpeed * sin(-carInfo.steeringAngle * (DX_PI / 180.0f)), carInfo.currentSpeed);

    // �^�C���̉����C�ɂ�郈�[���[�����g
    float frontLateralForce = -slipAngleFront * 2000.0f; // ���̒l
    float rearLateralForce = -slipAngleRear * 1800.0f; // ���̒l

    // �ԑ̂̉�]�ɉe��
    yawMoment = (frontLateralForce * (wheelBase / 2.0f)) - (rearLateralForce * (wheelBase / 2.0f));

    // �p���x�̓K�p�i�����l���j
    float angularAcceleration = yawMoment / carInfo.mass;
    wheelCollider.rearWheelRotation.y += angularAcceleration * deltaTime;

    // ���݂̃X�e�A�����O��ۑ�
    prevSteering = axisX;
}

// ���C��K�p����֐�
VOID ApplyFriction(float deltaTime)
{
    float throttle = CarApplyAcceleration(deltaTime);
    float brake = CarApplyBraking(deltaTime);

    // ���C�̓K�p
    float baseForwardFriction = wheelCollider.forwardFriction;
    float baseSidewaysFriction = wheelCollider.sidewaysFriction;

    if (throttle > 0.1f) // �A�N�Z�����͖��C�����炷
    {
        car.frontLeft.forwardFriction = baseForwardFriction * 0.9f;
        car.frontRight.forwardFriction = baseForwardFriction * 0.9f;
        car.rearLeft.forwardFriction = baseForwardFriction * 0.8f;
        car.rearRight.forwardFriction = baseForwardFriction * 0.8f;
    }
    else if (brake > 0.1f) // �u���[�L���͖��C�𑝂₷
    {
        car.frontLeft.forwardFriction = baseForwardFriction * 1.2f;
        car.frontRight.forwardFriction = baseForwardFriction * 1.2f;
        car.rearLeft.forwardFriction = baseForwardFriction * 1.5f;
        car.rearRight.forwardFriction = baseForwardFriction * 1.5f;
    }
    else // �A�N�Z���I�t���i�G���W���u���[�L�j
    {
        car.frontLeft.forwardFriction += (baseForwardFriction - car.frontLeft.forwardFriction) * deltaTime;
        car.frontRight.forwardFriction += (baseForwardFriction - car.frontRight.forwardFriction) * deltaTime;
        car.rearLeft.forwardFriction += (baseForwardFriction - car.rearLeft.forwardFriction) * deltaTime;
        car.rearRight.forwardFriction += (baseForwardFriction - car.rearRight.forwardFriction) * deltaTime;
    }

    // �������̖��C���X�e�A�����O�p�x�ɂ���Ē���
    float steeringFactor = fabsf(carInfo.steeringAngle) / (carInfo.maxSteeringAngle + carInfo.minSteeringAngle); // 0 �` 1
    float driftFriction = wheelCollider.sidewaysFriction * (1.0f - 0.5f * steeringFactor); // �X�e�A�p���傫���قǖ��C����

    car.frontLeft.sidewaysFriction = driftFriction;
    car.frontRight.sidewaysFriction = driftFriction;
    car.rearLeft.sidewaysFriction = driftFriction;
    car.rearRight.sidewaysFriction = driftFriction;

}