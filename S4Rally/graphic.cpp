//�摜�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "graphic.h"

//�O���[�o���ϐ�

Graphic TitleBackImage;				//�^�C�g���w�i�摜
Graphic TitleLogo;					//�^�C�g�����S
/*
Graphic TitleButton;				//�^�C�g���{�^��
Graphic BackImageTurn;				//�w�i���]
Graphic BackGrass[BackGrassNum];	//����
Graphic GameoverLogo;		//�Q�[���I�[�o�[���S
Graphic NewrecordLogo;		//�j���[���R�[�h���S
*/
//�����܂�

//�ǂݍ��݁E�n���h������
BOOL Graphic_Init(VOID)
{

	//�^�C�g���w�i�摜
	TitleBackImage = LoadGraphic(
		GraphicPathTitleBackImage,
		GetPoint(0, 0),				//�w�i�͍��ォ��
		GetRect(0, 0, 0, 0),		//�����蔻��̗]���͖���
		GraAlignTopL); if (TitleBackImage.Handle == -1) { return FALSE; }

	
	//�^�C�g�����S
	TitleLogo = LoadGraphic(
		GraphicPathTitleLogo,
		GetGameWindowCenter(),				//���S�͉�ʒ�������
		GetRect(0, 0, 0, 0),				//�����蔻��̗]���͖���
		GraAlignBottomC); if (TitleLogo.Handle == -1) { return FALSE; }


	////�^�C�g���{�^��
	//TitleButton = LoadGraphic(
	//	GraphicPathTitleButton,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y + 50),		//�{�^���͉��ӂ肩��
	//	GetRect(10, 10, 10, 10),		//�����蔻������߂�
	//	GraAlignTopC); if (TitleButton.Handle == -1) { return FALSE; }

	////�v���C�w�i�摜
	//BackImageTurn = TitleBackImage;
	//BackImageTurn.c.Pos.x = BackImageTurn.c.Pos.x + BackImageTurn.c.Width;
	//BackImageTurn.c.Pos.y = 0;

	////����
	//BackGrass[0] = LoadGraphic(
	//	GraphicPathBackGrass,
	//	GetPoint(0, GameHeight),			//������Y�ʒu�͉�ʉ�
	//	GetRect(0, 0, 0, 0),		//�����蔻��̗]���͖���
	//	GraAlignBottomL); if (BackGrass[0].Handle == -1) { return FALSE; }

	//for (int i = 1; i < BackGrassNum; i++)
	//{
	//	//�����R�s�[
	//	BackGrass[i] = BackGrass[i - 1];

	//	//�摜�����ɕ��ׂ�
	//	BackGrass[i].c.Pos.x = BackGrass[i - 1].c.Pos.x + BackGrass[i - 1].c.Width;
	//}

	////�Q�[���I�[�o�[���S
	//GameoverLogo = LoadGraphic(
	//	GraphicPathGameOverLogo,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y),				//���S�͉�ʒ�������
	//	GetRect(0, 0, 0, 0),				//�����蔻��̗]���͖���
	//	GraAlignTopC); if (GameoverLogo.Handle == -1) { return FALSE; }

	////�j���[���R�[�h���S
	//NewrecordLogo = LoadGraphic(
	//	GraphicPathNewrecordLogo,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y - 200),				//���S�͉�ʒ�������
	//	GetRect(0, 0, 0, 0),				//�����蔻��̗]���͖���
	//	GraAlignTopC); if (GameoverLogo.Handle == -1) { return FALSE; }

	return TRUE;
}

//��n��
VOID Graphic_End(VOID)
{

	DeleteGraphic(TitleBackImage);
	DeleteGraphic(TitleLogo);
	//DeleteGraphic(TitleButton);
	//DeleteGraphic(BackImageTurn);
	//for (int i = 0; i < BackGrassNum; i++) { DeleteGraphic(BackGrass[i]); }
	//DeleteGraphic(GameoverLogo);
	//DeleteGraphic(NewrecordLogo);


	return;
}

