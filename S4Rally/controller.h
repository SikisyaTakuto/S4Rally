#pragma once

#include "DxLib.h"
#include "geometory.h"
#include "game.h"

// XInput�{�^�����͒�`
#define XINPUT_BUTTON_DPAD_UP (0)	     // �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_DOWN (1)	     // �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_LEFT	(2)	     // �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_RIGHT (3)	 // �f�W�^�������{�^���E
#define XINPUT_BUTTON_START	(4)	         // START�{�^��
#define XINPUT_BUTTON_BACK (5)	         // BACK�{�^��
#define XINPUT_BUTTON_LEFT_THUMB (6)	 // ���X�e�B�b�N��������
#define XINPUT_BUTTON_RIGHT_THUMB (7)	 // �E�X�e�B�b�N��������
#define XINPUT_BUTTON_LEFT_SHOULDER	(8)	 // LB�{�^��
#define XINPUT_BUTTON_RIGHT_SHOULDER (9) // RB�{�^��
#define XINPUT_BUTTON_A	(12)	         // A�{�^��
#define XINPUT_BUTTON_B	(13)	         // B�{�^��
#define XINPUT_BUTTON_X	(14)	         // X�{�^��
#define XINPUT_BUTTON_Y	(15)	         // Y�{�^��

// XInput �̃W���C�p�b�h���͏��
typedef struct _XINPUT_STATE
{
    unsigned char	Buttons[16];	// �{�^���P�U��( �Y���ɂ� XINPUT_BUTTON_DPAD_UP �����g�p����A0:������Ă��Ȃ�  1:������Ă��� )
    unsigned char	LeftTrigger;	// ���g���K�[( 0�`255 )
    unsigned char	RightTrigger;	// �E�g���K�[( 0�`255 )
    short		ThumbLX;		    // ���X�e�B�b�N�̉����l( -32768 �` 32767 )
    short		ThumbLY;		    // ���X�e�B�b�N�̏c���l( -32768 �` 32767 )
    short		ThumbRX;		    // �E�X�e�B�b�N�̉����l( -32768 �` 32767 )
    short		ThumbRY;		    // �E�X�e�B�b�N�̏c���l( -32768 �` 32767 )

}XINPUTSTATE;

// �萔: �{�^��ID�̒�`�iUnity��Input.GetKey�Ɏ����d�g�݂�͕�j
enum ControllerButton
{
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_LB,
    BUTTON_RB,
    BUTTON_LT,
    BUTTON_RT,
    BUTTON_START,
    BUTTON_BACK,
    BUTTON_COUNT // �{�^����
};

//�O���O���[�o���ϐ�
extern XINPUTSTATE padState;

// �O���v���g�^�C�v�錾
extern VOID ControllerInit(VOID);                   //�R���g���[���[�̏�����
extern VOID ControllerUpdate(VOID);                 //�{�^���̍X�V
extern BOOL GetButtonDown(ControllerButton button); //�{�^���������ꂽ�u��
extern BOOL GetButton(ControllerButton button);     //�{�^����������Ă���
extern BOOL GetButtonUp(ControllerButton button);   //�{�^���������ꂽ�u��
extern FLOAT GetAxisX(VOID);                        //�A�i���O�X�e�B�b�N��X���̒l���擾
extern FLOAT GetAxisY(VOID);                        //�A�i���O�X�e�B�b�N��Y���̒l���擾