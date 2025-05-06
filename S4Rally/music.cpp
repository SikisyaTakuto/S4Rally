//音楽処理のソースファイル

//ヘッダファイル読み込み
#include "music.h"
#include "fps.h"

//グローバル変数
Music TitleBGM;			//タイトルの音楽
Music PlayBGM;			//プレイの音楽
Music ResultBGM;		//リザルトの音楽
Music ButtonClickSE;	//ボタンクリック音
//Music JumpSE;			//ジャンプ音
//Music ExplSE;			//爆発音
//Music AvoidSE;			//避ける音

//読み込み・ハンドル生成
BOOL Music_Init(VOID)
{

	//BGM
	TitleBGM = LoadMusic(MusicPathTitleBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (TitleBGM.Handle == -1) { return FALSE; }
	PlayBGM = LoadMusic(MusicPathPlayBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (PlayBGM.Handle == -1) { return FALSE; }
	
	//ResultBGM = LoadMusic(MusicPathResultBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (ResultBGM.Handle == -1) { return FALSE; }

	//SE
	ButtonClickSE = LoadMusic(MusicPathButtonClickSE, VolumeSE, DX_PLAYTYPE_BACK); if (ButtonClickSE.Handle == -1) { return FALSE; }

	return TRUE;
}

//後始末
VOID Music_End(VOID)
{
	DeleteMusic(TitleBGM);
	DeleteMusic(PlayBGM);
	DeleteMusic(ResultBGM);
	DeleteMusic(ButtonClickSE);

	return;
}

//読み込みエラーメッセージ
VOID MusicLoadErrMessage(const char* path)
{
	//エラー用文字列
	char ErrStr[MusicErrStrMax];

	//エラー用文字列の内容を空にする
	for (int i = 0; i < MusicErrStrMax; i++) { ErrStr[i] = '\0'; }	//ヌル文字(\0)で埋める

	//エラー内容を連結して生成
	sprintf_s(ErrStr, sizeof(ErrStr), "%s%s", path, MusicLoadErrCap);

	//※WinAPI関数
	//パスが間違っているなどでインストールできない場合はエラー
	MessageBox(
		GetMainWindowHandle(),		//ウィンドウハンドル
		ErrStr,						//エラー内容
		MusicLoadErrTitle,		//エラータイトル
		MB_OK);						//OKボタンのみ

	return;
}

//読み込み
//※ボリュームはMIN:0～MAX:100で指定すること
Music LoadMusic(const char* path, int volume, int playType)
{
	Music music;

	//ファイルの場所をコピー
	//music.Path = path;	//文字列なので,そのまま代入できない
	strcpy_s(music.Path, sizeof(music.Path), path);

	//音楽をメモリに読み込み
	music.Handle = LoadSoundMem(music.Path);

	//読み込みエラー
	if (music.Handle == -1)
	{
		MusicLoadErrMessage(music.Path);
	}
	else
	{
		//正常に読み込みができたらパラメータ設定
		music.Volume = volume;						//最大音量を指定
		ChangeVolumeMusic(&music, music.Volume);	//再生音量を設定
		music.PlayType = playType;					//再生形式指定
		music.IsPlayStart = TRUE;					//停止後は最初から生成する
	}

	return music;
}

//再生
VOID PlayMusic(Music music)
{
	switch (music.PlayType)
	{
	case DX_PLAYTYPE_BACK:
		//通常バックグラウンド再生

		//直ぐに再生する
		PlaySoundMem(music.Handle, DX_PLAYTYPE_BACK, music.IsPlayStart);
		break;

	case DX_PLAYTYPE_LOOP:
		//ループ再生

		//再生されていないときは
		if (CheckSoundMem(music.Handle) == 0)
		{
			//ループ再生する
			PlaySoundMem(music.Handle, DX_PLAYTYPE_LOOP, music.IsPlayStart);
		}
	default:
		break;
	}

	return;
}

//一時停止
VOID PauseMusic(Music* music)
{
	//停止後は今の再生位置から再生する
	music->IsPlayStart = FALSE;

	//再生されてるなら
	if (CheckSoundMem(music->Handle) != 0)
	{
		//音楽停止
		StopSoundMem(music->Handle);

		//マスタ音量に戻す
		ChangeVolumeSoundMem(GetVolume(music->Volume), music->Handle);

		//フェード初期化
		music->FadeInStartFlg = FALSE;
		music->FadeOutStartFlg = FALSE;
	}

	return;
}

//停止
VOID StopMusic(Music* music)
{
	//停止後は最初の位置から再生する
	music->IsPlayStart = TRUE;

	//再生されてるなら
	if (CheckSoundMem(music->Handle) != 0)
	{
		//音楽停止
		StopSoundMem(music->Handle);

		//マスタ音量に戻す
		ChangeVolumeSoundMem(GetVolume(music->Volume), music->Handle);

		//フェード初期化
		music->FadeInStartFlg = FALSE;
		music->FadeOutStartFlg = FALSE;
	}

	return;
}

//解放
VOID DeleteMusic(Music music)
{
	//正常に読み込みができていれば
	if (music.Handle != -1)
	{
		//メモリから解放
		DeleteSoundMem(music.Handle);
	}

	return;
}

//音量変更
//※ボリュームはMIN:0～MAX:100で指定すること
VOID ChangeVolumeMusic(Music* music, int volume)
{
	//音楽のマスター音量を借るのではなく、
	//再生されている音量を変更する
	if (volume <= music->Volume)
	{
		ChangeVolumeSoundMem(GetVolume(volume), music->Handle);
	}

	return;
}

//フェードイン
//※EffectTimeは発動する効果ミリ秒間
BOOL FadeInPlayMusic(Music* music, int EffectTime)
{
	//初めてのフェードイン処理の時
	if (music->FadeInStartFlg == FALSE)
	{
		//フェードインのミリ秒を初期化
		music->FadeInMillTime = 0;
		music->FadeInMillTimeMax = (EffectTime / 1000.0f) * GameFPS;

		//音量を0にする
		ChangeVolumeMusic(music, 0);

		//音楽再生
		PlayMusic(*music);	//*で間接参照して本体を持ってくる

		//フェードイン開始フラグON
		music->FadeInStartFlg = TRUE;
	}

	if (music->FadeInStartFlg == TRUE)
	{
		//フェードイン中
		if (music->FadeInMillTime <= music->FadeInMillTimeMax)
		{
			//効果時間経過
			music->FadeInMillTime++;

			//音量を上げる
			ChangeVolumeMusic(
				music,
				(float)music->FadeInMillTime / music->FadeInMillTimeMax	//経過時間÷MAX時間で全体の割合(%)を計算
				* music->Volume);										//%を整数に変換
		}
		else if (music->FadeInMillTime >= music->FadeInMillTimeMax)
		{
			//フェードイン完了
			return TRUE;
		}
	}

	return FALSE;
}

//フェードアウト
//※EffectTimeは発動する効果ミリ秒間
BOOL FadeOutPlayMusic(Music* music, int EffectTime)
{
	//初めてのフェードアウト処理の時
	if (music->FadeOutStartFlg == FALSE)
	{
		//フェードアウトのミリ秒を初期化
		music->FadeOutMillTime = 0;
		music->FadeOutMillTimeMax = (EffectTime / 1000.0f) * GameFPS;

		//音量をMAXにする
		ChangeVolumeMusic(music, music->Volume);

		//音楽再生
		PlayMusic(*music);	//*で間接参照して本体を持ってくる

		//フェードアウト開始フラグON
		music->FadeOutStartFlg = TRUE;
	}

	if (music->FadeOutStartFlg == TRUE)
	{
		//フェードアウト中
		if (music->FadeOutMillTime <= music->FadeOutMillTimeMax)
		{
			//効果時間経過
			music->FadeOutMillTime++;

			//音量を上げる
			ChangeVolumeMusic(
				music,
				(1 - (float)music->FadeOutMillTime / music->FadeOutMillTimeMax)	//MAXの割合(100% - 減らしたい%)
				* music->Volume);												//MAXの音量から
		}
		else if (music->FadeOutMillTime >= music->FadeOutMillTimeMax)
		{
			//フェードアウト完了
			return TRUE;
		}
	}

	return FALSE;
}