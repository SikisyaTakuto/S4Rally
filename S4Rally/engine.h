#pragma once

#include "DxLib.h"

// �O���錾�i�z�ˑ��h�~�j
class Transmission;

class Engine {
public:
    float engineRPM;                 // �G���W����]��
    float idleRPM;                  // �A�C�h�����O��]��
    float maxRPM;                   // �ő��]��
    float redlineRPM;              // ���b�h�]�[���J�n��]��
    float engineBrakeCoefficient;  // �G���W���u���[�L�̌���

    int carModelHandle;            // �ԑ̃��f���n���h��

    Engine();

    void Update(float velocity, Transmission& transmission); // RPM�X�V�����i�M�A�Ƃ̘A�g�j

    // �e��Q�b�^�[
    float GetRPM() const;
    float GetBrakeCoefficient() const;

    // �Z�b�^�[�i�K�v�ɉ����āj
    void SetCarModelHandle(int handle);
    void SetRPM(float rpm);
};