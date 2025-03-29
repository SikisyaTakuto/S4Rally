#include "map.h"
#include <cfloat>

int Add, y, x, i;
Stage stage;

VOID MapInit(VOID)
{
    // ライトの初期化
    VECTOR lightDirection = VGet(0.0f, 10.0f, 10.0f); // ライトの方向
    SetLightDirection(lightDirection);


    stage.mapHandle = MV1LoadModel("fbx/Stage/SakuraStage4.fbx");
    stage.lockLeft=false;
    stage.lockRight = false;
    MV1SetPosition(stage.mapHandle, VGet(1000.0f,30.0f, 0.0f));

    // モデル全体のコリジョン情報を構築
    MV1SetupCollInfo(stage.mapHandle, -1, 16, 16, 16);

    // モデルの進行方向をセット
    Add = 8;

    // モデルの移動位置をセット
    y = 0;
    x = 250.0f;

	return VOID();
}
// マップの描画関数
VOID GraphDraw(VOID)
{
    //3Dモデルの当たり判定
    {
        MV1DrawModel(stage.mapHandle);

        //球体(タイヤ)の位置
        stage.frontLeftPos = VGet(car.frontLeft.center.x, car.frontLeft.center.y+35.0f, car.frontLeft.center.z);
        stage.frontRightPos = VGet(car.frontRight.center.x, car.frontRight.center.y + 35.0f, car.frontRight.center.z);
        stage.rearLeftPos = VGet(car.rearLeft.center.x, car.rearLeft.center.y + 35.0f, car.rearLeft.center.z);
        stage.rearRightPos = VGet(car.rearRight.center.x, car.rearRight.center.y + 35.0f, car.rearRight.center.z);

        //モデルと球(タイヤ)との当たり判定
        stage.frontLeftHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.frontLeftPos, 35.0f);
        stage.frontRightHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.frontRightPos, 35.0f);
        stage.rearLeftHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.rearLeftPos, 35.0f);
        stage.rearRightHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.rearRightPos, 35.0f);

        // 球の描画
        DrawSphere3D(stage.frontLeftPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.frontRightPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.rearLeftPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.rearRightPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);

        //当たり判定
        HitDetection();

        // 当たり判定情報の後始末
        MV1CollResultPolyDimTerminate(stage.frontLeftHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.frontRightHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.rearLeftHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.rearRightHitPolyDim);
    }

    //デバック時位置が分かるようにラインを引く
    {
        // 位置関係が分かるように地面にラインを描画する
        {
        	int i;
        	VECTOR Pos1;
        	VECTOR Pos2;

        	SetUseZBufferFlag(TRUE);

        	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, -100.0f, -LINE_AREA_SIZE / 2.0f);
        	Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, -100.0f, LINE_AREA_SIZE / 2.0f);
        	for (i = 0; i <= LINE_NUM; i++)
        	{
        		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
        		Pos1.x += LINE_AREA_SIZE / LINE_NUM;
        		Pos2.x += LINE_AREA_SIZE / LINE_NUM;
        	}

        	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, -100.0f, -LINE_AREA_SIZE / 2.0f);
        	Pos2 = VGet(LINE_AREA_SIZE / 2.0f, -100.0f, -LINE_AREA_SIZE / 2.0f);
        	for (i = -1; i < LINE_NUM; i++)
        	{
        		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
        		Pos1.z += LINE_AREA_SIZE / LINE_NUM;
        		Pos2.z += LINE_AREA_SIZE / LINE_NUM;
        	}

        	SetUseZBufferFlag(FALSE);
        }
    }

    return;
}

