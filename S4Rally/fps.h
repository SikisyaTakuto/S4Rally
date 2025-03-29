//FPS�����̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"

//�}�N����`
#define GameFPS 60//�ݒ肵����FPS

#define MicroSecond 1000000.0f//1�}�C�N���b
#define MillSecond 1000.0f//1�~���b
#define WaitTimeMill 3000//�ő�ő҂Ă�~����

//�\���̂̒�`�����O�̍Ē�`
typedef struct _FPS
{
	LONGLONG FirstTakeTime = 0;//1�t���[���ڂ̌v������
	LONGLONG NowTakeTime = 0;//���݂̌v������
	LONGLONG OldTakeTime = 0;//�ȑO�̌v������

	float Deltatime = 0.000001f;//�f���^�^�C���i�o�ߎ��ԁj
	int FrameCount = 1;//���݂̃t���[�����i1�t���[���ځ`MAX�t���[���ڂ܂Łj
	float Average = 0.0f;//����FPS�l
}FPS;

//�O���O���[�o���ϐ�
extern FPS fps;

//�O���v���g�^�C�v�錾
extern VOID FPSInit(VOID);//FPS������
extern VOID FPSCheck(VOID);//FPS�v��
extern VOID FPSDraw(VOID);//FPS�`��
extern VOID FPSWait(VOID);//FPS�ҋ@