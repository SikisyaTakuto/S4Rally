//�}�E�X�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "mouse.h"
#include"game.h"
#include"geometory.h"

//�O���[�o���ϐ�(�ߋ�)
//MousePoint NowPoint;//���݂̃}�E�X�̈ʒu
//MousePoint OldPoint;//�ȑO�̃}�E�X�̈ʒu

POINT NowPoint;//���݂̃}�E�X�̈ʒu
POINT OldPoint;//�ȑO�̃}�E�X�̈ʒu

int NowMousePressFrame[MouseKindMax];//���݂̃}�E�X�̃{�^���������Ă���t���[�������Ǘ�
int OldMousePressFrame[MouseKindMax];//�ȑO�̃}�E�X�̃{�^���������Ă���t���[�������Ǘ�

int NowWheelValue;//���݂̃}�E�X�̃z�C�[����]��
int OldWheelValue;//�ȑO�̃}�E�X�̃z�C�[����]��

//�}�E�X�̃{�^���R�[�h�ƃt���[�������Ǘ����Ă���z��̗v�f����R�t����
int MouseCodeIndex[MouseKindMax]
{
	MOUSE_INPUT_LEFT,//�z��̗v�f����0�ɂ̓}�E�X�{�^������0*0001�̏��������
	MOUSE_INPUT_RIGHT,//�z��̗v�f����1�ɂ̓}�E�X�{�^������0*0002�̏��������
	MOUSE_INPUT_MIDDLE,//�z��̗v�f����2�ɂ̓}�E�X�{�^������0*0004�̏��������
	MOUSE_INPUT_4,//�z��̗v�f����3�ɂ̓}�E�X�{�^������0*0008�̏��������
	MOUSE_INPUT_5,//�z��̗v�f����4�ɂ̓}�E�X�{�^������0*0010�̏��������
	MOUSE_INPUT_6,//�z��̗v�f����5�ɂ̓}�E�X�{�^������0*0020�̏��������
	MOUSE_INPUT_7,//�z��̗v�f����6�ɂ̓}�E�X�{�^������0*0040�̏��������
	MOUSE_INPUT_8//�z��̗v�f����7�ɂ̓}�E�X�{�^������0*0080�̏��������
};

//Now???�n�̕ϐ��̒l��Old???�n�̕ϐ��ϐ��ւ����
VOID MouseNowIntoOld(VOID)
{
	OldPoint = NowPoint;//�}�E�X�̈ʒu

	//�t���[����
	for (int i = 0; i < MouseKindMax; i++)
	{
		OldMousePressFrame[i] = NowMousePressFrame[i];
	}

	//�z�C�[����
	OldWheelValue = NowWheelValue;

	return;
}

//�}�E�X�̃{�^���R�[�h��z��̗v�f���ɕϊ�����
int MouseCodeToIndex(int MOUSE_INPUT_)
{
	for (int i = 0; i < MouseKindMax; i++)
	{
		//�}�E�X�R�[�h���R�t������Ă���Ȃ�
		if (MouseCodeIndex[i] == MOUSE_INPUT_)
		{
			return i;//���̗v�f����Ԃ�
		}
	}

	//�}�E�X�R�[�h���Ȃ��Ȃ�G���[!
	return MouseCodeErrIndex;
}

//�}�E�X�����̏�����
VOID MouseInit(VOID)
{
	//�}�E�X�̈ʒu
	NowPoint.x = 0;
	NowPoint.y = 0;

	//�t���[����
	for (int i = 0; i < MouseKindMax; i++)
	{
		 NowMousePressFrame[i]=0;
	}

	//�z�C�[����
    NowWheelValue=0;

	//Old�n��������
	MouseNowIntoOld();

	return;
}

//�}�E�X�����̍X�V
VOID MouseUpdate(VOID)
{
	//�}�E�X�̃{�^�����������ׂĎ擾
	int Input;

	//���݂̏����ȑO�̏��Ƃ��ĕۑ�
	MouseNowIntoOld();

	//���݂̃}�E�X�̃N���C�A���g���W�̈ʒu���擾(�ߋ�)
	//GetMousePoint(&NowPoint.x, &NowPoint.y);

	int GetX, GetY;//�擾�p��XY���W

	//���݂̃}�E�X�̃N���C�A���g���W�̈ʒu���擾
	GetMousePoint(&GetX, &GetY);

	//�}�E�X�̍��W�ɓ����
	NowPoint = GetPoint(GetX, GetY);

	//�����A�}�E�X�̍��W���Q�[����ʊO�ɂ���Ȃ�Q�[����ʓ��Ɏ��߂�
	if (NowPoint.x < 0) { NowPoint.x = 0; }//��
	else if (NowPoint.x > GameWidth) { NowPoint.x = GameWidth; }//�E
	if (NowPoint.y < 0) { NowPoint.y = 0; }//��
	else if (NowPoint.y > GameHeight) { NowPoint.y = GameHeight; }//��

	//�}�E�X�̃{�^��������C�Ɏ擾
	Input = GetMouseInput();

	//�e�{�^���������Ă��邩�`�F�b�N
	for (int i = 0; i < MouseKindMax; i++)
	{
		if ((Input & MouseCodeIndex[i]) == MouseCodeIndex[i])
		{
			//���݉����Ă���{�^���̃t���[�������J�E���g�A�b�v
			NowMousePressFrame[i]++;
		}
		else if ((Input & MouseCodeIndex[i]) != MouseCodeIndex[i])
		{
			//���݉����Ă���{�^���̃t���[�������[���N���A
			NowMousePressFrame[i]=0;
		}
	}

	//�z�C�[���̉�]�ʂ��擾
	NowWheelValue = GetMouseWheelRotVol();

	return;
}

