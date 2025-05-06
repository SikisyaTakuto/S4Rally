#include "DxLib.h"
#include "fps.h"
#include "game.h"
#include "key.h"
#include "mouse.h"
#include "font.h"
#include "music.h"
#include "graphic.h"
#include "map.h"
#include "timer.h"
#include "controller.h"
#include "car.h"


//�v���g�^�C�v�錾
VOID GameAndDxLibAllEnd(VOID);//�Q�[���I���̌�n��

//�Q�[���I���̌�n��
//������???_End�n���ł߂�
VOID GameAndDxLibAllEnd(VOID)
{
	Font_End();			//�t�H���g�I��
	Music_End();		//���y��n��
	Graphic_End();		//�摜��n��
	DxLib_End();		// �c�w���C�u�����g�p�̏I������

	return;
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//Windows�p�\�t�g�̃v���O�����̃X�^�[�g�n�_�ƂȂ�֐��̐錾
{
	SetOutApplicationLogValidFlag(FALSE);//Log.txt ���o�͂��Ȃ��@
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�̐ݒ�
	SetGraphMode(GameWidth, GameHeight, GameColor);//�𑜓x��ݒ�
	SetWindowSize(GameWidth, GameHeight);//�E�B���h�E�̑傫����ݒ�
	SetMainWindowText(GameTitle);//�E�B���h�E�̃^�C�g��
	SetBackgroundColor(0, 0, 0);//�E�B���h�E�̔w�i�F
	SetWaitVSyncFlag(GameVsync);//���������̐ݒ�
	SetAlwaysRunFlag(TRUE);//��A�N�e�B�u�ł����s

	SetWindowIconID(GameIcon);//�A�C�R����ݒ�

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//���y������
	if (Music_Init() == FALSE)
	{
		GameAndDxLibAllEnd();	//�Q�[���I���̌�n��
		return -1;
	}

	//����ʂɕ`�悷��i�_�u���o�b�t�@�����O�j
	SetDrawScreen(DX_SCREEN_BACK);

	//�t�H���g�̏�����
	if (Font_Init() == FALSE)
	{
		//�Q�[���I���̌�n��
		GameAndDxLibAllEnd();

		return -1;
	}

	//�摜������
	if (Graphic_Init() == FALSE)
	{
		GameAndDxLibAllEnd();	//�Q�[���I���̌�n��
		return -1;
	}

	//�Q�[����ʂ̏�����
	GameInit();

	//FPS������
	FPSInit();

	//�L�[�{�[�h������
	KeyInit();

	//�}�E�X������
	MouseInit();

	//�R���g���[���[������
	ControllerInit();

	//�^�C�g���V�[����������
	TitleInit();

	//�^�C�}�[�i����j�̏�����
	TimerInit();

	//�������[�v�i�Q�[�����[�v�j
	while (TRUE)
	{
		//���b�Z�[�W����������i�}�E�X�����L�[���͂Ȃǂ��󂯑�����j
		if (ProcessMessage() != 0)
		{
			break;//�������[�v����o��i�Q�[���I���j
		}

		//��ʂ���������i1���[�v������������j
		if (ClearDrawScreen() != 0)
		{
			break;//�������[�v����o��i�Q�[���I���j
		}

		//�����ɑS�ẴQ�[���̓��삪����
		{
			FPSCheck();//FPS�v��
			KeyUpdate();//�L�[�{�[�h�X�V
			MouseUpdate();//�}�E�X�X�V

			//���̃V�[����������
			if (NowGameScene != ChangeGameScene)
			{
				//�e�V�[���̏��������Ăяo��
				//�V�[���؂�ւ�
				switch (ChangeGameScene)
				{
				case TitleScene:
					TitleInit();
					break;
				case PlayScene:
					PlayInit();
					break;
				case ResultScene:
					ResultInit();
					break;
				case RuleScene:
					RuleInit();
					break;
				default:
					break;
				}

				//�V�[���؂�ւ���̃V�[�������݂̃V�[���ɂ���
			     NowGameScene= ChangeGameScene;
			}
			else
			{
				//�V�[���؂�ւ�
				switch (NowGameScene)
				{
				case TitleScene:
					TitleCtrl();
					break;
				case PlayScene:
					PlayCtrl();
					break;
				case ResultScene:
					ResultCtrl();
					break;
				case RuleScene:
					RuleCtrl();
				default:
					break;
				}
			}

			FPSDraw();//FPS�`��
			FPSWait();//FPS�ҋ@
		}

		//����ʂ��摜�ɕ`��
		ScreenFlip();
	}

	//�Q�[���I���̌�n��
	GameAndDxLibAllEnd();

	return 0;				// �\�t�g�̏I�� 
}

