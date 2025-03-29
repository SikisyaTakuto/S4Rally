//�Q�[�������̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include"DxLib.h"

//�}�N����`
#define GameWidth 1280	//��ʂ̕�
#define GameHeight 720	//��ʂ̍���
#define GameColor 32	//��ʂ̐F
#define GameTitle "GameTitle"//��ʃ^�C�g��
#define GameVsync TRUE	//Vsync���g���Ȃ�TRUE
#define GameIcon 999//�摜�A�C�R��

#define GameDebug TRUE//�f�o�b�N���[�h

#define GameSceneCount 4//�Q�[���V�[���̐�
#define GameSceneNameMax 20//�Q�[���V�[�����̕�����MAX

#define GameSceneChangeFrame 60//�Q�[���V�[���̐؂�ւ����\�ɂ���t���[����

//�F�n�̖��O
#define Color_brack GetColor(0,0,0)
#define Color_white GetColor(255,255,255)
#define Color_skyblue GetColor(135,206,235)
#define Color_tomato GetColor(255,99,71)
#define Color_lightyellow GetColor(255,255,224)
#define Color_lawngreen GetColor(124,252,0)
#define Color_pink GetColor(255,192,203)
#define Color_yellow GetColor(255,255,0)

//�񋓌^

//�Q�[���V�[��
enum GameScene{
	TitleScene,//�^�C�g���V�[��
	PlayScene,//�v���C�V�[��
	ResultScene,//���U���g�V�[��
	RuleScene//���[���V�[��
};

//�O���̃O���[�o���ϐ�
extern enum GameScene NowGameScene;//���݂̃Q�[���V�[��
extern enum GameScene ChangeGameScene;//�؂�ւ��Q�[���V�[��

//�O���̃v���g�^�C�v�錾

extern VOID GameInit(VOID);//�Q�[����ʏ�����
extern HWND GetGameHandle(VOID);//�Q�[����ʂ̃n���h�����擾
extern RECT GetGameWindow(VOID);//�Q�[����ʂ̋�`���擾

extern POINT GetGameWindowCenter(VOID);//�Q�[����ʂ̏㉺���E�̒������W���擾

extern VOID TitleInit(VOID);//�^�C�g��������
extern VOID TitleCtrl(VOID);//�^�C�g���Ǘ�
extern VOID TitleProc(VOID);//�^�C�g������
extern VOID TitleDraw(VOID);//�^�C�g���`��

extern VOID PlayInit(VOID);//�v���C������
extern VOID PlayCtrl(VOID);//�v���C�Ǘ�
extern VOID PlayProc(VOID);//�v���C����
extern VOID PlayDraw(VOID);//�v���C�`��

extern VOID TimeAttackPlayInit(VOID);//�v���C������
extern VOID TimeAttackPlayCtrl(VOID);//�v���C�Ǘ�
extern VOID TimeAttackPlayProc(VOID);//�v���C����
extern VOID TimeAttackPlayDraw(VOID);//�v���C�`��

extern VOID LicensePlayInit(VOID);//�v���C������
extern VOID LicensePlayCtrl(VOID);//�v���C�Ǘ�
extern VOID LicensePlayProc(VOID);//�v���C����
extern VOID LicensePlayDraw(VOID);//�v���C�`��

extern VOID ResultInit(VOID);//���U���g������
extern VOID ResultCtrl(VOID);//���U���g�Ǘ�
extern VOID ResultProc(VOID);//���U���g����
extern VOID ResultDraw(VOID);//���U���g�`��

extern VOID RuleInit(VOID);//���[���E�������������
extern VOID RuleCtrl(VOID);//���[���E��������Ǘ�
extern VOID RuleProc(VOID);//���[���E�����������
extern VOID RuleDraw(VOID);//���[���E��������`��