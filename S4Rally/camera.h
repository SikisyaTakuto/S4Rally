#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include"DxLib.h"
#include"fps.h"
#include"car.h"

class Camera
{
public:
    void Init();
    void Update();  // Car�̎Q�Ƃ�n��

private:
    bool debugMode = false; // �f�o�b�O���[�h���ǂ���
    VECTOR debugCamPos = VGet(0.0f, 300.0f, -500.0f); // �f�o�b�O�J���������ʒu
    float debugCamYaw = 0.0f;  // ����������]
    float debugCamPitch = 0.0f; // �㉺������]
};