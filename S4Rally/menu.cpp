//���j���[�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "menu.h"

//�O���̃O���[�o���ϐ�
enum MenuScene MenuScene;//���j���[�V�[��
enum MenuScene ReturnGameScene;//�Q�[���V�[���ɖ߂�

//���j���[�V�[���̖��O
char MenuSceneName[GameSceneCount][GameSceneNameMax]
{
	"���j���[���"
};

//�V�[���؂�ւ���̃t���[�������Ǘ�
int MenuSceneFrameCount[GameSceneCount];

//�Q�[����ʊ֌W
RECT MenuWindow;//�Q�[����ʂ̋�`
HWND MenuHandle;//�Q�[���̃E�B���h�E�n���h��

//�Q�[����ʂ̃n���h�����擾
HWND GetMenuHandle(VOID)
{
	return MenuHandle;
}

//�Q�[����ʂ̋�`���擾
RECT GetMenuWindow(VOID)
{
	return MenuWindow;
}

//�Q�[����ʂ̏㉺���E�̒������W���擾
POINT GetMenuWindowCenter(VOID)
{
	POINT pt;

	pt.x = GameWidth / 2;
	pt.y = GameHeight / 2;

	return pt;
}

//�Q�[����ʂ̏�����
VOID AllMenuInit(VOID)
{
	return;
}

//���j���[������
VOID MenuInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//�V�[�����\��
		DrawFormatStringToHandle(
			GameWidth - 500, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", MenuSceneName[ChangeGameScene], "������");
	}

	//�V�[����؂�ւ����t���[������������
	MenuSceneFrameCount[ChangeGameScene] = 0;
	return;
}

//���j���[�Ǘ�
VOID MenuCtrl(VOID)
{
	return;
}

//���j���[����
VOID MenuProc(VOID)
{
	return;
}
//���j���[�`��
VOID MenuDraw(VOID)
{
	return;
}