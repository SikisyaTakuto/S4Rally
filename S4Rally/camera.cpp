#include "camera.h"

void Camera::Init()
{
    SetCameraNearFar(1.0f, 10000.0f);
}

void Camera::Update()
{
    extern Car car; // �O���ϐ�

    // �ԑ̂̈ʒu�Ɖ�]���擾
    VECTOR carPos = car.GetPosition();       // �Ԃ̈ʒu
    VECTOR carRot = car.GetRotation();       // �Ԃ̉�]�iY���j

    // �������i�Ԃ̌���x�N�g���j
    VECTOR backDir = VGet(-sinf(carRot.y), 0.3f, -cosf(carRot.y));
    backDir = VNorm(backDir); // ���K��

    float distance = 500.0f;    // �Ԃ���̋���
    float heightOffset = 200.0f; // �����I�t�Z�b�g

    // �J�����ʒu���v�Z�i�Ԃ̌�����j
    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    cameraPos.y += heightOffset;

    // �����_�͎Ԃ̏�����
    VECTOR target = carPos;
    target.y += 50.0f;

    // �J�����ݒ�
    SetCameraPositionAndTarget_UpVecY(cameraPos, target);
}