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
    float mass = 1.0f;           // ���ʁiKg�P�ʁB0�ȉ��ɂ���ƕ������Z�����j
    float drag = 0.0f;           // �ړ����̋�C��R�W���i�����Ɏg�p�j
    float angularDrag = 0.05f;   // ��]���̋�C��R�W���i����͖��g�p�j
    bool isGravity = true;       // �d�͂�K�p���邩�ǂ����itrue�Ȃ�d�͉����x�������j
    bool isGrounded = false;     // �n�ʂɐڐG���Ă��邩�itrue�Ȃ�d�͂������Ȃ��j

    //��ԊǗ�
    VECTOR position = { 0, 0, 0 }; // ���݂̃��[���h���W
    VECTOR velocity = { 0, 0, 0 }; // ���݂̈ړ����x�x�N�g��
    VECTOR force = { 0, 0, 0 };    // ���t���[���ŉ�����ꂽ���v�̗�

    //���\�b�h

    // �R���X�g���N�^
    // �������������s���i���ʁA��R�l�A�ʒu�Ȃǁj
    RigidBody();

    // �w�肳�ꂽ�͂����Z����
    // �͎͂���Update()���ɑ��x��ʒu�ɔ��f�����
    void AddForce(VECTOR f);

    // ���t���[���Ăяo���A������Ԃ��X�V����
    // deltaTime: 1�t���[��������̌o�ߎ��ԁi�b�j
    void Update(float deltaTime);
}