//�ǂݍ��݃G���[���b�Z�[�W
VOID GraphicLoadErrMessage(const char* path)
{
	//�G���[�p������
	char ErrStr[ImagePathStrMax];

	//�G���[�p������̓��e����ɂ���
	for (int i = 0; i < ImagePathStrMax; i++) { ErrStr[i] = '\0'; }

	//�G���[���e��A�����Đ���
	sprintf_s(ErrStr, sizeof(ErrStr), "%s%s", path, ImageLoadErrCap);

	//�p�X���Ԉ���Ă���ȂǂŃC���X�g�[���ł��Ȃ��ꍇ�̓G���[
	MessageBox(
		GetMainWindowHandle(),		//�E�B���h�E�n���h��
		ErrStr,						//�G���[���e
		ImageLoadErrTitle,			//�G���[�^�C�g��
		MB_OK);						//OK�{�^���̂�

	return;
}

//�ǂݍ���
Graphic LoadGraphic(const char* path, POINT point, RECT margin, Graphic_Align align)
{
	Graphic graphic;

	//�t�@�C���̏ꏊ���R�s�[
	strcpy_s(graphic.c.Path, sizeof(graphic.c.Path), path);

	//�摜���������ɓǂݍ���
	graphic.Handle = LoadGraph(graphic.c.Path);

	//�ǂݍ��݃G���[
	if (graphic.Handle == -1)
	{
		GraphicLoadErrMessage(graphic.c.Path);
	}
	else
	{
		//����ɓǂݍ��݂��ł�����p���[���^�ݒ�

		//���ƍ������擾
		GetGraphSize(graphic.Handle, &graphic.c.Width, &graphic.c.Height);

		//��_
		graphic.c.Pos = point;

		//��������
		graphic.c.Align = align;

		//�����蔻��
		graphic.c.CollMargin = margin;	//�]����ݒ�
		ReMakeCollRect(&graphic);		//�����蔻�萶��

		//�摜��`�悷��
		graphic.c.IsDraw = TRUE;

		//�G�t�F�N�g������
		graphic.c.IsFxStartFlg = FALSE;			//�`��G�t�F�N�g�t���O������
		graphic.c.Fx = GraFxNone;				//�ʏ�`��
		graphic.c.FxMillTime = 0;				//�G�t�F�N�g���ԃJ�E���g
		graphic.c.FxMillTimeMax = 0;			//�G�t�F�N�g���ԃJ�E���gMAX
		graphic.c.FxInterMillTime = 0;			//�G�t�F�N�g�Ԋu�J�E���g
		graphic.c.FxInterMillTimeMax = 0;		//�G�t�F�N�g�Ԋu�J�E���gMAX
		graphic.c.Alpha = 1.0f;					//�����x������
	}

	return graphic;
}

