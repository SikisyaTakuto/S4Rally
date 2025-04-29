#pragma once

#include "DxLib.h"

class SuspensionSpring {
public:
    float spring;           // �X�v�����O�萔 (N/m)
    float damper;           // �����W�� (Ns/m)
    float targetPosition;   // �T�X�y���V�����̖ڕW�ʒu (0.0 �` 1.0)
    float restLength;       // �T�X�y���V�����̎��R�� (m)
    float currentLength;    // ���݂̃T�X�y���V������ (m)
    float velocity;         // �T�X�y���V�����̐L�k���x (m/s)

    // �R���X�g���N�^�E�f�X�g���N�^ 
    SuspensionSpring();
    ~SuspensionSpring();

    // �T�X�y���V�����̗͂��v�Z����
    float CalculateForce(float deltaTime, float newLength);

};
