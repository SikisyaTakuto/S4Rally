#include "camera.h"

void Camera::Init()
{
    SetCameraNearFar(1.0f, 10000.0f);
}

void Camera::Update()
{
    extern Car car; // �O���ϐ��Ƃ��ĎԂ̏����擾

    // �Ԃ̈ʒu�Ɖ�]���擾
    VECTOR carPos = car.GetPosition();
    VECTOR carRot = car.GetRotation();

    // �Ԃ̌��������v�Z
    VECTOR backDir = VGet(-sinf(carRot.y), 0.0f, -cosf(carRot.y));
    backDir = VNorm(backDir); // ���K���i�����̂݁j

    // �J�����ƎԂ̋����A�J�����̍���
    float distance = 500.0f;
    float heightOffset = 300.0f;

    // �J�����̈ʒu���v�Z�i�Ԃ̌��ɔz�u�j
    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    cameraPos.y += heightOffset; // �����̒���

    // �����_���Ԃ̏�����ɐݒ�
    VECTOR target = carPos;
    target.y += 50.0f;  // �Ԃ̏�̕��ɒ����_����������

    // �J�����̈ʒu�ƒ����_��ݒ�
    SetCameraPositionAndTarget_UpVecY(cameraPos, target);
}