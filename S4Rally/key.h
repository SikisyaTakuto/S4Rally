//キーボード処理のヘッダファイル
#pragma once

//ヘッダファイル
#include"Dxlib.h"

//マクロ定義
#define KeyKindMax 256//キーの種類

//外部プロトタイプ宣言
extern VOID KeyInit(VOID);//キーボード処理の初期化
extern VOID KeyUpdate(VOID);//キーボード処理の更新
extern BOOL KeyDown(int KEY_INPUT_);//特定のキーを押したか？
extern BOOL KeyClick(int KEY_INPUT_);//特定のキーをクリックしたか？
extern int KeyPressFrame(int KEY_INPUT_);//特定のキーを押したフレーム数