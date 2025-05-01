//wheelCollider�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "map.h"
#include "car.h"

class WheelCollider {
public:
    // ��{�v���p�e�B
    float mass;                  //�z�C�[���̎���
    float radius;                //�z�C�[���̔��a
    float wheelDampingRate;      //�z�C�[���ɓK�p����錸���l
    float suspensionDistance;    //�z�C�[���T�X�y���V�����̍ő剄������
    float forceAppPointDistance; //�z�C�[���̗͂��K�p�����ʒu���`
    VECTOR center;               //�I�u�W�F�N�g�̃z�C�[���̒��S�B

    /// <summary>
    /// �z�C�[�����O�]����ۂ̃^�C���̖��C�̃v���p�e�B
    /// </summary>
    struct ForwardFriction {
        float extremumSlip;    //�Ȑ��̋ɒl�_   
        float extremumValue;   //�Ȑ��̋ɒl�_   
        float asymptoteSlip;   //���C�Ȑ��̑Q�ߐ��̃X���b�v�l
        float asymptoteValue;  //���C�Ȑ��̑Q�ߐ��̃t�H�[�X�l
        float stiffness;       //Extremum Value �� Asymptote Value �ɑ΂���搔 (�f�t�H���g�� 1)�B 0�ɐݒ肷��ƁA�z�C�[������̂��ׂĂ̖��C�����S�ɖ���
    }forwardFriction;
    
    /// <summary>
    /// �z�C�[�������]����ۂ̃^�C���̖��C�̃v���p�e�B
    /// </summary>
    struct SidewaysFriction {
        float extremumSlip;    //�Ȑ��̋ɒl�_          
        float extremumValue;   //�Ȑ��̋ɒl�_          
        float asymptoteSlip;   //���C�Ȑ��̑Q�ߐ��̃X���b�v�l
        float asymptoteValue;  //���C�Ȑ��̑Q�ߐ��̃t�H�[�X�l
        float stiffness;       //Extremum Value �� Asymptote Value �ɑ΂���搔 (�f�t�H���g�� 1)�B 0�ɐݒ肷��ƁA�z�C�[������̂��ׂĂ̖��C�����S�ɖ���
    }sidewaysFriction;


    // �R���X�g���N�^�E�f�X�g���N�^ 
    WheelCollider();   // ������
    ~WheelCollider();  // �I�������i����͓��Ƀ��\�[�X�J���͕s�v�j

    // �T�X�y���V�����v�Z�{�n�ʂƂ̐ڐG
    bool GetGroundHit(const VECTOR& wheelPosition, VECTOR& hitPoint, VECTOR& hitNormal, float& suspensionCompression);

    // ���C�͌v�Z
    VECTOR CarCulateFrictionForce(const VECTOR& velocity, float suspensionCompression, float sideBrakeInput);

private:
    // �n�ʂƂ̔���(�ȈՔ�: �n��Y=0�Œ�j
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    // Clamp�֐��F�l��[min, max]�͈̔͂Ɏ��߂�
    float Clamp(float value, float min, float max);
};