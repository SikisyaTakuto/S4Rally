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

    //// F1�L�[�Ńf�o�b�O���[�hON/OFF�؂�ւ�
    //if (CheckHitKey(KEY_INPUT_F1) && !debugMode) {
    //    debugMode = true;
    //}
    //else if (CheckHitKey(KEY_INPUT_F2) && debugMode) {
    //    debugMode = false;
    //}

    //if (debugMode) {
    //    // �f�o�b�O�J��������
    //    const float moveSpeed = 10.0f;
    //    const float rotSpeed = 0.02f;

    //    // ��]�i���L�[�j
    //    if (CheckHitKey(KEY_INPUT_LEFT)) {
    //        debugCamYaw -= rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_RIGHT)) {
    //        debugCamYaw += rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_UP)) {
    //        debugCamPitch -= rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_DOWN)) {
    //        debugCamPitch += rotSpeed;
    //    }

    //    // �s�b�`����
    //    if (debugCamPitch > DX_PI_F / 2.0f) debugCamPitch = DX_PI_F / 2.0f;
    //    if (debugCamPitch < -DX_PI_F / 2.0f) debugCamPitch = -DX_PI_F / 2.0f;

    //    // �O���x�N�g�����v�Z
    //    VECTOR forward = VGet(
    //        cosf(debugCamPitch) * sinf(debugCamYaw),
    //        sinf(debugCamPitch),
    //        cosf(debugCamPitch) * cosf(debugCamYaw)
    //    );
    //    forward = VNorm(forward);

    //    // �E�x�N�g��
    //    VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), forward);
    //    right = VNorm(right);

    //    // �㉺�ړ�
    //    VECTOR up = VGet(0.0f, 1.0f, 0.0f);

    //    // �ړ��iWASDQE�L�[�j
    //    if (CheckHitKey(KEY_INPUT_I)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(forward, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_K)) {
    //        debugCamPos = VSub(debugCamPos, VScale(forward, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_J)) {
    //        debugCamPos = VSub(debugCamPos, VScale(right, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_L)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(right, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_U)) {
    //        debugCamPos = VSub(debugCamPos, VScale(up, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_O)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(up, moveSpeed));
    //    }

    //    // �����_���v�Z
    //    VECTOR target = VAdd(debugCamPos, forward);

    //    // �J�����ݒ�
    //    SetCameraPositionAndTarget_UpVecY(debugCamPos, target);
    //}
    //else {
    //    // �ʏ�J�����i�ԑ̒Ǐ]�j
    //    VECTOR carPos = car.GetPosition();
    //    VECTOR carRot = car.GetRotation();

    //    VECTOR backDir = VGet(-sinf(carRot.y), 0.3f, -cosf(carRot.y));
    //    backDir = VNorm(backDir);

    //    float distance = 500.0f;
    //    float heightOffset = 300.0f;

    //    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    //    cameraPos.y += heightOffset;

    //    VECTOR target = carPos;
    //    target.y += 50.0f;

    //    SetCameraPositionAndTarget_UpVecY(cameraPos, target);
    //}
}