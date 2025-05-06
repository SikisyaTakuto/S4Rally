//画像処理のソースファイル

//ヘッダファイル読み込み
#include "graphic.h"

//グローバル変数

Graphic TitleBackImage;				//タイトル背景画像
Graphic TitleLogo;					//タイトルロゴ
/*
Graphic TitleButton;				//タイトルボタン
Graphic BackImageTurn;				//背景反転
Graphic BackGrass[BackGrassNum];	//草原
Graphic GameoverLogo;		//ゲームオーバーロゴ
Graphic NewrecordLogo;		//ニューレコードロゴ
*/
//ここまで

//読み込み・ハンドル生成
BOOL Graphic_Init(VOID)
{

	//タイトル背景画像
	TitleBackImage = LoadGraphic(
		GraphicPathTitleBackImage,
		GetPoint(0, 0),				//背景は左上から
		GetRect(0, 0, 0, 0),		//当たり判定の余白は無し
		GraAlignTopL); if (TitleBackImage.Handle == -1) { return FALSE; }

	
	//タイトルロゴ
	TitleLogo = LoadGraphic(
		GraphicPathTitleLogo,
		GetGameWindowCenter(),				//ロゴは画面中央から
		GetRect(0, 0, 0, 0),				//当たり判定の余白は無し
		GraAlignBottomC); if (TitleLogo.Handle == -1) { return FALSE; }


	////タイトルボタン
	//TitleButton = LoadGraphic(
	//	GraphicPathTitleButton,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y + 50),		//ボタンは下辺りから
	//	GetRect(10, 10, 10, 10),		//当たり判定を挟める
	//	GraAlignTopC); if (TitleButton.Handle == -1) { return FALSE; }

	////プレイ背景画像
	//BackImageTurn = TitleBackImage;
	//BackImageTurn.c.Pos.x = BackImageTurn.c.Pos.x + BackImageTurn.c.Width;
	//BackImageTurn.c.Pos.y = 0;

	////草原
	//BackGrass[0] = LoadGraphic(
	//	GraphicPathBackGrass,
	//	GetPoint(0, GameHeight),			//草原のY位置は画面下
	//	GetRect(0, 0, 0, 0),		//当たり判定の余白は無し
	//	GraAlignBottomL); if (BackGrass[0].Handle == -1) { return FALSE; }

	//for (int i = 1; i < BackGrassNum; i++)
	//{
	//	//情報をコピー
	//	BackGrass[i] = BackGrass[i - 1];

	//	//画像を横に並べる
	//	BackGrass[i].c.Pos.x = BackGrass[i - 1].c.Pos.x + BackGrass[i - 1].c.Width;
	//}

	////ゲームオーバーロゴ
	//GameoverLogo = LoadGraphic(
	//	GraphicPathGameOverLogo,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y),				//ロゴは画面中央から
	//	GetRect(0, 0, 0, 0),				//当たり判定の余白は無し
	//	GraAlignTopC); if (GameoverLogo.Handle == -1) { return FALSE; }

	////ニューレコードロゴ
	//NewrecordLogo = LoadGraphic(
	//	GraphicPathNewrecordLogo,
	//	GetPoint(GetGameWindowCenter().x, GetGameWindowCenter().y - 200),				//ロゴは画面中央から
	//	GetRect(0, 0, 0, 0),				//当たり判定の余白は無し
	//	GraAlignTopC); if (GameoverLogo.Handle == -1) { return FALSE; }

	return TRUE;
}

//後始末
VOID Graphic_End(VOID)
{

	DeleteGraphic(TitleBackImage);
	DeleteGraphic(TitleLogo);
	//DeleteGraphic(TitleButton);
	//DeleteGraphic(BackImageTurn);
	//for (int i = 0; i < BackGrassNum; i++) { DeleteGraphic(BackGrass[i]); }
	//DeleteGraphic(GameoverLogo);
	//DeleteGraphic(NewrecordLogo);


	return;
}

//読み込みエラーメッセージ
VOID GraphicLoadErrMessage(const char* path)
{
	//エラー用文字列
	char ErrStr[ImagePathStrMax];

	//エラー用文字列の内容を空にする
	for (int i = 0; i < ImagePathStrMax; i++) { ErrStr[i] = '\0'; }

	//エラー内容を連結して生成
	sprintf_s(ErrStr, sizeof(ErrStr), "%s%s", path, ImageLoadErrCap);

	//パスが間違っているなどでインストールできない場合はエラー
	MessageBox(
		GetMainWindowHandle(),		//ウィンドウハンドル
		ErrStr,						//エラー内容
		ImageLoadErrTitle,			//エラータイトル
		MB_OK);						//OKボタンのみ

	return;
}

