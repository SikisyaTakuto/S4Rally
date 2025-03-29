//�L�[�{�[�h�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include"key.h"

//�O���[�o���ϐ�

//���݂̃L�[�������Ă���t���[����
int NowKeyPressFrame[KeyKindMax];

//1�O�̃L�[�������Ă���t���[����
int OldKeyPressFrame[KeyKindMax];

//�֐�

//�L�[�{�[�h�����̏�����
VOID KeyInit(VOID)
{
	//�z��̏�����
	for (int i = 0; i < KeyKindMax; i++)
	{
		NowKeyPressFrame[i] = 0;
		OldKeyPressFrame[i] = 0;
	}

	return;
}

//�L�[�{�[�h�����̍X�V
VOID KeyUpdate(VOID)
{
	//�ꎞ�I�ɕێ�����L�[���
	char KeyState[KeyKindMax];

	//���ׂẴL�[������x�Ɏ擾
	GetHitKeyStateAll(&KeyState[0]);

	//�L�[�����X�V
	for (int i = 0; i < KeyKindMax; i++)
	{
		//1�O�̃L�[���͂�ۑ�
		OldKeyPressFrame[i] = NowKeyPressFrame[i];

		//����,�L�[��������Ă���Ƃ�
		if (KeyState[i] != 0)
		{
			//������Ă���t���[�������J�E���g�A�b�v
			NowKeyPressFrame[i]++;
		}
		else if (KeyState[i] == 0)
		{
			//������Ă��Ȃ��Ȃ�A�t���[�������[���N���A
			NowKeyPressFrame[i]=0;
		}
	}

	return;
}

//����̃L�[�����������H
//����:DX���C�u�����[�̃L�[�R�[�h(KEY_INPUT_�Ŏn�܂�}�N����`)
BOOL KeyDown(int KEY_INPUT_)
{
	//���݉�����Ă���L�[�̃t���[������0������Ȃ�
	if (NowKeyPressFrame[KEY_INPUT_] > 0)
	{
		return TRUE;//������
	}

	return FALSE;//�����Ă��Ȃ�
}

//����̃L�[���N���b�N���H
//����:DX���C�u�����[�̃L�[�R�[�h(KEY_INPUT_�Ŏn�܂�}�N����`)
//����:�u�L�[�������{�L�[��������v���Z�b�g�ōs��ꂽ�Ƃ����A�N���b�N
BOOL KeyClick(int KEY_INPUT_)
{
	//����,�L�[�͉�����Ă��炸�@����
	//��O�̃L�[�͉�����Ă�����
	if (NowKeyPressFrame[KEY_INPUT_] == 0&&
		OldKeyPressFrame[KEY_INPUT_] > 0)
	{
		return TRUE;//������
	}

	return FALSE;//�����Ă��Ȃ�
}

//����̃L�[���������t���[����
//����:DX���C�u�����[�̃L�[�R�[�h(KEY_INPUT_�Ŏn�܂�}�N����`)
//����:�߂�l�̓~���b�Ȃǂł͂Ȃ��A�t���[����!
int KeyPressFrame(int KEY_INPUT_)
{
	return NowKeyPressFrame[KEY_INPUT_];
}