//�}�b�v�����̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "car.h"
#include "wheelcollider.h"
#include "rigidbody.h"

// �萔��`
#define LINE_AREA_SIZE 1000000.0f // ���C����`���͈�
#define LINE_NUM 1000             // ���C���̐�

class Map
{
public:

    int modelHandle;  // ���f���n���h��

    Map();          // �R���X�g���N�^
    ~Map();         // �f�X�g���N�^

    void Init();    // �}�b�v������
    void Update();  // �}�b�v�X�V�i�K�v�Ȃ�j
    void Draw();    // �}�b�v�`��i���f���{���C���j
};