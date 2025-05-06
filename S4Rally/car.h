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

    void Init();  // ������
    void Update(float deltaTime); // �X�V
    void Draw();   // �`��

    bool CheckBodyCollision();

    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    VECTOR GetPosition() const { return carBodyPosition; }
    VECTOR GetRotation() const { return carBodyRotation; }

private:

    enum WheelPosition
    {
        FrontLeft,
        FrontRight,
        RearLeft,
        RearRight,
        WheelNum
    };

    int wheelModelHandles[WheelNum];    // �z�C�[�����f���n���h��
    VECTOR wheelPositions[WheelNum];    // �z�C�[���̈ʒu
    VECTOR wheelRotations[WheelNum];    // �z�C�[���̉�]

    VECTOR wheelOffsets[WheelNum]; // �ԑ̂���̃I�t�Z�b�g�ʒu�i���E�O��j

    float motorForce;      // �쓮��
    float steeringAngle;   // �n���h���p�x
    float accelInput;
    float steerInput;
    float sideBrakeInput;  // �T�C�h�u���[�L���́i�����Ă��1.0�A�����ĂȂ����0.0�j

    VECTOR frontStart;
    VECTOR frontEnd ;

    VECTOR backStart;
    VECTOR backEnd;

    VECTOR leftStart;
    VECTOR leftEnd;

    VECTOR rightStart;
    VECTOR rightEnd;

    bool hitFront;
    bool hitBack;
    bool hitLeft;
    bool hitRight;



    VECTOR frontStartDeceleration;
    VECTOR frontEndDeceleration;

    VECTOR backStartDeceleration;
    VECTOR backEndDeceleration;

    VECTOR leftStartDeceleration;
    VECTOR leftEndDeceleration;

    VECTOR rightStartDeceleration;
    VECTOR rightEndDeceleration;

    bool hitFrontDeceleration;
    bool hitBackDeceleration;
    bool hitLeftDeceleration;
    bool hitRightDeceleration;



    VECTOR boxVertices[8]; // �ԑ�Box�̒��_���X�g
    float boxHalfWidth = 100.0f;  // ���E���� ����
    float boxHalfLength = 160.0f; // �O����� ������
    float boxHeight = 50.0f;   // ����
};