//モデルと球体の当たり判定
VOID HitDetection(VOID) 
{
    int yOffset = BASE_Y_POS; // 描画位置のオフセット

    // 車の向きを取得
    MATRIX carMatrix = MGetRotY(carInfo.rotation.y);

    // Z軸の方向（前方向）
    VECTOR carForward = VTransform(VGet(0.0f, 0.0f, 1.0f), carMatrix);

    // X軸の方向（右方向）
    VECTOR carRight = VTransform(VGet(1.0f, 0.0f, 0.0f), carMatrix);

    // 当たったかどうかで処理を分岐
    if (stage.frontLeftHitPolyDim.HitNum >= 1)
    {
        // 当たったポリゴンの数を描画
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "frontLeft Hit Poly Num   %d", stage.frontLeftHitPolyDim);

        yOffset += Y_OFFSET;

        // 当たったポリゴンの数だけ繰り返し
        for (i = 0; i < stage.frontLeftHitPolyDim.HitNum; i++)
        {
            stage.frontLeftHitPos = stage.frontLeftHitPolyDim.Dim[i].HitPosition;  // **衝突座標を取得**
            stage.frontLeftNormal= stage.frontLeftHitPolyDim.Dim[i].Normal;  // **法線ベクトルを取得**

            // **床と壁の判定**
            if (stage.frontLeftNormal.y > 0.01f)
            {
                // **床に衝突**
                // **衝突座標を画面に表示**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "frontLeft Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                // **衝突座標に球を描画して視覚化**
                DrawSphere3D(stage.frontLeftHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **壁に衝突**
                // **衝突座標を画面に表示**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "frontLeft Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                // **衝突座標に球を描画して視覚化**
                DrawSphere3D(stage.frontLeftHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.frontLeftWallHit = true;

                // 法線と前方向の内積を計算
                float dot = VDot(stage.frontLeftNormal, carForward);
                float sideDot = VDot(stage.frontLeftNormal, carRight);

                if (dot > 0.7f)  // 正面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "正面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (dot < -0.7f)  // 背面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "背面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (sideDot > 0.7f)  // 右側面
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "右側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (sideDot < -0.7f)  // 左側面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "左側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else  // その他（斜め）
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "斜め: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
            }

            yOffset += Y_OFFSET;

        }
    }

    // 当たったかどうかで処理を分岐
    if (stage.frontRightHitPolyDim.HitNum >= 1)
    {
        // 当たったポリゴンの数を描画
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "frontRight Hit Poly Num   %d", stage.frontRightHitPolyDim);
        yOffset += Y_OFFSET;

        // 当たったポリゴンの数だけ繰り返し
        for (i = 0; i < stage.frontRightHitPolyDim.HitNum; i++)
        {
            stage.frontRightHitPos = stage.frontRightHitPolyDim.Dim[i].HitPosition;
            stage.frontRightNormal = stage.frontRightHitPolyDim.Dim[i].Normal;  // **法線ベクトルを取得**

            // **床と壁の判定**
            if (stage.frontRightNormal.y > 0.01f)
            {
                // **床に衝突**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "frontRight Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                DrawSphere3D(stage.frontRightHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **壁に衝突**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "frontRight Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                DrawSphere3D(stage.frontRightHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.frontRightWallHit = true;

                // 法線と前方向の内積を計算
                float dot = VDot(stage.frontRightNormal, carForward);
                float sideDot = VDot(stage.frontRightNormal, carRight);

                if (dot > 0.7f)  // 正面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "正面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (dot < -0.7f)  // 背面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "背面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (sideDot > 0.7f)  // 右側面
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "右側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (sideDot < -0.7f)  // 左側面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "左側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else  // その他（斜め）
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "斜め: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
            }

            yOffset += Y_OFFSET;

        }
    }

    // 当たったかどうかで処理を分岐
    if (stage.rearLeftHitPolyDim.HitNum >= 1)
    {
        // 当たったポリゴンの数を描画
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "rearLeft Hit Poly Num   %d", stage.rearLeftHitPolyDim);
        yOffset += Y_OFFSET;

        // 当たったポリゴンの数だけ繰り返し
        for (i = 0; i < stage.rearLeftHitPolyDim.HitNum; i++)
        {
            stage.rearLeftHitPos = stage.rearLeftHitPolyDim.Dim[i].HitPosition;
            stage.rearLeftNormal= stage.rearLeftHitPolyDim.Dim[i].Normal;  // **法線ベクトルを取得**

            // **床と壁の判定**
            if (stage.rearLeftNormal.y > 0.01f)
            {
                // **床に衝突**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "rearLeft Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                DrawSphere3D(stage.rearLeftHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **壁に衝突**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "rearLeft Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                DrawSphere3D(stage.rearLeftHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.rearLeftWallHit =true;

                // 法線と前方向の内積を計算
                float dot = VDot(stage.rearLeftNormal, carForward);
                float sideDot = VDot(stage.rearLeftNormal, carRight);

                if (dot > 0.7f)  // 正面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "正面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (dot < -0.7f)  // 背面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "背面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (sideDot > 0.7f)  // 右側面
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "右側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (sideDot < -0.7f)  // 左側面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "左側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else  // その他（斜め）
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "斜め: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
            }
            yOffset += Y_OFFSET;
        }
    }

    // 当たったかどうかで処理を分岐
    if (stage.rearRightHitPolyDim.HitNum >= 1)
    {
        // 当たったポリゴンの数を描画
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "rearRight Hit Poly Num   %d", stage.rearRightHitPolyDim);
        yOffset += Y_OFFSET;

        // 当たったポリゴンの数だけ繰り返し
        for (i = 0; i < stage.rearRightHitPolyDim.HitNum; i++)
        {
            stage.rearRightHitPos = stage.rearRightHitPolyDim.Dim[i].HitPosition;
            stage.rearRightNormal = stage.rearRightHitPolyDim.Dim[i].Normal;  // **法線ベクトルを取得**

            // **床と壁の判定**
            if (stage.rearRightNormal.y > 0.01f)
            {
                // **床に衝突**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "rearRight Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                DrawSphere3D(stage.rearRightHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **壁に衝突**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "rearRight Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                DrawSphere3D(stage.rearRightHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.rearRightWallHit =true;

                // 法線と前方向の内積を計算
                float dot = VDot(stage.rearRightNormal, carForward);
                float sideDot = VDot(stage.rearRightNormal, carRight);

                if (dot > 0.7f)  // 正面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "正面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (dot < -0.7f)  // 背面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "背面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (sideDot > 0.7f)  // 右側面
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "右側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (sideDot < -0.7f)  // 左側面
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "左側面: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else  // その他（斜め）
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "斜め: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
            }
        }
        yOffset += Y_OFFSET;
    }

    bool frontCollision = false;
    bool rearCollision = false;
    bool LeftCollision = false;
    bool RightCollision = false;

    // 各タイヤの当たり判定を確認
    struct CollisionPoint {
        MV1_COLL_RESULT_POLY_DIM& hitPolyDim;
        VECTOR& hitNormal;
        VECTOR& hitPos;
    };

    CollisionPoint collisionPoints[] = {
        { stage.frontLeftHitPolyDim, stage.frontLeftNormal, stage.frontLeftHitPos },
        { stage.frontRightHitPolyDim, stage.frontRightNormal, stage.frontRightHitPos },
        { stage.rearLeftHitPolyDim, stage.rearLeftNormal, stage.rearLeftHitPos },
        { stage.rearRightHitPolyDim, stage.rearRightNormal, stage.rearRightHitPos },
    };

    for (auto& point : collisionPoints) {
        if (point.hitPolyDim.HitNum > 0) {
            for (int i = 0; i < point.hitPolyDim.HitNum; i++) {
                point.hitPos = point.hitPolyDim.Dim[i].HitPosition;
                point.hitNormal = point.hitPolyDim.Dim[i].Normal;

                float forwardDot = VDot(point.hitNormal, carForward);
                float sideDot = VDot(point.hitNormal, carRight);

                if (forwardDot > 0.7f) { // 正面衝突
                    frontCollision = true;
                }
                else if (forwardDot < -0.7f) { // 背面衝突
                    rearCollision = true;
                }
                else if (sideDot > 0.7f) { // 横衝突
                    RightCollision = true;
                }
                else if (sideDot < -0.7f)
                {
                    LeftCollision = true;
                }
            }
        }
    }

    // 衝突による制限処理
    if (frontCollision) {
        carInfo.currentSpeed = 0.0f; // 前進を止める
    }
    if (rearCollision) {
        carInfo.currentSpeed =  0.0f; // 後退を止める
    }
    if (RightCollision) {
        stage.lockRight = true;
    }
    else
    {
         stage.lockRight = false;
    }
    if (LeftCollision)
    {
        stage.lockLeft = true;
    }
    else
    {
        stage.lockLeft = false;
    }
}