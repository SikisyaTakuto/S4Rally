//���y�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"

//�G���[
#define MusicLoadErrCap		"���ǂݍ��߂܂���ł���"
#define MusicLoadErrTitle	"���y�ǂݍ��݃G���["

//�֐��`���}�N��
#define GetVolume(volume)	(255 * volume / 100)	//�{�����[����MIN:0�`MAX:100����MIN:0�`MAX:255�ɂ���

//������̒���
#define MusicPathStrMax 256
#define MusicErrStrMax 512

//�f�t�H���g�̉���
#define VolumeBGM	50	//BGM�͍T���߂�
#define VolumeSE	100	//SE�̓h�[���ƁI

//���y�t�@�C���̃p�X
//�����{���G�����Ȃ�2�o�C�g����������邱��(�ň����������œǂݍ��߂Ȃ���������)
//���S�p�󔒁A���p�󔒖�킸�󔒂�����邱��(�t�@�C�����̋�؂肾�Ǝv���邱�Ƃ�����j

#define MusicPathTitleBGM		"./music/bgm/maou_game_town04.mp3"
#define MusicPathPlayBGM		"./music/bgm/maou_game_medley02.mp3"
#define MusicPathButtonClickSE	"./music/se/maou_se_system47.mp3"
/*
#define MusicPathResultBGM		".\\music\\bgm\\maou_19_12345.mp3"
#define MusicPathButtonClickSE	".\\music\\se\\buttonClickSE.mp3"
#define MusicPathJumpSE			".\\music\\se\\jumpSE.mp3"
#define MusicPathExplSE			".\\music\\se\\explSE.mp3"
#define MusicPathAvoidSE		".\\music\\se\\avoidSE.mp3"
*/

//���y�\����
typedef struct _Music
{
	int Handle = -1;					//�摜�n���h��
	char Path[MusicPathStrMax];			//�p�X
	int Volume = -1;					//����
	BOOL IsPlayStart = TRUE;			//��~��͍ŏ�����Đ����邩�H
	int PlayType = DX_PLAYTYPE_BACK;	//�Đ��`��(�f�t�H���g��1��̂�)

	int FadeInMillTime = 0;				//�t�F�[�h�C���b
	int FadeOutMillTime = 0;			//�t�F�[�h�A�E�g�b

	int FadeInMillTimeMax = 0;			//�t�F�[�h�C��MAX�b
	int FadeOutMillTimeMax = 0;			//�t�F�[�h�A�E�gMAX�b

	BOOL FadeInStartFlg = FALSE;		//�t�F�[�h�C���J�n�t���O
	BOOL FadeOutStartFlg = FALSE;		//�t�F�[�h�C���I���t���O
}Music;

//�O���O���[�o���ϐ�
extern Music TitleBGM;		//�^�C�g���̉��y
extern Music PlayBGM;		//�v���C�̉��y
extern Music ResultBGM;		//���U���g�̉��y

extern Music ButtonClickSE;	//�{�^���N���b�N��
extern Music JumpSE;		//�W�����v��
extern Music ExplSE;		//������
extern Music AvoidSE;		//�����鉹

//�O���v���g�^�C�v�錾
extern BOOL Music_Init(VOID);										//�ǂݍ��݁E�n���h������
extern VOID Music_End(VOID);										//��n��
extern VOID MusicLoadErrMessage(const char* path);					//�ǂݍ��݃G���[���b�Z�[�W

extern Music LoadMusic(const char* path, int volume, int playType);	//�ǂݍ���
extern VOID PlayMusic(Music music);									//�Đ�
extern VOID PauseMusic(Music* music);								//�ꎞ��~
extern VOID StopMusic(Music* music);								//��~
extern VOID DeleteMusic(Music music);								//���

extern VOID ChangeVolumeMusic(Music* music, int volume);			//���ʕύX

extern BOOL FadeInPlayMusic(Music* music, int EffectTime);			//�t�F�[�h�C��
extern BOOL FadeOutPlayMusic(Music* music, int EffectTime);			//�t�F�[�h�A�E�g