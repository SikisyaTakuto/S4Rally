#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "car.h"
#include "suspension.h"

// �쓮�����̒�`
enum DriveType {
    FWD, // �O�֋쓮
    RWD, // ��֋쓮
    AWD  // �l�֋쓮
};

typedef struct _WheelCollider   // �z�C�[���R���C�_�[�̏��
{
    float mass;                        // �z�C�[���̎���
    float radius;                      // �z�C�[���̔��a
    float wheelDampingRate;            // �����l
    float suspensionDistance;          // �T�X�y���V�����̍ő勗��
    float forceAppPointDistance;       // �͂̓K�p�|�C���g
    VECTOR center;                     // �z�C�[���̒��S�ʒu
    VECTOR rotation;                   //�z�C�[���̉�]
    float forwardFriction;             // �O�i���C
    float sidewaysFriction;            // ���������C

    VECTOR velocity;                   // ���݂̃z�C�[�����x
    float currentSuspensionCompression;// ���݂̃T�X�y���V�������k��

    VECTOR frontWheelPosition;         //�O�ցi2�ւ̒��ԁj�̈ʒu
    VECTOR frontWheelRotation;         //�O�ցi2�ւ̒��ԁj�̉�]
    VECTOR rearWheelPosition;          //��ցi2�ւ̒��ԁj�̈ʒu
    VECTOR rearWheelRotation;          //��ցi2�ւ̒��ԁj�̉�]

    int wheelModelHandleLeft;          // �^�C�����f���̃n���h��
    int wheelModelHandleRight;         // �^�C�����f���̃n���h��

}WheelCollider;

typedef struct _ForwardFriction//�O�㖀�C�̏��
{
    float forwardStiffness;    // ���C�W��
}ForwardFriction;

typedef struct _SidewaysFriction//���E���C�̏��
{
    float sidewaysStiffness;    // ���C�W��
}SidewaysFriction;

// �ԗ����
typedef struct _Car {
    WheelCollider frontLeft;
    WheelCollider frontRight;
    WheelCollider rearLeft;
    WheelCollider rearRight;
    DriveType driveType;        // �쓮����
}Car;

//�O���O���[�o���ϐ�
extern Car car;
extern WheelCollider wheelCollider;
extern ForwardFriction forwardFriction;
extern SidewaysFriction sidewaysFriction;

//�O���v���g�^�C�v�錾
extern VOID CarApplySteering(VOID);// ���Ǌp�x�̐ݒ�
extern VOID ApplyVehicleDynamics(float deltaTime);
extern VOID ApplyFriction(float deltaTime);

