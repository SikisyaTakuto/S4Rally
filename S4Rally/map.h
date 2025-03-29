//マップ処理のヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "car.h"

#define LINE_AREA_SIZE 1000000.0f// ラインを描く範囲
#define LINE_NUM 1000            // ラインの数

#define MAP_SIZE	64			// マップチップ一つのドットサイズ
#define MAP_WIDTH	50			// マップの幅
#define MAP_HEIGHT	50			// マップの縦長さ

// マクロを定義して表示の開始位置を調整
#define BASE_Y_POS 90
#define Y_OFFSET 40

//構造体
typedef struct _Stage {

	int mapHandle;
	VECTOR SpherePos;							//球体の位置を決める
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;		//球体とモデルの当たり判定

	VECTOR frontLeftPos;			//前輪（右）の位置を決める
	VECTOR frontRightPos;			//前輪（左）の位置を決める
	VECTOR rearLeftPos;				//後輪（右）の位置を決める
	VECTOR rearRightPos;			//後輪（左）の位置を決める
	VECTOR carBodyPos1, carBodyPos2;//車体の位置を決める

    MV1_COLL_RESULT_POLY_DIM frontLeftHitPolyDim;  //前輪（右）の当たり判定
	MV1_COLL_RESULT_POLY_DIM frontRightHitPolyDim; //前輪（左）の当たり判定
    MV1_COLL_RESULT_POLY_DIM rearLeftHitPolyDim;   //後輪（右）の当たり判定
    MV1_COLL_RESULT_POLY_DIM rearRightHitPolyDim;  //後輪（左）の当たり判定
	MV1_COLL_RESULT_POLY_DIM carBodyHitPolyDim;	   //車体の当たり判定

	VECTOR frontLeftHitPos;		//前輪（右）の当たった座標を取得
	VECTOR frontRightHitPos;	//前輪（左）の当たった座標を取得
	VECTOR rearLeftHitPos;		//後輪（右）の当たった座標を取得
	VECTOR rearRightHitPos;		//後輪（左）の当たった座標を取得
	VECTOR carBodyHitPos;		//車体の当たった座標を取得

	VECTOR frontLeftNormal;		//前輪（右）の法線ベクトルを取得
	VECTOR frontRightNormal;	//前輪（左）の法線ベクトルを取得
	VECTOR rearLeftNormal;		//後輪（右）の法線ベクトルを取得
	VECTOR rearRightNormal;		//後輪（左）の法線ベクトルを取得
	VECTOR carBodyNormal;		//車体の法線ベクトルを取得

	bool frontLeftWallHit;   //前輪（右）の壁の当たり判定
	bool frontRightWallHit;  //前輪（左）の壁の当たり判定
	bool rearLeftWallHit;    //後輪（右）の壁の当たり判定
	bool rearRightWallHit;   //後輪（左）の壁の当たり判定
	bool carBodyWallHit;	 //車体の壁の当たり判定

	bool lockLeft;
	bool lockRight;

}Stage;

extern Stage stage;

extern VOID MapInit(VOID);      //初期化
extern VOID GraphDraw(VOID);	// マップとプレイヤーの描画関数
extern VOID HitDetection(VOID); //当たり判定