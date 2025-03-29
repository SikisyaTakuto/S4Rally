//幾何学（図形）処理のヘッダファイル
#pragma once

//ヘッダファイル
#include"Dxlib.h"
#include<math.h>

//構造体
typedef struct _CIRCLE
{
	POINT pt;//円の中心
	float radius = 0.0f;//円の半径
}CIRCLE;//円

//外部プロトタイプ宣言

//点関係
extern POINT GetPoint(int x, int y);//x地点とy地点からPOINT型を一時的に取得
extern BOOL CollPointToPoint(POINT a, POINT b);//点と点が当たっているか？

//矩形(くけい＝四角)関係
extern RECT GetRect(int left, int top, int right, int bottom);//左上右下からRECT型を一時的に取得
extern BOOL CollRectToRect(RECT a, RECT b);//矩形と矩形が当たっているか？
extern BOOL CollRectToPoint(RECT rect,POINT pt);//矩形と点が当たっているか？
extern VOID DrawRect(RECT rect,unsigned int color,BOOL fill);//RECT型で矩形を描く

//円関係
extern CIRCLE GetCircle(POINT pt, float rad);//Circle型を一時的に取得
extern BOOL CollCircletoCircle(CIRCLE C1, CIRCLE C2);//円と円が当たっているか？
extern BOOL CollCircletoPoint(CIRCLE circle, POINT pt);//円と点が当たっているか？
extern VOID DrawEn(CIRCLE circle, unsigned int color, BOOL fill);//CIRCLE型で描く