//����̃}�E�X�̃{�^�������������H
//����:DX���C�u�����[�̃}�E�X�R�[�h(MOUSE_INPUT_�Ŏn�܂�}�N����`)
BOOL MouseDown(int MOUSE_INPUT_)
{
   //�}�E�X�R�[�h��z��̗v�f���ɕϊ�����
	int index = MouseCodeToIndex(MOUSE_INPUT_);

	//���݂���}�E�X�R�[�h�Ȃ�
	if (index != MouseCodeErrIndex)
	{
		//���݉����Ă���Ȃ�
		if (NowMousePressFrame[index] > 0)
		{
			return TRUE;//�����Ă���
		}
	}

	return FALSE;//�����Ă��Ȃ�
}

//����̃}�E�X�̃{�^�����N���b�N���H
//����:DX���C�u�����[�̃}�E�X�R�[�h(MOUSE_INPUT_�Ŏn�܂�}�N����`)
//����:�u�{�^���������{�{�^����������v���Z�b�g�ōs��ꂽ�Ƃ����A�N���b�N
BOOL MouseClick(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h��z��̗v�f���ɕϊ�����
	int index = MouseCodeToIndex(MOUSE_INPUT_);

	//���݂���}�E�X�R�[�h�Ȃ�
	if (index != MouseCodeErrIndex)
	{
		if (NowMousePressFrame[index] == 0//���݉����Ă��Ȃ���
			&& OldMousePressFrame[index] > 0)//�ȑO�͉����Ă���
		{
			return TRUE;//�����Ă���
		}
	}

	return FALSE;//�����Ă��Ȃ�
}

//����̃}�E�X���������t���[����
//����:DX���C�u�����[�̃}�E�X�R�[�h(MOUSE_INPUT_�Ŏn�܂�}�N����`)
//����:�߂�l�̓~���b�Ȃǂł͂Ȃ��A�t���[����!
int MousePressFrame(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h��z��̗v�f���ɕϊ�����
	int index = MouseCodeToIndex(MOUSE_INPUT_);

	//���݂���}�E�X�R�[�h�Ȃ�
	if (index != MouseCodeErrIndex)
	{
		return NowMousePressFrame[MOUSE_INPUT_];
	}

	return 0;
}

//�}�E�X�̌��݂̈ʒu���擾����
POINT GetPointMouse(VOID)
{
	return NowPoint;
}

//�}�E�X�̈ȑO�̈ʒu���擾����
POINT GetOldPointMouse(VOID)
{
	return OldPoint;
}

 //�}�E�X�̈ȑO�ƌ��݂̈ʒu�̍����擾����
POINT GetDiffPointMouse(VOID)
{
	POINT diff;
	diff.x = OldPoint.x - NowPoint.x;
	diff.y = OldPoint.y - NowPoint.y;
	return diff;
}

//�}�E�X�̌��݂̃z�C�[���ʂ��擾����
int GetWheelMouse(VOID)
{
	return NowWheelValue;
}

//��`���Ń}�E�X�̍��W�͓������Ă��邩�H
BOOL CollRectToMouse(RECT rect)
{
	return CollRectToPoint(rect, NowPoint);
}

//��`�ƃ}�E�X�̃{�^�������������H
BOOL CollRectToMouseDown(RECT rect, int MOUSE_INPUT_)
{
	//��`����
	if (CollRectToMouse(rect) == TRUE)
	{
		//�{�^������������?
		if (MouseDown(MOUSE_INPUT_) == TRUE)
		{
			//������
			return TRUE;
		}
	}
	//�����ĂȂ�
	return FALSE;
}

//��`�ƃ}�E�X�̃{�^�����N���b�N�������H
BOOL CollRectToMouseClick(RECT rect, int MOUSE_INPUT_)
{
	//��`����
	if (CollRectToMouse(rect) == TRUE)
	{
		//�{�^�����N���b�N������
		if (MouseClick(MOUSE_INPUT_) == TRUE)
		{
			//�N���b�N����
			return TRUE;
		}
	}
	//�N���b�N���ĂȂ�
	return FALSE;
}

//�~���Ń}�E�X�̍��W�͓������Ă��邩�H
BOOL CollCircleToMouse(CIRCLE circle)
{
	return CollCircletoPoint(circle, NowPoint);
}

//�~�ƃ}�E�X�̓_���������Ă��邩�H
BOOL CollCircleToMousePoint(CIRCLE circle)
{
	//�~�Ɠ_�̓����蔻��̌��ʂ����̂܂ܕԂ�
	return CollCircleToMouse(circle);
}

//�~���ƃ}�E�X�̃{�^�������������H
BOOL CollCircleToMouseDown(CIRCLE circle, int MOUSE_INPUT_)
{
	//�~��
	if (CollCircleToMouse(circle) == TRUE)
	{
		//�{�^������������?
		if (MouseDown(MOUSE_INPUT_) == TRUE)
		{
			//������	
			return TRUE;
		}
	}
	//�����ĂȂ�
	return FALSE;

}

//�~���ƃ}�E�X�̃{�^�����N���b�N�������H
BOOL CollCircleToMouseClick(CIRCLE circle, int MOUSE_INPUT_)
{
	//�~��
	if (CollCircleToMouse(circle) == TRUE)
	{
		//�{�^�����N���b�N������
		if (MouseClick(MOUSE_INPUT_) == TRUE)
		{
			//�N���b�N����
			return TRUE;
		}
	}
	//�N���b�N���Ȃ�
	return FALSE;

}