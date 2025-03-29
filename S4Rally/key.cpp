//キーボード処理のソースファイル

//ヘッダファイル読み込み
#include"key.h"

//グローバル変数

//現在のキーを押しているフレーム数
int NowKeyPressFrame[KeyKindMax];

//1つ前のキーを押しているフレーム数
int OldKeyPressFrame[KeyKindMax];

//関数

//キーボード処理の初期化
VOID KeyInit(VOID)
{
	//配列の初期化
	for (int i = 0; i < KeyKindMax; i++)
	{
		NowKeyPressFrame[i] = 0;
		OldKeyPressFrame[i] = 0;
	}

	return;
}

//キーボード処理の更新
VOID KeyUpdate(VOID)
{
	//一時的に保持するキー情報
	char KeyState[KeyKindMax];

	//すべてのキー情報を一度に取得
	GetHitKeyStateAll(&KeyState[0]);

	//キー情報を更新
	for (int i = 0; i < KeyKindMax; i++)
	{
		//1つ前のキー入力を保存
		OldKeyPressFrame[i] = NowKeyPressFrame[i];

		//現在,キーが押されているとき
		if (KeyState[i] != 0)
		{
			//押されているフレーム数をカウントアップ
			NowKeyPressFrame[i]++;
		}
		else if (KeyState[i] == 0)
		{
			//押されていないなら、フレーム数をゼロクリア
			NowKeyPressFrame[i]=0;
		}
	}

	return;
}

//特定のキーを押したか？
//引数:DXライブラリーのキーコード(KEY_INPUT_で始まるマクロ定義)
BOOL KeyDown(int KEY_INPUT_)
{
	//現在押されているキーのフレーム数が0よりも上なら
	if (NowKeyPressFrame[KEY_INPUT_] > 0)
	{
		return TRUE;//押した
	}

	return FALSE;//押していない
}

//特定のキーをクリックか？
//引数:DXライブラリーのキーコード(KEY_INPUT_で始まるマクロ定義)
//説明:「キーを押す＋キーをあげる」がセットで行われたときが、クリック
BOOL KeyClick(int KEY_INPUT_)
{
	//現在,キーは押されておらず　かつ
	//一つ前のキーは押されていた時
	if (NowKeyPressFrame[KEY_INPUT_] == 0&&
		OldKeyPressFrame[KEY_INPUT_] > 0)
	{
		return TRUE;//押した
	}

	return FALSE;//押していない
}

//特定のキーを押したフレーム数
//引数:DXライブラリーのキーコード(KEY_INPUT_で始まるマクロ定義)
//注意:戻り値はミリ秒などではなく、フレーム数!
int KeyPressFrame(int KEY_INPUT_)
{
	return NowKeyPressFrame[KEY_INPUT_];
}