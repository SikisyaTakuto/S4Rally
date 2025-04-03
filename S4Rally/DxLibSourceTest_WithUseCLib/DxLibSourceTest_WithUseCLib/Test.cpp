#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int x, add ;

	// ウインドウモードで起動
	ChangeWindowMode( TRUE ) ;

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1 ;

	// 描画先を裏画面にする
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// メインループ
	x = 0 ;
	add = 8 ;
	while( ProcessMessage() == 0 )
	{
		// 移動
		x += add ;
		if( x < 0 || x > 640 ) add = -add ;

		// 画面のクリア
		ClearDrawScreen() ;

		// 四角形の描画
		DrawBox( x, 240 - 32, x + 64, 240 + 32, GetColor( 255,255,255 ), TRUE ) ;

		// 裏画面の内容を表画面に反映
		ScreenFlip() ;
	}

	// ＤＸライブラリの後始末
	DxLib_End() ;

	// ソフトの終了
	return 0 ;
}
