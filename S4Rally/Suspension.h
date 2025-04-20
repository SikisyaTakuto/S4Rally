#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "wheelcollider.h"
#include "fps.h"
#include "car.h"
#include "map.h"

//�\����
typedef struct _SuspensionSpring//�T�X�y���V�����X�v�����O�̏��
{
    float spring;           // �X�v�����O��
    float damper;           // ������
    float targetPosition;   // �T�X�y���V�����̎c�苗�� (0.0 �` 1.0)
}SuspensionSpring;

//�O���O���[�o���ϐ�
extern SuspensionSpring suspensionSpring;

//�O���v���g�^�C�v�錾
extern VOID CarBicycleModelUpdate(float deltaTime);                             
//extern VOID ApplySuspension(WheelCollider wheel, VECTOR hitPos, float deltaTime);
extern VOID UpdateCarPitchAndRoll(VOID);               
extern VOID SuspensionDrow(float deltaTime);