//読み込み
Graphic LoadGraphic(const char* path, POINT point, RECT margin, Graphic_Align align)
{
	Graphic graphic;

	//ファイルの場所をコピー
	strcpy_s(graphic.c.Path, sizeof(graphic.c.Path), path);

	//画像をメモリに読み込み
	graphic.Handle = LoadGraph(graphic.c.Path);

	//読み込みエラー
	if (graphic.Handle == -1)
	{
		GraphicLoadErrMessage(graphic.c.Path);
	}
	else
	{
		//正常に読み込みができたらパラーメタ設定

		//幅と高さを取得
		GetGraphSize(graphic.Handle, &graphic.c.Width, &graphic.c.Height);

		//基準点
		graphic.c.Pos = point;

		//揃え方向
		graphic.c.Align = align;

		//当たり判定
		graphic.c.CollMargin = margin;	//余白を設定
		ReMakeCollRect(&graphic);		//当たり判定生成

		//画像を描画する
		graphic.c.IsDraw = TRUE;

		//エフェクト初期化
		graphic.c.IsFxStartFlg = FALSE;			//描画エフェクトフラグ初期化
		graphic.c.Fx = GraFxNone;				//通常描画
		graphic.c.FxMillTime = 0;				//エフェクト時間カウント
		graphic.c.FxMillTimeMax = 0;			//エフェクト時間カウントMAX
		graphic.c.FxInterMillTime = 0;			//エフェクト間隔カウント
		graphic.c.FxInterMillTimeMax = 0;		//エフェクト間隔カウントMAX
		graphic.c.Alpha = 1.0f;					//透明度初期化
	}

	return graphic;
}

//描画
VOID DrawGraphic(Graphic* graphic)
{
	//エフェクトがあるときは、エフェクト開始処理
	if (graphic->c.IsFxStartFlg == TRUE)
	{
		//描画エフェクト開始処理
		switch (graphic->c.Fx)
		{
		case GraFxBlick:	//点滅
			//エフェクト間隔が最大の時に透明度を切り替える
			if (graphic->c.FxInterMillTime == graphic->c.FxInterMillTimeMax)
			{
				if (graphic->c.Alpha == 1.0)
				{
					graphic->c.Alpha = 0.0f;		//透明にする
				}
				else if (graphic->c.Alpha == 0.0)
				{
					graphic->c.Alpha = 1.0f;		//不透明にする
				}
			}
			//透過処理
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeIn:
			//画像の透明度を計算
			graphic->c.Alpha
				= sin(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//経過時間から全体の％を計算
					* (DX_PI / 2));														//sin(90)=1として割合を計算
			//透過処理
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeOut:
			//画像の透明度を計算
			graphic->c.Alpha
				= cos(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//経過時間から全体の％を計算
					* (DX_PI / 2));														//cos(90)=1として割合を計算
			//透過処理
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxFadeInOut:
			//画像の透明度を計算
			graphic->c.Alpha
				= sin(
					(float)graphic->c.FxInterMillTime / graphic->c.FxInterMillTimeMax		//経過時間から全体の％を計算
					* DX_PI);															//sin(180)=0～1～0として割合を計算
			//透過処理
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphic->c.Alpha * 255);
			break;

		case GraFxNone:
			//何もしない
			break;
		default:
			break;
		}
		//エフェクト時間カウントアップ
		graphic->c.FxMillTime++;

		//エフェクト間隔カウントアップ
		graphic->c.FxInterMillTime++;
	}

	//描画できるときは,描画処理
	if (graphic->c.IsDraw == TRUE)
	{
		//基準をもとに描画
		switch (graphic->c.Align)
		{
		case GraAlignTopL:
			//上左を基準に描画
			DrawGraph(
				graphic->c.Pos.x,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignTopC:
			//上中央を基準に描画
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignTopR:
			//上右を基準に描画
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width,
				graphic->c.Pos.y,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomL:
			//下左を基準に描画
			DrawGraph(
				graphic->c.Pos.x,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomC:
			//下中央を基準に描画
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignBottomR:
			//下右を基準に描画
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width,
				graphic->c.Pos.y - graphic->c.Height,
				graphic->Handle, TRUE);
			break;

		case GraAlignAllCenter:
			//上下左右の中央を基準に描画
			DrawGraph(
				graphic->c.Pos.x - graphic->c.Width / 2,
				graphic->c.Pos.y - graphic->c.Height / 2,
				graphic->Handle, TRUE);
			break;

		default:
			break;
		}
	}

	//エフェクトがあるときは、エフェクト終了処理
	if (graphic->c.IsFxStartFlg == TRUE)
	{
		switch (graphic->c.Fx)
		{
		case GraFxBlick:
			//アルファブレンド(透過処理)終了
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeIn:
			//アルファブレンド(透過処理)終了
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeOut:
			//アルファブレンド(透過処理)終了
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxFadeInOut:
			//アルファブレンド(透過処理)終了
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case GraFxNone:
			//何もしない
			break;
		default:
			break;
		}

		//インターバルがMAXなら元に戻す
		if (graphic->c.FxInterMillTime > graphic->c.FxInterMillTimeMax)
		{
			graphic->c.FxInterMillTime = 0;
		}

		//エフェクトが無限継続ではないとき
		if (graphic->c.FxMillTimeMax != GraFxInfinity)
		{
			//エフェクト時間が終わりならばエフェクト終了
			if (graphic->c.FxMillTime > graphic->c.FxMillTimeMax)
			{
				graphic->c.IsFxStartFlg = FALSE;
			}

		}
	}

	//デバッグモードなら当たり判定を描画
	if (GameDebug == TRUE)
	{
		//描画できるときは
		if (graphic->c.IsDraw == TRUE)
		{
			////矩形を描画
			//DrawBox(
			//	graphic->c.Coll.left,
			//	graphic->c.Coll.top,
			//	graphic->c.Coll.right,
			//	graphic->c.Coll.bottom,
			//	GetColor(255, 0, 0),		//赤色
			//	FALSE);
		}
	}

	return;
}

