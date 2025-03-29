//�}�E�X�����̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C��
#include"Dxlib.h"
#include"geometory.h"

//�}�N����`
#define MouseKindMax 8//�}�E�X�̃{�^���̎��
#define MouseCodeErrIndex 999//�}�E�X�R�[�h��ύX�����v�f���̃G���[�l

//�O���̃v���g�^�C�v�錾
extern VOID MouseNowIntoOld(VOID);//Now???�n�̕ϐ��̒l��Old???�n�̕ϐ��ɂ����
extern int MouseCodeToIndex(int MOUSE_INPUT_);//�}�E�X�̃{�^���R�[�h��z��̗v�f���ɕϊ�����

extern VOID MouseInit(VOID);//�}�E�X�����̏�����
extern VOID MouseUpdate(VOID);//�}�E�X�����̍X�V
extern BOOL MouseDown(int MOUSE_INPUT_);//����̃{�^�������������H
extern BOOL MouseClick(int MOUSE_INPUT_);//����̃{�^�����N���b�N�������H
extern int MousePressFrame(int MOUSE_INPUT_);//����̃{�^�����������t���[����

extern POINT GetPointMouse(VOID);//�}�E�X�̌��݂̈ʒu���擾����
extern POINT GetOldPointMouse(VOID);//�}�E�X�̈ȑO�̈ʒu���擾����
extern POINT GetDiffPointMouse(VOID);//�}�E�X�̈ȑO�ƌ��݂̈ʒu�̍����擾����
extern int GetWheelMouse(VOID); //�}�E�X�̌��݂̃z�C�[���ʂ��擾����

extern BOOL CollRectToMouse(RECT rect);//��`�ƃ}�E�X�̓_���������Ă��邩�H
extern BOOL CollRectToMouseDown(RECT rect, int MOUSE_INPUT_);//��`�ƃ}�E�X�̃{�^�������������H
extern BOOL CollRectToMouseClick(RECT rect, int MOUSE_INPUT_);//��`�ƃ}�E�X�̃{�^�����N���b�N�������H

extern BOOL CollCircleToMouse(CIRCLE circle);//�~�ƃ}�E�X�̓_���������Ă��邩�H
extern BOOL CollCircleToMouseDown(CIRCLE circle, int MOUSE_INPUT_);//�~���ƃ}�E�X�̃{�^�������������H
extern BOOL CollCircleToMouseClick(CIRCLE circle, int MOUSE_INPUT_);//�~���ƃ}�E�X�̃{�^�����N���b�N�������H