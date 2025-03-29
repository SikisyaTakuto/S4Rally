//�摜�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "geometory.h"
#include "game.h"
#include "fps.h"

//�G�t�F�N�g�����p��
//�������Ȃ̂Œʏ�͂��肦�Ȃ�����
#define GraFxInfinity	-99999

//�G���[
#define ImageLoadErrCap		"���ǂݍ��߂܂���ł���"
#define ImageLoadErrTitle	"�摜�ǂݍ��݃G���["

//������̒���
#define ImagePathStrMax	256
#define ImageErrStrMax	512

//�摜�t�@�C���̃p�X
//�����{���G�����Ȃ�2�o�C�g����������邱��(�ň����������œǂݍ��߂Ȃ��Ƃ�������)
//���S�p�󔒁A���p�󔒖�킸�󔒂�����邱��(�t�@�C�����̋�؂肾�Ǝv���邱�Ƃ�����)

#define GraphicPathTitleBackImage	".\\graphic\\TitleBack.png"
#define GraphicPathTitleLogo		".\\graphic\\TitleLogo.png"
//#define GraphicPathTitleButton		".\\graphic\\StartButton.png"	//�@�T�C�g�b��z�Ȑ�	�t�q�k�bhttps://kopacurve.blog.fc2.com/
//#define GraphicPathBackGrass		".\\graphic\\weed.png"
//#define BackGrassNum				15		//�����̔w�i�̌�
//#define GraphicPathGameOverLogo		".\\graphic\\GameOverLogo.png"
//#define GraphicPathNewrecordLogo	".\\graphic\\NewrecordLogo.png"


//�񋓌^
enum Graphic_Align
{
	GraAlignTopL,		//�摜�̊�_�ŏ㍶����
	GraAlignTopC,		//�摜�̊�_�ŏ㒆������
	GraAlignTopR,		//�摜�̊�_�ŏ�E����

	GraAlignBottomL,	//�摜�̊�_�ŉ�������
	GraAlignBottomC,	//�摜�̊�_�ŉ���������
	GraAlignBottomR,	//�摜�̊�_�ŉ��E����

	GraAlignAllCenter,	//�摜�̊�_�ŏ㉺�����E����������
};

//�G�t�F�N�g�̎��(�`��̌���)
enum Graphic_Fx
{
	GraFxBlick,			//�_��
	GraFxFadeIn,		//�������s����
	GraFxFadeOut,		//�s����������
	GraFxFadeInOut,		//�s������������
	GraFxNone			//�ʏ�`��
};

//�摜�\���̂̃R�A����
typedef struct _GraphicCore
{
	char Path[ImagePathStrMax];			//�p�X

	POINT Pos;							//�摜�̊�_(�ʏ�͍���)
	int Width = 0;						//��(�E�B�h�X)
	int Height = 0;						//����(�n�C�g)

	RECT Coll;							//��`�̓����蔻��
	RECT CollMargin;					//��`�̓����蔻������߂�

	Graphic_Align Align = GraAlignTopL;	//��������

	float Rate = 1.0f;					//�g�嗦
	float Angle = 0.0f;					//�p�x

	BOOL IsDraw = FALSE;				//�`�悷�邩�H

	BOOL IsFxStartFlg = FALSE;			//�`��G�t�F�N�g�t���O
	Graphic_Fx Fx = GraFxNone;			//�`��G�t�F�N�g�̎��
	int FxMillTime = 0;					//�G�t�F�N�g���ԃJ�E���g
	int FxMillTimeMax = 0;				//�G�t�F�N�g���ԃJ�E���gMAX
	int FxInterMillTime = 0;			//�G�t�F�N�g�Ԋu�J�E���g
	int FxInterMillTimeMax = 0;			//�G�t�F�N�g�Ԋu�J�E���gMAX
	float Alpha = 0.0f;					//�摜�̓����x(MIN0.0�`MAX1.0�����邱��)
}GraphicCore;

//�摜�\����
typedef struct _Graphic
{
	GraphicCore c;		//�R�A
	int Handle = -1;	//�n���h��
}Graphic;

//�O���O���[�o���ϐ�
extern Graphic TitleBackImage;		//�^�C�g���w�i�摜
extern Graphic TitleLogo;			//�^�C�g�����S
extern Graphic TitleButton;			//�^�C�g���{�^��
extern Graphic BackImageTurn;		//�w�i���]
//extern Graphic BackGrass[BackGrassNum];	//����
extern Graphic GameoverLogo;		//�Q�[���I�[�o�[���S
extern Graphic NewrecordLogo;		//�j���[���R�[�h���S

//�O���v���g�^�C�v�錾
extern BOOL Graphic_Init(VOID);							//�ǂݍ��݁E�n���h������
extern VOID Graphic_End(VOID);							//��n��
extern VOID GraphicLoadErrMessage(const char* path);	//�ǂݍ��݃G���[���b�Z�[�W

extern Graphic LoadGraphic(const char* path, POINT point, RECT margin, Graphic_Align align);	//�ǂݍ���
extern VOID DrawGraphic(Graphic* graphic);				//�`��
extern VOID DeleteGraphic(Graphic graphic);				//���

extern VOID ReMakeCollRect(Graphic* graphic);			//�����蔻��̋�`���Đ���

extern VOID GraphicFxStart(Graphic* graphic, Graphic_Fx Fx, int MillTime, int MillInterval);	//�G�t�F�N�g(Fx)�J�n