//解放
VOID DeleteGraphic(Graphic graphic)
{
	//正常に読み込みができていれば
	if (graphic.Handle != -1)
	{
		//メモリから解放
		DeleteGraph(graphic.Handle);
	}

	return;
}

//当たり判定の矩形を再生成
VOID ReMakeCollRect(Graphic* graphic)
{
	//基準をもとに再生成
	switch (graphic->c.Align)
	{
	case GraAlignTopL:
		//上左を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignTopC:
		//上中央を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignTopR:
		//上右を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width + graphic->c.CollMargin.left,
			graphic->c.Pos.y + graphic->c.CollMargin.top,
			graphic->c.Pos.x - graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomL:
		//下左を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomC:
		//下中央を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignBottomR:
		//下右を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height + graphic->c.CollMargin.top,
			graphic->c.Pos.x - graphic->c.CollMargin.right,
			graphic->c.Pos.y - graphic->c.CollMargin.bottom
		);
		break;

	case GraAlignAllCenter:
		//中心を基準
		graphic->c.Coll = GetRect(
			graphic->c.Pos.x - graphic->c.Width / 2 + graphic->c.CollMargin.left,
			graphic->c.Pos.y - graphic->c.Height / 2 + graphic->c.CollMargin.top,
			graphic->c.Pos.x + graphic->c.Width / 2 + graphic->c.CollMargin.right,
			graphic->c.Pos.y + graphic->c.Height / 2 + graphic->c.CollMargin.bottom
		);
		break;

	default:
		break;
	}

	return;
}

//エフェクト開始
VOID GraphicFxStart(Graphic* graphic, Graphic_Fx Fx, int MillTime, int MillInterval)
{
	//エフェクトの秒数を初期化
	graphic->c.FxMillTime = 0;

	//もしエフェクトが無限継続なら、実際のMAX時間を計算させない
	if (MillTime == GraFxInfinity)
	{
		graphic->c.FxMillTimeMax = GraFxInfinity;
	}
	else
	{
		graphic->c.FxMillTimeMax = (MillTime / 1000.0f) * GameFPS;
	}

	//エフェクトの間隔を初期化
	graphic->c.FxInterMillTime = 0;
	graphic->c.FxInterMillTimeMax = (MillInterval / 1000.0f) * GameFPS;

	//エフェクトの種類を設定
	graphic->c.Fx = Fx;

	//エフェクト開始フラグON
	graphic->c.IsFxStartFlg = TRUE;

	return;
}