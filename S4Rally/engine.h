#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "car.h"

// �ԗ����\����
typedef struct _EngineInfo {
    float engineRPM;                 // �G���W����]�� (�����l 1000 RPM)
    float idleRPM;                   // �A�C�h�����O��]��
    float maxRPM;                    // �ő��]��
    float redlineRPM;                // ���b�h�]�[���J�n��]��
    float engineBrakeCoefficient;    // �G���W���u���[�L�̌���

    int carModelHandle;              // �ԑ̃��f���n���h��

}EngineInfo;

extern EngineInfo engineInfo;

//�O���v���g�^�C�v�錾
extern FLOAT CarCulateRPM(float velocity, int gear);    // RPM�̌v�Z�֐�
extern VOID UpdateEngineRPM(VOID);