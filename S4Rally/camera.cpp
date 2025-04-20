#include "camera.h"

// �J�������
VECTOR cameraPosition;
VECTOR cameraTarget;
float cameraDistance = 1000.0f; // �J��������
float cameraAngleX = 0.0f;      // �J�����̏㉺�p�x
float cameraAngleY = 0.0f;      // �J�����̐����p�x
const float cameraZoomSpeed = 50.0f;   // �Y�[�����x
const float cameraRotateSpeed = 0.05f; // ��]���x


// ?? �p�x�̕␳�i-180�`180�x�͈͂Ɏ��߂�j
float NormalizeAngle(float angle) {
    while (angle > 360.0f) angle = 0.0f;
    while (angle < -360.0f) angle = 0.0f;
    return angle;
}

//�J�����̐ݒ�
VOID CarSetCamera(VOID) {
    // �J�����̃p�����[�^
    const float CAMERA_DISTANCE = 700.0f;   // �Ԃ̌������
    const float CAMERA_HEIGHT = 200.0f;     // �J�����̍���
    const float CAMERA_SMOOTHNESS = 0.1f;   // �Ǐ]�X���[�W���O

    float deltaTime = fps.Deltatime;

    //�Ԃ̉�]�p�x���擾�i���K���j
    float targetAngleY = NormalizeAngle(-carInfo.steeringAngle * deltaTime);

    //�p�x��Ԃ��X���[�Y�ɏ����i�}���ȉ�]��h���j
    float angleDiff = NormalizeAngle(targetAngleY - cameraAngleY);
    cameraAngleY += angleDiff * CAMERA_SMOOTHNESS;

    // �Ԃ̌�����\����]�s��
    MATRIX carMatrix = MGetRotY(cameraAngleY);

    // �Ԃ̌���ɃI�t�Z�b�g��K�p
    VECTOR offset = VTransform(VGet(0.0f, CAMERA_HEIGHT, -CAMERA_DISTANCE), carMatrix);

    // �ڕW�J�����ʒu���v�Z
    VECTOR targetCameraPos = VAdd(carInfo.position, offset);

    // �X���[�Y�ɃJ��������
    cameraPosition = VAdd(VScale(cameraPosition, 1.0f - CAMERA_SMOOTHNESS),
        VScale(targetCameraPos, CAMERA_SMOOTHNESS));

    // �J������ݒ�
    SetCameraPositionAndTarget_UpVecY(cameraPosition, carInfo.position);
}