//�z�C�[���R���C�_�[�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "fps.h"
#include "geometory.h"
#include "controller.h"
#include "key.h"
#include "map.h"
#include "camera.h"
#include "suspension.h"
#include "wheelcollider.h"
#include "engine.h"
#include "transmission.h"

// �ԗ����\����
typedef struct _CarInfo {
    float engineForse;               //�G���W���̗� (N)
    float ballast;                   // �o���X�g�d��
    float acceleration;              // �����x
    float brakeForce;                // �u���[�L��
    float turboMultiplier;           // �^�[�{�{��
    bool turboEnabled;               // �^�[�{�̗L����
    float steeringAngle;             // ���Ǌp�x
    float maxSteeringAngle;          // �ő呀�Ǌp�x
    float minSteeringAngle;          // �ŏ����Ǌp�x
    VECTOR position;                 // �ԗ��̈ʒu
    VECTOR rotation;                 // �ԗ��̉�]

    float mass = 1.0f;               // ����
    float drag = 0.1f;               // �ړ����̒�R]
    float currentSpeed;              // ���ݑ��x (m/s)
    float maxSpeed;                  // �ő呬�x (m/s)

    int carModelHandle;              // �ԑ̃��f���n���h��

} CarInfo;

extern CarInfo carInfo;

//�O���v���g�^�C�v�錾
extern VOID CarInit(VOID);                              // �ԗ����̏�����
extern VOID CarUpdate(VOID);                            // �ԗ����̍X�V
extern VOID CarDraw(VOID);                              // �`��
extern FLOAT CarCulateEngineForce(int gear);            // �M�A�̕ύX
extern FLOAT CarApplyAcceleration(float deltaTime);     // �A�N�Z���̓K�p
extern FLOAT CarApplyBraking(float deltaTime);          // �u���[�L�̓K�p