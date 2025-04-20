#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "suspension.h"
#include "wheelcollider.h"
#include "engine.h"
#include "car.h"

// �ԗ����\����
typedef struct _TransmissionInfo {

    int gear;               // ���݂̃M�A (1-5)
    int maxGear;
    bool gearChangeLocked;  // �M�A�`�F���W�̃��b�N
    float gearRatios[8];     // �M�A��
    float maxSpeeds[8];      // �e�M�A�̍ő呬�x (m/s)

} TransmissionInfo;

extern TransmissionInfo transmissionInfo;

extern FLOAT  CarCulateRPM(float velocity, int gear);
extern FLOAT CarCulateEngineForce(int gear);
extern VOID ChangeGear(VOID);