#include "controller.h"

int controllerStateNow[BUTTON_COUNT];  // ���݂̃{�^�����
int controllerStatePrev[BUTTON_COUNT]; // 1�t���[���O�̃{�^�����
int analogX;                           // �A�i���O�X�e�B�b�NX��
int analogY;                           // �A�i���O�X�e�B�b�NY��
int triggerLT;                         // ���g���K�[�l (0�`255)
int triggerRT;                         // �E�g���K�[�l (0�`255)

float controllerCirclePointX = 0.0f;
float controllerCirclePointY = 0.0f;

//�R���g���[���[�̏�����
VOID ControllerInit(VOID) {
	for (int i = 0; i < BUTTON_COUNT; ++i)
	{
		controllerStateNow[i] = 0;
		controllerStatePrev[i] = 0;
	}
	analogX = 0;
	analogY = 0;
    triggerLT = 0;
    triggerRT = 0;
}

//�{�^���̍X�V
VOID ControllerUpdate(VOID)
{
    // 1�t���[���O�̏�Ԃ�ۑ�
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        controllerStatePrev[i] = controllerStateNow[i];
    }

    // XInput �̃W���C�p�b�h���͏����擾
    XINPUT_STATE padState;
    if (GetJoypadXInputState(DX_INPUT_PAD1, &padState) != 0) {
        // �W���C�p�b�h��񂪎擾�ł��Ȃ��ꍇ�͓��͏�Ԃ����Z�b�g
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            controllerStateNow[i] = 0;
        }
        return;
    }

    // �{�^����Ԃ��X�V
    controllerStateNow[BUTTON_A] = padState.Buttons[XINPUT_BUTTON_A];
    controllerStateNow[BUTTON_B] = padState.Buttons[XINPUT_BUTTON_B];
    controllerStateNow[BUTTON_X] = padState.Buttons[XINPUT_BUTTON_X];
    controllerStateNow[BUTTON_Y] = padState.Buttons[XINPUT_BUTTON_Y];
    controllerStateNow[BUTTON_LB] = padState.Buttons[XINPUT_BUTTON_LEFT_SHOULDER];
    controllerStateNow[BUTTON_RB] = padState.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER];
    controllerStateNow[BUTTON_START] = padState.Buttons[XINPUT_BUTTON_START];
    controllerStateNow[BUTTON_BACK] = padState.Buttons[XINPUT_BUTTON_BACK];

    // �g���K�[�l���X�V (�A�i���O)
    triggerLT = padState.LeftTrigger;
    triggerRT = padState.RightTrigger;

    // �g���K�[���{�^���Ƃ��Ĉ��� (128�ȏ�ŉ�������)
    controllerStateNow[BUTTON_LT] = (triggerLT > 128) ? 1 : 0;
    controllerStateNow[BUTTON_RT] = (triggerRT > 128) ? 1 : 0;

    // �A�i���O�X�e�B�b�N�̒l���擾
    analogX = padState.ThumbLX;
    analogY = padState.ThumbLY;

}

// �{�^���������ꂽ�u��
BOOL GetButtonDown(ControllerButton button) {
    return (controllerStateNow[button] == 1 && controllerStatePrev[button] == 0);
}

// �{�^����������Ă���
BOOL GetButton(ControllerButton button) {
    return controllerStateNow[button] == 1;
}

// �{�^���������ꂽ�u��
BOOL GetButtonUp(ControllerButton button) {
    return (controllerStateNow[button] == 0 && controllerStatePrev[button] == 1);
}

// ���X�e�B�b�NX��
FLOAT GetAxisX(VOID) {
    return analogX / 32767.0f; // -1.0 �` 1.0 �͈̔͂ɐ��K��
}

// ���X�e�B�b�NY��
FLOAT GetAxisY(VOID) {
    return analogY / 32767.0f; // -1.0 �` 1.0 �͈̔͂ɐ��K��
}