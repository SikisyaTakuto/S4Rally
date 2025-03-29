//マウス処理のヘッダファイル
#pragma once

//ヘッダファイル
#include"Dxlib.h"
#include"geometory.h"

//マクロ定義
#define MouseKindMax 8//マウスのボタンの種類
#define MouseCodeErrIndex 999//マウスコードを変更した要素数のエラー値

//外部のプロトタイプ宣言
extern VOID MouseNowIntoOld(VOID);//Now???系の変数の値をOld???系の変数にいれる
extern int MouseCodeToIndex(int MOUSE_INPUT_);//マウスのボタンコードを配列の要素数に変換する

extern VOID MouseInit(VOID);//マウス処理の初期化
extern VOID MouseUpdate(VOID);//マウス処理の更新
extern BOOL MouseDown(int MOUSE_INPUT_);//特定のボタンを押したか？
extern BOOL MouseClick(int MOUSE_INPUT_);//特定のボタンをクリックしたか？
extern int MousePressFrame(int MOUSE_INPUT_);//特定のボタンを押したフレーム数

extern POINT GetPointMouse(VOID);//マウスの現在の位置を取得する
extern POINT GetOldPointMouse(VOID);//マウスの以前の位置を取得する
extern POINT GetDiffPointMouse(VOID);//マウスの以前と現在の位置の差を取得する
extern int GetWheelMouse(VOID); //マウスの現在のホイール量を取得する

extern BOOL CollRectToMouse(RECT rect);//矩形とマウスの点が当たっているか？
extern BOOL CollRectToMouseDown(RECT rect, int MOUSE_INPUT_);//矩形とマウスのボタンを押したか？
extern BOOL CollRectToMouseClick(RECT rect, int MOUSE_INPUT_);//矩形とマウスのボタンをクリックしたか？

extern BOOL CollCircleToMouse(CIRCLE circle);//円とマウスの点が当たっているか？
extern BOOL CollCircleToMouseDown(CIRCLE circle, int MOUSE_INPUT_);//円内とマウスのボタンを押したか？
extern BOOL CollCircleToMouseClick(CIRCLE circle, int MOUSE_INPUT_);//円内とマウスのボタンをクリックしたか？