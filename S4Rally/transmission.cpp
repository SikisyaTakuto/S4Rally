#include "transmission.h"

TransmissionInfo transmissionInfo;

// RPM�̌v�Z�֐�
FLOAT  CarCulateRPM(float velocity, int gear) {
    const float finalDriveRatio = 4.1f; // �f�t�@�����V������

    // �G���W����]�� (RPM) = (���x / �^�C�����a) * �M�A�� * �f�t�� * 60 / (2��)
    float rpm = (velocity / wheelCollider.radius) * transmissionInfo.gearRatios[gear - 1] * finalDriveRatio * 60.0f / (2.0f * DX_PI);

    return rpm;
}

// �M�A�ɂ��G���W���͂̕ω�
FLOAT CarCulateEngineForce(int gear) {
    // �M�A��ɂ���ăG���W���͂𒲐�
    return carInfo.engineForse * transmissionInfo.gearRatios[gear - 1];
}

// �M�A�ύX����
VOID ChangeGear(VOID) {
    // �M�A�ύX�����b�N����Ă��Ȃ��ꍇ�̂ݏ������s��
    if (!transmissionInfo.gearChangeLocked) {
        // X�{�^����������A���݂̃M�A���ő�M�A�����ł���΃V�t�g�A�b�v
        if (GetButton(BUTTON_X) && transmissionInfo.gear < 5) {
            transmissionInfo.gear++; // �M�A��1�グ��
            transmissionInfo.gearChangeLocked = true; // �A�����͂�h�����߂Ƀ��b�N
        }
        // A�{�^����������A���݂̃M�A��1���傫����΃V�t�g�_�E��
        if (GetButton(BUTTON_A) && transmissionInfo.gear > 1) {
            transmissionInfo.gear--; // �M�A��1������
            transmissionInfo.gearChangeLocked = true; // �A�����͂�h�����߂Ƀ��b�N
        }
    }

    // X�{�^����A�{�^���̗�����������Ă��Ȃ��ꍇ�A�M�A�ύX�̃��b�N������
    if ((!GetButton(BUTTON_X)) && (!GetButton(BUTTON_A))) {
        transmissionInfo.gearChangeLocked = false; // �L�[�𗣂��������
    }
}