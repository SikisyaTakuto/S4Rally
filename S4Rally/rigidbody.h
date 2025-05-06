//rigidbody�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "fps.h"
#include "wheelcollider.h"

class RigidBody
{
    public:
        // �����v���p�e�B
        float mass = 1.0f;           // ���� (Kg)
        float drag = 0.0f;           // ��C��R
        float angularDrag = 0.05f;   // ��]��R�i����͖������j
        bool isGravity = true;       // �d�͂��󂯂邩
        bool isGrounded = false;     // �n�ʂɐڐG���Ă��邩

        // ���
        VECTOR position = { 0, 0, 0 }; // ���݂̈ʒu
        VECTOR velocity = { 0, 0, 0 }; // ���݂̑��x
        VECTOR force = { 0, 0, 0 };    // ���̃t���[���œK�p�����

        // �R���X�g���N�^
        RigidBody();

        // �͂�������
        void AddForce(VECTOR f);

        // ���t���[���X�V
        void Update(float deltaTime);
};