// Car.h
#pragma once

// �w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "fps.h"
#include "geometory.h"
#include "controller.h"
#include "key.h"
#include "map.h"
#include "camera.h"
#include "suspension.h"
#include "wheelcollider.h"
#include "rigidbody.h"
#include <type_traits>
#include <algorithm>
#include <vector> 

class Car
{
public:

    int carModelHandle;        // �ԑ̃��f���n���h��
    VECTOR carBodyPosition;    // �ԑ̂̈ʒu
    VECTOR carBodyRotation;    // �ԑ̂̉�]

    Car(); // �R���X�g���N�^
    ~Car(); // �f�X�g���N�^

    void Init();  // �ԗ��̏������֐�
    void Update(float deltaTime); // ���t���[���̍X�V����
    void Draw();   // �ԗ����f���̕`�揈��

    // �ԑ̂̃R���W�����`�F�b�N�i�ǂȂǂɏՓ˂��Ă��邩����j
    bool CheckBodyCollision();

    // ���C�L���X�g���΂��ďՓ˔�����s��
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    // �ԑ̂̌��݈ʒu���擾
    VECTOR GetPosition() const { return carBodyPosition; }

    // �ԑ̂̌��݉�]���擾
    VECTOR GetRotation() const { return carBodyRotation; }

private:

    // �z�C�[���̈ʒu���
    enum WheelPosition
    {
        FrontLeft,   // �O��
        FrontRight,  // �O�E
        RearLeft,    // �㍶
        RearRight,   // ��E
        WheelNum     // �z�C�[����
    };

    int wheelModelHandles[WheelNum];    // �e�z�C�[�����f���̃n���h��
    VECTOR wheelPositions[WheelNum];    // �e�z�C�[���̃��[���h���W�ʒu
    VECTOR wheelRotations[WheelNum];    // �e�z�C�[���̉�]�i�I�C���[�p�j

    VECTOR wheelOffsets[WheelNum];      // �ԑ̒��S����e�z�C�[���܂ł̃I�t�Z�b�g�ʒu

    float motorForce;       // �G���W���̋쓮�́i�A�N�Z���ɂ��́j
    float steeringAngle;    // ���݂̃X�e�A�����O�p�x
    float accelInput;       // �A�N�Z�����͒l�i�ʏ�0.0?1.0�j
    float steerInput;       // �X�e�A�����O���͒l�i-1.0?1.0�j
    float sideBrakeInput;   // �T�C�h�u���[�L���͒l�i0.0 or 1.0�j

    // �ԑ̑O�ʂ̎n�_�E�I�_�i���C�L���X�g�p�j
    VECTOR frontStart;
    VECTOR frontEnd;

    // �ԑ̌�ʂ̎n�_�E�I�_�i���C�L���X�g�p�j
    VECTOR backStart;
    VECTOR backEnd;

    // �ԑ̍��ʂ̎n�_�E�I�_�i���C�L���X�g�p�j
    VECTOR leftStart;
    VECTOR leftEnd;

    // �ԑ̉E�ʂ̎n�_�E�I�_�i���C�L���X�g�p�j
    VECTOR rightStart;
    VECTOR rightEnd;

    // �e�����ւ̏Փˌ��m�t���O
    bool hitFront;
    bool hitBack;
    bool hitLeft;
    bool hitRight;

    // �����p���C�L���X�g�̎n�_�E�I�_�i�O���j
    VECTOR frontStartDeceleration;
    VECTOR frontEndDeceleration;

    // �����p���C�L���X�g�̎n�_�E�I�_�i����j
    VECTOR backStartDeceleration;
    VECTOR backEndDeceleration;

    // �����p���C�L���X�g�̎n�_�E�I�_�i�����j
    VECTOR leftStartDeceleration;
    VECTOR leftEndDeceleration;

    // �����p���C�L���X�g�̎n�_�E�I�_�i�E���j
    VECTOR rightStartDeceleration;
    VECTOR rightEndDeceleration;

    // ��������p�̃q�b�g�t���O
    bool hitFrontDeceleration;
    bool hitBackDeceleration;
    bool hitLeftDeceleration;
    bool hitRightDeceleration;

    // �ԑ̂̃o�E���f�B���O�{�b�N�X�̒��_���X�g�i8���_�j
    VECTOR boxVertices[8];

    // �ԑ̂̃o�E���f�B���O�{�b�N�X�T�C�Y�ݒ�
    float boxHalfWidth = 100.0f;   // �����i���E�����j
    float boxHalfLength = 160.0f;  // �������i�O������j
    float boxHeight = 50.0f;       // �����i�㉺�����j
};