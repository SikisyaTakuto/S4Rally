//�Q�[�������̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "game.h"
#include "key.h"
#include "mouse.h"
#include "geometory.h"
#include "font.h"
#include "music.h"
#include "graphic.h"
#include "map.h"
//#include "rallycar.h"
#include "timer.h"
#include "controller.h"
#include "car.h"
//#include "wheelcollider.h"
//#include "carbase.h"

//�O���[�o���ϐ�

//�Q�[���V�[��
enum GameScene NowGameScene;//���݂̃Q�[���V�[��
enum GameScene ChangeGameScene;//�؂�ւ��Q�[���V�[��

//�Q�[����ʊ֌W
RECT GameWindow;//�Q�[����ʂ̋�`
HWND GameHandle;//�Q�[���̃E�B���h�E�n���h��

//�Q�[���V�[���̖��O
char GameSceneName[GameSceneCount][GameSceneNameMax]
{
	"�^�C�g�����",
	"�v���C���",
	"���U���g���",
	"���j���[���"
};

//�V�[���؂�ւ���̃t���[�������Ǘ�
int GameSceneFrameCount[GameSceneCount];

//�e�V�[���̐؂�ւ��{�^��
CIRCLE StartCircle;//�X�^�[�g�̉~
CIRCLE PlayCircle;//�v���C�̉~
CIRCLE ResultCircle;//���U���g�̉~
CIRCLE MenuCircle;//���j���[�̉~

RECT controllerPointRect; //�R���g���[���[�̃X�e�B�b�N�̉~

RECT TimeAttackRect; //�^�C���A�^�b�N
RECT RuleRunRect;    //���[��
RECT LockRunRect;    //���b�N

RECT backSceneRect;	 //�V�[���ɖ߂�
RECT backPanelRect;  //�o�b�N�p�l��

int BGMVolume = VolumeBGM;	// BGM�̉��ʒ����̒l
int BGMVolumeDrawValue;		// BGM�̕`��Ŏg���l

int SEVolume = VolumeSE;//SE�̉��ʒ����̒l
int SEVolumeDrawValue;	// SE�̕`��Ŏg���l

// �I�����o�𒲐����邽�߂̕ϐ�
int stickCooldown = 0; // �N�[���_�E���^�C�}�[
const int stickCooldownMax = 10; // �N�[���_�E���Ԋu

float LeftPlayerPosition = 990.0f;
float TopPlayerPosition = 540.0f;
float RightPlayerPosition = 1015.0f;
float BottomPlayerPosition = 565.0f;

float StickSensitivity = 10.0f; // �X�e�B�b�N�̊��x�ݒ�i�l�𒲐����ē��쑬�x�𐧌�j

float mainSelect=0;

//�֐�

//�Q�[����ʂ̃n���h�����擾
HWND GetGameHandle(VOID)
{
	return GameHandle;
}

//�Q�[����ʂ̋�`���擾
RECT GetGameWindow(VOID)
{
	return GameWindow;
}

//�Q�[����ʂ̏㉺���E�̒������W���擾
POINT GetGameWindowCenter(VOID)
{
	POINT pt;

	pt.x = GameWidth / 2;
	pt.y = GameHeight / 2;

	return pt;
}

