//�L�[�{�[�h�����̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C��
#include"Dxlib.h"

//�}�N����`
#define KeyKindMax 256//�L�[�̎��

//�O���v���g�^�C�v�錾
extern VOID KeyInit(VOID);//�L�[�{�[�h�����̏�����
extern VOID KeyUpdate(VOID);//�L�[�{�[�h�����̍X�V
extern BOOL KeyDown(int KEY_INPUT_);//����̃L�[�����������H
extern BOOL KeyClick(int KEY_INPUT_);//����̃L�[���N���b�N�������H
extern int KeyPressFrame(int KEY_INPUT_);//����̃L�[���������t���[����