//�`��
VOID DrawGraphic(Graphic* graphic)
{
	//�G�t�F�N�g������Ƃ��́A�G�t�F�N�g�J�n����
	if (graphic->c.IsFxStartFlg == TRUE)
	{
		//�`��G�t�F�N�g�J�n����
		switch (graphic->c.Fx)
		{
		case GraFxBlick:	//�_��
			//�G�t�F�N�g�Ԋu���ő�̎��ɓ����x��؂�ւ���
			if (graphic->c.FxInterMillTime == graphic->c.FxInterMillTimeMax)
			{
				if (graphic->c.Alpha == 1.0)
				{
					graphic->c.Alpha = 0.0f;		//�����ɂ���
				}
				else if (graphic->c.Alpha == 0.0)
				{
					graphic->c.Alpha = 1.0f;		//�s�����ɂ���
				}
			}
			//���ߏ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeIn:
			//�摜�̓����x���v�Z
			graphic->c.Alpha
				= sin(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//�o�ߎ��Ԃ���S�̂́����v�Z
					* (DX_PI / 2));														//sin(90)=1�Ƃ��Ċ������v�Z
			//���ߏ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeOut:
			//�摜�̓����x���v�Z
			graphic->c.Alpha
				= cos(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//�o�ߎ��Ԃ���S�̂́����v�Z
					* (DX_PI / 2));														//cos(90)=1�Ƃ��Ċ������v�Z
			//���ߏ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeInOut:
			//�摜�̓����x���v�Z
			graphic->c.Alpha
				= sin(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//�o�ߎ��Ԃ���S�̂́����v�Z
					* DX_PI);															//sin(180)=0�`1�`0�Ƃ��Ċ������v�Z
			//���ߏ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxNone:
			//�������Ȃ�
			break;
		default:
			break;
		}
		//�G�t�F�N�g���ԃJ�E���g�A�b�v
		graphic->c.FxMillTime++;

		//�G�t�F�N�g�Ԋu�J�E���g�A�b�v
		graphic->c.FxInterMillTime++;
	}

	//�`��ł���Ƃ���,�`�揈��
	if (graphic->c.IsDraw == TRUE)
	{
		//������Ƃɕ`��
		switch (graphic->c.Align)
		{
		case GraAlignTopL:
			//�㍶����ɕ`��
			DrawGraph(
				graphic->c.Pos.x,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignTopC:
			//�㒆������ɕ`��
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignTopR:
			//��E����ɕ`��
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomL:
			//��������ɕ`��
			DrawGraph(
				graphic->c.Pos.x,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomC:
			//����������ɕ`��
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomR:
			//���E����ɕ`��
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignAllCenter:
			//�㉺���E�̒�������ɕ`��
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y - graphic->c.Height / 2,
				graphic->Handle, TRUE);
			break;

		default:
			break;
		}
	}

	//�G�t�F�N�g������Ƃ��́A�G�t�F�N�g�I������
	if (graphic->c.IsFxStartFlg == TRUE)
	{
		switch (graphic->c.Fx)
		{
		case GraFxBlick:
			//�A���t�@�u�����h(���ߏ���)�I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeIn:
			//�A���t�@�u�����h(���ߏ���)�I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeOut:
			//�A���t�@�u�����h(���ߏ���)�I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeInOut:
			//�A���t�@�u�����h(���ߏ���)�I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxNone:
			//�������Ȃ�
			break;
		default:
			break;
		}

		//�C���^�[�o����MAX�Ȃ猳�ɖ߂�
		if (graphic->c.FxInterMillTime > graphic->c.FxInterMillTimeMax)
		{
			graphic->c.FxInterMillTime = 0;
		}

		//�G�t�F�N�g�������p���ł͂Ȃ��Ƃ�
		if (graphic->c.FxMillTimeMax != GraFxInfinity)
		{
			//�G�t�F�N�g���Ԃ��I���Ȃ�΃G�t�F�N�g�I��
			if (graphic->c.FxMillTime > graphic->c.FxMillTimeMax)
			{
				graphic->c.IsFxStartFlg = FALSE;
			}

		}
	}

	//�f�o�b�O���[�h�Ȃ瓖���蔻���`��
	if (GameDebug == TRUE)
	{
		//�`��ł���Ƃ���
		if (graphic->c.IsDraw == TRUE)
		{
			////��`��`��
			//DrawBox(
			//	graphic->c.Coll.left,
			//	graphic->c.Coll.top,
			//	graphic->c.Coll.right,
			//	graphic->c.Coll.bottom,
			//	GetColor(255, 0, 0),		//�ԐF
			//	FALSE);
		}
	}

	return;
}

//���
VOID DeleteGraphic(Graphic graphic)
{
	//����ɓǂݍ��݂��ł��Ă����
	if (graphic.Handle != -1)
	{
		//������������
		DeleteGraph(graphic.Handle);
	}

	return;
}

//�����蔻��̋�`���Đ���
VOID ReMakeCollRect(Graphic* graphic)
{
	//������ƂɍĐ���
	switch (graphic->c.Align)
	{
	case GraAlignTopL:
		//�㍶���
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignTopC:
		//�㒆�����
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignTopR:
		//��E���
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomL:
		//�������
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomC:
		//���������
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomR:
		//���E���
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignAllCenter:
		//���S���
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height / 2 + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 + graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height / 2 + graphic->c.CollMargin.bottom
		);
		break;

	default:
		break;
	}

	return;
}

//�G�t�F�N�g�J�n
VOID GraphicFxStart(Graphic* graphic, Graphic_Fx Fx, int MillTime, int MillInterval)
{
	//�G�t�F�N�g�̕b����������
	graphic->c.FxMillTime = 0;

	//�����G�t�F�N�g�������p���Ȃ�A���ۂ�MAX���Ԃ��v�Z�����Ȃ�
	if (MillTime == GraFxInfinity)
	{
		graphic->c.FxMillTimeMax = GraFxInfinity;
	}
	else
	{
		graphic->c.FxMillTimeMax = (MillTime / 1000.0f) * GameFPS;
	}

	//�G�t�F�N�g�̊Ԋu��������
	graphic->c.FxInterMillTime = 0;
	graphic->c.FxInterMillTimeMax = (MillInterval / 1000.0f) * GameFPS;

	//�G�t�F�N�g�̎�ނ�ݒ�
	graphic->c.Fx = Fx;

	//�G�t�F�N�g�J�n�t���OON
	graphic->c.IsFxStartFlg = TRUE;

	return;
}