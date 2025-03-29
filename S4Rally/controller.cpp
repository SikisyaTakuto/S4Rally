#include "controller.h"

int controllerStateNow[BUTTON_COUNT];  // 現在のボタン状態
int controllerStatePrev[BUTTON_COUNT]; // 1フレーム前のボタン状態
int analogX;                           // アナログスティックX軸
int analogY;                           // アナログスティックY軸
int triggerLT;                         // 左トリガー値 (0～255)
int triggerRT;                         // 右トリガー値 (0～255)

float controllerCirclePointX = 0.0f;
float controllerCirclePointY = 0.0f;

//コントローラーの初期化
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

//ボタンの更新
VOID ControllerUpdate(VOID)
{
    // 1フレーム前の状態を保存
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        controllerStatePrev[i] = controllerStateNow[i];
    }

    // XInput のジョイパッド入力情報を取得
    XINPUT_STATE padState;
    if (GetJoypadXInputState(DX_INPUT_PAD1, &padState) != 0) {
        // ジョイパッド情報が取得できない場合は入力状態をリセット
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            controllerStateNow[i] = 0;
        }
        return;
    }

    // ボタン状態を更新
    controllerStateNow[BUTTON_A] = padState.Buttons[XINPUT_BUTTON_A];
    controllerStateNow[BUTTON_B] = padState.Buttons[XINPUT_BUTTON_B];
    controllerStateNow[BUTTON_X] = padState.Buttons[XINPUT_BUTTON_X];
    controllerStateNow[BUTTON_Y] = padState.Buttons[XINPUT_BUTTON_Y];
    controllerStateNow[BUTTON_LB] = padState.Buttons[XINPUT_BUTTON_LEFT_SHOULDER];
    controllerStateNow[BUTTON_RB] = padState.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER];
    controllerStateNow[BUTTON_START] = padState.Buttons[XINPUT_BUTTON_START];
    controllerStateNow[BUTTON_BACK] = padState.Buttons[XINPUT_BUTTON_BACK];

    // トリガー値を更新 (アナログ)
    triggerLT = padState.LeftTrigger;
    triggerRT = padState.RightTrigger;

    // トリガーをボタンとして扱う (128以上で押下判定)
    controllerStateNow[BUTTON_LT] = (triggerLT > 128) ? 1 : 0;
    controllerStateNow[BUTTON_RT] = (triggerRT > 128) ? 1 : 0;

    // アナログスティックの値を取得
    analogX = padState.ThumbLX;
    analogY = padState.ThumbLY;

}

// ボタンが押された瞬間
BOOL GetButtonDown(ControllerButton button) {
    return (controllerStateNow[button] == 1 && controllerStatePrev[button] == 0);
}

// ボタンが押されている
BOOL GetButton(ControllerButton button) {
    return controllerStateNow[button] == 1;
}

// ボタンが離された瞬間
BOOL GetButtonUp(ControllerButton button) {
    return (controllerStateNow[button] == 0 && controllerStatePrev[button] == 1);
}

// 左スティックX軸
FLOAT GetAxisX(VOID) {
    return analogX / 32767.0f; // -1.0 ～ 1.0 の範囲に正規化
}

// 左スティックY軸
FLOAT GetAxisY(VOID) {
    return analogY / 32767.0f; // -1.0 ～ 1.0 の範囲に正規化
}