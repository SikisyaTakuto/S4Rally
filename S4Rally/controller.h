#pragma once

#include "DxLib.h"
#include "geometory.h"
#include "game.h"

// XInputボタン入力定義
#define XINPUT_BUTTON_DPAD_UP (0)	     // デジタル方向ボタン上
#define XINPUT_BUTTON_DPAD_DOWN (1)	     // デジタル方向ボタン下
#define XINPUT_BUTTON_DPAD_LEFT	(2)	     // デジタル方向ボタン左
#define XINPUT_BUTTON_DPAD_RIGHT (3)	 // デジタル方向ボタン右
#define XINPUT_BUTTON_START	(4)	         // STARTボタン
#define XINPUT_BUTTON_BACK (5)	         // BACKボタン
#define XINPUT_BUTTON_LEFT_THUMB (6)	 // 左スティック押し込み
#define XINPUT_BUTTON_RIGHT_THUMB (7)	 // 右スティック押し込み
#define XINPUT_BUTTON_LEFT_SHOULDER	(8)	 // LBボタン
#define XINPUT_BUTTON_RIGHT_SHOULDER (9) // RBボタン
#define XINPUT_BUTTON_A	(12)	         // Aボタン
#define XINPUT_BUTTON_B	(13)	         // Bボタン
#define XINPUT_BUTTON_X	(14)	         // Xボタン
#define XINPUT_BUTTON_Y	(15)	         // Yボタン

// XInput のジョイパッド入力情報
typedef struct _XINPUT_STATE
{
    unsigned char	Buttons[16];	// ボタン１６個( 添字には XINPUT_BUTTON_DPAD_UP 等を使用する、0:押されていない  1:押されている )
    unsigned char	LeftTrigger;	// 左トリガー( 0～255 )
    unsigned char	RightTrigger;	// 右トリガー( 0～255 )
    short		ThumbLX;		    // 左スティックの横軸値( -32768 ～ 32767 )
    short		ThumbLY;		    // 左スティックの縦軸値( -32768 ～ 32767 )
    short		ThumbRX;		    // 右スティックの横軸値( -32768 ～ 32767 )
    short		ThumbRY;		    // 右スティックの縦軸値( -32768 ～ 32767 )

}XINPUTSTATE;

// 定数: ボタンIDの定義（UnityのInput.GetKeyに似た仕組みを模倣）
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
    BUTTON_COUNT // ボタン数
};

//外部グローバル変数
extern XINPUTSTATE padState;

// 外部プロトタイプ宣言
extern VOID ControllerInit(VOID);                   //コントローラーの初期化
extern VOID ControllerUpdate(VOID);                 //ボタンの更新
extern BOOL GetButtonDown(ControllerButton button); //ボタンが押された瞬間
extern BOOL GetButton(ControllerButton button);     //ボタンが押されている
extern BOOL GetButtonUp(ControllerButton button);   //ボタンが離された瞬間
extern FLOAT GetAxisX(VOID);                        //アナログスティックのX軸の値を取得
extern FLOAT GetAxisY(VOID);                        //アナログスティックのY軸の値を取得