//���j���[�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include"game.h"
#include"key.h"
#include"mouse.h"
#include"geometory.h"
#include"font.h"
#include "music.h"
#include "graphic.h"

//�}�N����`
#define MenuWidth 1280	//��ʂ̕�
#define MenuHeight 720	//��ʂ̍���
#define MenuColor 32	//��ʂ̐F
#define MenuTitle "GameTitle"//��ʃ^�C�g��
#define MenuVsync TRUE	//Vsync���g���Ȃ�TRUE
#define MenuIcon 999//�摜�A�C�R��

#define MenuDebug TRUE//�f�o�b�N���[�h

#define MenuSceneCount 3//�Q�[���V�[���̐�
#define MenuSceneNameMax 20//�Q�[���V�[�����̕�����MAX

#define MenuSceneChangeFrame 60//�Q�[���V�[���̐؂�ւ����\�ɂ���t���[����

//�F�n�̖��O
#define Color_brack GetColor(0,0,0)
#define Color_white GetColor(255,255,255)
#define Color_skyblue GetColor(135,206,235)
#define Color_tomato GetColor(255,99,71)
#define Color_lightyellow GetColor(255,255,224)
#define Color_lawngreen GetColor(124,252,0)
#define Color_pink GetColor(255,192,203)
#define Color_yellow GetColor(255,255,0)

enum MenuScene {
    Scene_Menu,    //���j���[���
    Scene_Game,    //�Q�[�����
    Scene_Config,  //�ݒ���
};

//�O���̃O���[�o���ϐ�
extern enum MenuScene MenuScene;//���j���[�V�[��
extern enum MenuScene ReturnGameScene;//�Q�[���V�[���ɖ߂�

//�O���̃v���g�^�C�v�錾

extern VOID AllMenuInit(VOID);//���j���[��ʏ�����
extern HWND GetMenuHandle(VOID);//���j���[��ʂ̃n���h�����擾
extern RECT GetMenuWindow(VOID);//���j���[��ʂ̋�`���擾

extern POINT GetMenuWindowCenter(VOID);//���j���[��ʂ̏㉺���E�̒������W���擾

extern VOID MenuInit(VOID);//���j���[������
extern VOID MenuCtrl(VOID);//���j���[�Ǘ�
extern VOID MenuProc(VOID);//���j���[����
extern VOID MenuDraw(VOID);//���j���[�`��