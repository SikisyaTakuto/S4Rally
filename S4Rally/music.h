//音楽のヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"

//エラー
#define MusicLoadErrCap		"が読み込めませんでした"
#define MusicLoadErrTitle	"音楽読み込みエラー"

//関数形式マクロ
#define GetVolume(volume)	(255 * volume / 100)	//ボリュームをMIN:0～MAX:100からMIN:0～MAX:255にする

//文字列の長さ
#define MusicPathStrMax 256
#define MusicErrStrMax 512

//デフォルトの音量
#define VolumeBGM	50	//BGMは控えめに
#define VolumeSE	100	//SEはドーンと！

//音楽ファイルのパス
//※日本語や絵文字など2バイト文字を避けること(最悪文字化けで読み込めない時がある)
//※全角空白、半角空白問わず空白を避けること(ファイル名の区切りだと思われることもある）

#define MusicPathTitleBGM		"./music/bgm/maou_game_town04.mp3"
#define MusicPathPlayBGM		"./music/bgm/maou_game_medley02.mp3"
#define MusicPathButtonClickSE	"./music/se/maou_se_system47.mp3"
/*
#define MusicPathResultBGM		".\\music\\bgm\\maou_19_12345.mp3"
#define MusicPathButtonClickSE	".\\music\\se\\buttonClickSE.mp3"
#define MusicPathJumpSE			".\\music\\se\\jumpSE.mp3"
#define MusicPathExplSE			".\\music\\se\\explSE.mp3"
#define MusicPathAvoidSE		".\\music\\se\\avoidSE.mp3"
*/

//音楽構造体
typedef struct _Music
{
	int Handle = -1;					//画像ハンドル
	char Path[MusicPathStrMax];			//パス
	int Volume = -1;					//音量
	BOOL IsPlayStart = TRUE;			//停止後は最初から再生するか？
	int PlayType = DX_PLAYTYPE_BACK;	//再生形式(デフォルトは1回のみ)

	int FadeInMillTime = 0;				//フェードイン秒
	int FadeOutMillTime = 0;			//フェードアウト秒

	int FadeInMillTimeMax = 0;			//フェードインMAX秒
	int FadeOutMillTimeMax = 0;			//フェードアウトMAX秒

	BOOL FadeInStartFlg = FALSE;		//フェードイン開始フラグ
	BOOL FadeOutStartFlg = FALSE;		//フェードイン終了フラグ
}Music;

//外部グローバル変数
extern Music TitleBGM;		//タイトルの音楽
extern Music PlayBGM;		//プレイの音楽
extern Music ResultBGM;		//リザルトの音楽

extern Music ButtonClickSE;	//ボタンクリック音
extern Music JumpSE;		//ジャンプ音
extern Music ExplSE;		//爆発音
extern Music AvoidSE;		//避ける音

//外部プロトタイプ宣言
extern BOOL Music_Init(VOID);										//読み込み・ハンドル生成
extern VOID Music_End(VOID);										//後始末
extern VOID MusicLoadErrMessage(const char* path);					//読み込みエラーメッセージ

extern Music LoadMusic(const char* path, int volume, int playType);	//読み込み
extern VOID PlayMusic(Music music);									//再生
extern VOID PauseMusic(Music* music);								//一時停止
extern VOID StopMusic(Music* music);								//停止
extern VOID DeleteMusic(Music music);								//解放

extern VOID ChangeVolumeMusic(Music* music, int volume);			//音量変更

extern BOOL FadeInPlayMusic(Music* music, int EffectTime);			//フェードイン
extern BOOL FadeOutPlayMusic(Music* music, int EffectTime);			//フェードアウト