//�Q�[����ʂ̏�����
VOID GameInit(VOID)
{
	//�Q�[����ʂ̃n���h��(�Ǘ��ԍ�)���擾
	GameHandle = GetMainWindowHandle();

	//�Q�[����ʂ̋�`���擾
	GameWindow = GetRect(0, 0, GameWidth, GameHeight);

	//�ŏ��̃V�[���̓^�C�g������
	NowGameScene = TitleScene;

	//���̃V�[�����^�C�g������
	ChangeGameScene = TitleScene;

	//�R���g���[���[�̃X�e�B�b�N�ő��삷���`
	LeftPlayerPosition = 990.0f;
	TopPlayerPosition = 540.0f;
	RightPlayerPosition = 1015.0f;
	BottomPlayerPosition = 565.0f;
	controllerPointRect= GetRect(LeftPlayerPosition, TopPlayerPosition,RightPlayerPosition, BottomPlayerPosition);

	//�e�V�[���̉~�̏�����
	StartCircle = GetCircle(GetGameWindowCenter(), 300.0f); //���a300.0f�̉~����ʒ����ɂ���
	PlayCircle = GetCircle(GetPoint(1200.0f, 150.0f), 30.0f);
	ResultCircle = GetCircle(GetGameWindowCenter(), 300.0f);//���a300.0f�̉~����ʒ����ɂ���
	MenuCircle = GetCircle(GetPoint(1200.0f,50.0f), 30.0f);

	//�X�^�[�g�V�[���̃Q�[�����[�h�I�����̋�`�̏�����
	TimeAttackRect=GetRect(700, 50, 1200, 200);
	RuleRunRect = GetRect(700, 250, 1200, 400);
	LockRunRect = GetRect(700, 450, 1200, 600);

	//�X�^�[�g�V�[���̃��j���[���
	backSceneRect = GetRect(50, 630, 300, 700);
	backPanelRect = GetRect(100, 50, 1200,600);

	return;
}
//�^�C�g��������
VOID TitleInit(VOID)
{
	//�R���g���[���[�̃X�e�B�b�N�ő��삷���`
	LeftPlayerPosition = 990.0f;
	TopPlayerPosition = 540.0f;
	RightPlayerPosition = 1015.0f;
	BottomPlayerPosition = 565.0f;

	if (GameDebug == TRUE)
	{
		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack,fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "������");
	}

	//�V�[����؂�ւ����t���[������������
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//�^�C�g���Ǘ�
VOID TitleCtrl(VOID)
{
	TitleProc();//�������Ă��邩��
	TitleDraw();// �`�悷��
	return;
}
//�^�C�g������
VOID TitleProc(VOID)
{
	//�V�[���؂�ւ���̃t���[�������J�E���g�A�b�v
	GameSceneFrameCount[NowGameScene]++;

	//���y���Đ�
	PlayMusic(TitleBGM);
	
	//�t�F�[�h�C�����čĐ�
	//FadeInPlayMusic(&TitleBGM, 5000);	//5�b�����ăt�F�[�h�C��

	//�t�F�[�h�A�E�g���čĐ�
	//FadeOutPlayMusic(&TitleBGM, 5000);	//5�b�����ăt�F�[�h�C��

	////�V�[���؂�ւ�
	//if (CollCircleToMouseDown(StartCircle, MOUSE_INPUT_LEFT) == TRUE//�~�̂ǂ������}�E�X���{�^���ŉ������Ƃ�
	//	&& GameSceneFrameCount[NowGameScene] >= GameSceneChangeFrame)	//���A�؂�ւ��\�ȃt���[�����𒴂�����
	//{
	//	//���ʉ����Đ�
	//	PlayMusic(ButtonClickSE);
	//	
	//	//���y��~
	//	PauseMusic(&TitleBGM);

	//	//�V�[���؂�ւ�
	//	ChangeGameScene = PlayScene;

	//	//�����ɐ؂�ւ���
	//	return;
	//}

	////�摜�G�t�F�N�g�J�n
	//GraphicFxStart(
	//	&TitleButton,		//�ǂ̉摜�ɃG�t�F�N�g��������H
	//	GraFxFadeInOut,		//�ǂ�ȃG�t�F�N�g��������H
	//	GraFxInfinity,		//�G�t�F�N�g�����p��
	//	1000);				//1000�~���b�Ԋu

	return;
}
//�^�C�g���`��
VOID TitleDraw(VOID)
{
	ControllerUpdate();//�R���g���[���[�̍X�V����

	// �A�i���O�X�e�B�b�N�̒l���擾���ĕ\��
	float axisX = GetAxisX();
	float axisY = GetAxisY();

	if (GameDebug == TRUE)
	{
		//�K���ɕ`��
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(178, 216, 255), TRUE);

		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "�`�撆");

		//���j���[�̑I��
		{
			if (CollRectToRect(TimeAttackRect, controllerPointRect))
			{
				DrawRect(TimeAttackRect, Color_tomato, TRUE);
				if (GetButtonDown(BUTTON_A)||KeyDown(KEY_INPUT_SPACE))
				{
					//���ʉ����Đ�
					PlayMusic(ButtonClickSE);

					//���y��~
					PauseMusic(&TitleBGM);

					//�V�[���؂�ւ�
					ChangeGameScene = PlayScene;

					//�����ɐ؂�ւ���
					return;
				}
			}
			else{
				DrawRect(TimeAttackRect, Color_white, TRUE);
			}

			if (CollRectToRect(RuleRunRect, controllerPointRect))
			{
				DrawRect(RuleRunRect, Color_tomato, TRUE);
				if (GetButtonDown(BUTTON_A) || KeyDown(KEY_INPUT_SPACE))
				{
					//���ʉ����Đ�
					PlayMusic(ButtonClickSE);

					//���y��~
					PauseMusic(&TitleBGM);

					//�V�[���؂�ւ�
					ChangeGameScene = RuleScene;

					//�����ɐ؂�ւ���
					return;
				}
			}
			else {
				DrawRect(RuleRunRect, Color_white, TRUE);
			}

			DrawRect(LockRunRect, Color_white, TRUE);
		}
		{
			if (CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) {
				LeftPlayerPosition -= StickSensitivity;
				RightPlayerPosition -= StickSensitivity;
			}
			else if (CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) {
				LeftPlayerPosition += StickSensitivity;
				RightPlayerPosition += StickSensitivity;
			}

			if (CheckHitKey(KEY_INPUT_W) || axisY >= 0.5f) {
				TopPlayerPosition -= StickSensitivity;
				BottomPlayerPosition -= StickSensitivity;
			}
			else if (CheckHitKey(KEY_INPUT_S) || axisY <= -0.5f) {
				TopPlayerPosition += StickSensitivity;
				BottomPlayerPosition += StickSensitivity;
			}

			// ��ʊO�ɏo�Ȃ��悤�ɐ������鏈��
			if (LeftPlayerPosition < 0) {
				LeftPlayerPosition = 0;
				RightPlayerPosition = 25.0f; // ��`�̕���ێ�
			}
			if (RightPlayerPosition > GameWidth) {
				RightPlayerPosition = GameWidth;
				LeftPlayerPosition = GameWidth - 25.0f; // ��`�̕���ێ�
			}
			if (TopPlayerPosition < 0) {
				TopPlayerPosition = 0;
				BottomPlayerPosition = 25.0f; // ��`�̍�����ێ�
			}
			if (BottomPlayerPosition > GameHeight) {
				BottomPlayerPosition = GameHeight;
				TopPlayerPosition = GameHeight - 25.0f; // ��`�̍�����ێ�
			}

			// �X�V�����l��controllerPointRect�ɔ��f
			controllerPointRect = GetRect(LeftPlayerPosition, TopPlayerPosition, RightPlayerPosition, BottomPlayerPosition);

			// ��`��`��
			DrawRect(controllerPointRect, GetColor(255, 0, 0), TRUE);
		}
	}


	DrawFormatStringToHandleAlign(
		300,50, Align_Center, Color_brack, fontJiyu100ptFuch.Handle,
		"%s", "S4Rally");

	DrawFormatStringToHandleAlign(
		950, 100, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
		"%s", "�^�C���A�^�b�N");

	DrawFormatStringToHandleAlign(
		950, 300, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
		"%s", "���[���E�������");

