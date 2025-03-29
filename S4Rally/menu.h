//メニューのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include"game.h"
#include"key.h"
#include"mouse.h"
#include"geometory.h"
#include"font.h"
#include "music.h"
#include "graphic.h"

//マクロ定義
#define MenuWidth 1280	//画面の幅
#define MenuHeight 720	//画面の高さ
#define MenuColor 32	//画面の色
#define MenuTitle "GameTitle"//画面タイトル
#define MenuVsync TRUE	//Vsyncを使うならTRUE
#define MenuIcon 999//画像アイコン

#define MenuDebug TRUE//デバックモード

#define MenuSceneCount 3//ゲームシーンの数
#define MenuSceneNameMax 20//ゲームシーン名の文字数MAX

#define MenuSceneChangeFrame 60//ゲームシーンの切り替えを可能にするフレーム数

//色系の名前
#define Color_brack GetColor(0,0,0)
#define Color_white GetColor(255,255,255)
#define Color_skyblue GetColor(135,206,235)
#define Color_tomato GetColor(255,99,71)
#define Color_lightyellow GetColor(255,255,224)
#define Color_lawngreen GetColor(124,252,0)
#define Color_pink GetColor(255,192,203)
#define Color_yellow GetColor(255,255,0)

enum MenuScene {
    Scene_Menu,    //メニュー画面
    Scene_Game,    //ゲーム画面
    Scene_Config,  //設定画面
};

//外部のグローバル変数
extern enum MenuScene MenuScene;//メニューシーン
extern enum MenuScene ReturnGameScene;//ゲームシーンに戻る

//外部のプロトタイプ宣言

extern VOID AllMenuInit(VOID);//メニュー画面初期化
extern HWND GetMenuHandle(VOID);//メニュー画面のハンドルを取得
extern RECT GetMenuWindow(VOID);//メニュー画面の矩形を取得

extern POINT GetMenuWindowCenter(VOID);//メニュー画面の上下左右の中央座標を取得

extern VOID MenuInit(VOID);//メニュー初期化
extern VOID MenuCtrl(VOID);//メニュー管理
extern VOID MenuProc(VOID);//メニュー処理
extern VOID MenuDraw(VOID);//メニュー描画