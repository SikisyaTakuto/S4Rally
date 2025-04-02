//�z�C�[���R���C�_�[�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include"DxLib.h"
#include "fps.h"
#include "geometory.h"
#include "controller.h"
#include "key.h"
#include "map.h"

// �쓮�����̒�`
enum DriveType {
    FWD, // �O�֋쓮
    RWD, // ��֋쓮
    AWD  // �l�֋쓮
};

//�\����
typedef struct _SuspensionSpring//�T�X�y���V�����X�v�����O�̏��
{
    float spring;           // �X�v�����O��
    float damper;           // ������
    float targetPosition;   // �T�X�y���V�����̎c�苗�� (0.0 �` 1.0)
} SuspensionSpring;

typedef struct _WheelCollider   // �z�C�[���R���C�_�[�̏��
{
    float mass;                        // �z�C�[���̎���
    float radius;                      // �z�C�[���̔��a
    float wheelDampingRate;            // �����l
    float suspensionDistance;          // �T�X�y���V�����̍ő勗��
    float forceAppPointDistance;       // �͂̓K�p�|�C���g
    VECTOR center;                     // �z�C�[���̒��S�ʒu
    VECTOR rotation;                   //�z�C�[���̉�]
    SuspensionSpring suspensionSpring; // �T�X�y���V�����X�v�����O
    float forwardFriction;             // �O�i���C
    float sidewaysFriction;            // ���������C

    VECTOR velocity;                   // ���݂̃z�C�[�����x
    float currentSuspensionCompression;// ���݂̃T�X�y���V�������k��

    VECTOR frontWheelPosition;         //�O�ցi2�ւ̒��ԁj�̈ʒu
    VECTOR frontWheelRotation;         //�O�ցi2�ւ̒��ԁj�̉�]
    VECTOR rearWheelPosition;          //��ցi2�ւ̒��ԁj�̈ʒu
    VECTOR rearWheelRotation;          //��ցi2�ւ̒��ԁj�̉�]

    int wheelModelHandleLeft;              // �^�C�����f���̃n���h��
    int wheelModelHandleRight;              // �^�C�����f���̃n���h��

} WheelCollider;

typedef struct _ForwardFriction//�O�㖀�C�̏��
{
    float forwardStiffness;             // ���C�W��
} ForwardFriction;

typedef struct _SidewaysFriction//���E���C�̏��
{
    float sidewaysStiffness;             // ���C�W��
} SidewaysFriction;

// �ԗ����
typedef struct _Car {
    WheelCollider frontLeft;
    WheelCollider frontRight;
    WheelCollider rearLeft;
    WheelCollider rearRight;
    DriveType driveType; // �쓮����
} Car;

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
    float drag = 0.1f;               // �ړ����̒�R

    int gear;                        // ���݂̃M�A (1-5)
    float currentSpeed;              // ���ݑ��x (m/s)
    float maxSpeed;                  // �ő呬�x (m/s)
    bool gearChangeLocked;           // �M�A�`�F���W�̃��b�N

    // �ǉ�����ϐ�
    float engineRPM;                 // �G���W����]�� (�����l 1000 RPM)
    float idleRPM;                   // �A�C�h�����O��]��
    float maxRPM;                    // �ő��]��
    float redlineRPM;                // ���b�h�]�[���J�n��]��
    float engineBrakeCoefficient;    // �G���W���u���[�L�̌���

    int carModelHandle;              // �ԑ̃��f���n���h��

} CarInfo;


//�O���O���[�o���ϐ�
extern WheelCollider wheelCollider;
extern SuspensionSpring suspensionSpring;
extern ForwardFriction forwardFriction;
extern SidewaysFriction sidewaysFriction;
extern Car car;
extern CarInfo carInfo;

//�O���v���g�^�C�v�錾
extern VOID CarInit(VOID);                              // �ԗ����̏�����
extern VOID CarUpdate(VOID);                            // �ԗ����̍X�V
extern VOID CarDraw(VOID);                              // �`��
extern VOID CarSetCamera(VOID);                         // �J�����ݒ�
extern FLOAT CarCulateEngineForce(int gear);            // �M�A�̕ύX
extern FLOAT CarCulateRPM(float velocity, int gear);    // RPM�̌v�Z�֐�
extern VOID CarApplySteering(VOID);                     // ���Ǌp�x�̐ݒ�
extern FLOAT CarApplyAcceleration(float deltaTime);     // �A�N�Z���̓K�p
extern FLOAT CarApplyBraking(float deltaTime);          // �u���[�L�̓K�p