return;
}

//�v���C������
VOID PlayInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "������");
	}

	//�V�[����؂�ւ����t���[������������
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//�v���C�Ǘ�
VOID PlayCtrl(VOID)
{
	PlayProc();//�������Ă��邩��
	PlayDraw();// �`�悷��
	return;
}
//�v���C����
VOID PlayProc(VOID)
{
	ControllerUpdate();

	//���y���Đ�
	PlayMusic(PlayBGM);

	//�V�[���؂�ւ���̃t���[�������J�E���g�A�b�v
	GameSceneFrameCount[NowGameScene]++;

	//if (GetButtonDown(BUTTON_START))
	//{
	//	//���ʉ����Đ�
	//	PlayMusic(ButtonClickSE);

	//	//���y��~
	//	PauseMusic(&PlayBGM);

	//	//�V�[���؂�ւ�
	//	ChangeGameScene =MenuScene;

	//	//�����ɐ؂�ւ���
	//	return;
	//}

	return;
}
//�v���C�`��
VOID PlayDraw(VOID)
{
	if (GameDebug == TRUE)
	{
		GraphDraw();	//�X�e�[�W��\��

		ControllerUpdate(); //�R���g���[���[�̍X�V

		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "�`�撆");

	    CarUpdate();

		TimerUpdate();
		TimerDraw();
	}

	return;
}

//���U���g������
VOID ResultInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "������");

		//���y��~
		PauseMusic(&PlayBGM);

	}

	//�V�[����؂�ւ����t���[������������
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//���U���g�Ǘ�
VOID ResultCtrl(VOID)
{
	ResultProc();//�������Ă��邩��
	ResultDraw();// �`�悷��
	return;
}
//���U���g����
VOID ResultProc(VOID)
{
	// �V�[���؂�ւ���̃t���[�������J�E���g�A�b�v
	GameSceneFrameCount[NowGameScene]++;

	// 7�b�o�ߌ�A�^�C�g���V�[���ɖ߂�
	if (GameSceneFrameCount[NowGameScene] >= 420)
	{
		//���ʉ����Đ�
		PlayMusic(ButtonClickSE);

		// �V�[���؂�ւ�
		ChangeGameScene = TitleScene;

		return;
	}

	return;
}
//���U���g�`��
VOID ResultDraw(VOID)
{
	if (GameDebug == TRUE)
	{
		//�K���ɕ`��
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(255, 230, 179), TRUE);

		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "�`�撆");
	}

	////�~�̒��Ƀ}�E�X�̍��W����������~�̐F���A��
	//if (CollCircleToMouse(StartCircle) == TRUE)
	//{
	//	//�~��`��
	//	DrawEn(StartCircle, Color_tomato, TRUE);
	//}
	//else
	//{
	//	//�~��`��
	//	DrawEn(StartCircle, Color_white, TRUE);
	//}

	////�Ƃ肠�����V�[������`��
	//DrawFormatStringToHandle(
	//	GetGameWindowCenter().x - 160, GetGameWindowCenter().y + 10, Color_brack, fontJiyu100ptFuch.Handle,
	//	"%s", " Result!!");

	return;
}

//���[���E�������������
VOID RuleInit(VOID)
{

	if (GameDebug == TRUE)
	{
		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "������");
	}

	//�V�[����؂�ւ����t���[������������
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}

//���[���E��������Ǘ�
VOID RuleCtrl(VOID)
{
	RuleProc();
	RuleDraw();
	return;
}

//���[���E�����������
VOID RuleProc(VOID)
{
	//�V�[���؂�ւ���̃t���[�������J�E���g�A�b�v
	GameSceneFrameCount[NowGameScene]++;

	return;
}

//���[���E��������`��
VOID RuleDraw(VOID)
{
	ControllerUpdate();//�R���g���[���[�̍X�V����

	// �A�i���O�X�e�B�b�N�̒l���擾���ĕ\��
	float axisX = GetAxisX();
	float axisY = GetAxisY();

	if (GameDebug == TRUE)
	{
		//�K���ɕ`��
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(255, 230, 179), TRUE);

		DrawRect(backPanelRect, Color_white, TRUE);

		if (CollRectToRect(backSceneRect, controllerPointRect))
		{
			DrawRect(backSceneRect, Color_tomato, TRUE);
			if (GetButtonDown(BUTTON_A) || KeyDown(KEY_INPUT_SPACE))
			{
				//���ʉ����Đ�
				PlayMusic(ButtonClickSE);

				//���y��~
				PauseMusic(&TitleBGM);

				//�V�[���؂�ւ�
				ChangeGameScene = TitleScene;

				//�����ɐ؂�ւ���
				return;
			}
		}
		else {
			DrawRect(backSceneRect, Color_white, TRUE);
		}
	}


	{
		if (CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) {
			LeftPlayerPosition -= StickSensitivity;
			RightPlayerPosition -= StickSensitivity;
		}
		else if (CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) {
			LeftPlayerPosition += StickSensitivity;
			RightPlayerPosition += StickSensitivity;
		}

		if (CheckHitKey(KEY_INPUT_W) || axisY >= 0.5f) {
			TopPlayerPosition -= StickSensitivity;
			BottomPlayerPosition -= StickSensitivity;
		}
		else if (CheckHitKey(KEY_INPUT_S) || axisY <= -0.5f) {
			TopPlayerPosition += StickSensitivity;
			BottomPlayerPosition += StickSensitivity;
		}

		// ��ʊO�ɏo�Ȃ��悤�ɐ������鏈��
		if (LeftPlayerPosition < 0) {
			LeftPlayerPosition = 0;
			RightPlayerPosition = 25.0f; // ��`�̕���ێ�
		}
		if (RightPlayerPosition > GameWidth) {
			RightPlayerPosition = GameWidth;
			LeftPlayerPosition = GameWidth - 25.0f; // ��`�̕���ێ�
		}
		if (TopPlayerPosition < 0) {
			TopPlayerPosition = 0;
			BottomPlayerPosition = 25.0f; // ��`�̍�����ێ�
		}
		if (BottomPlayerPosition > GameHeight) {
			BottomPlayerPosition = GameHeight;
			TopPlayerPosition = GameHeight - 25.0f; // ��`�̍�����ێ�
		}

		// �X�V�����l��controllerPointRect�ɔ��f
		controllerPointRect = GetRect(LeftPlayerPosition, TopPlayerPosition, RightPlayerPosition, BottomPlayerPosition);

		// ��`��`��
		DrawRect(controllerPointRect, GetColor(255, 0, 0), TRUE);

		DrawFormatStringToHandleAlign(
			170, 640, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
			"%s", "�߂�");
	}


	return;
}