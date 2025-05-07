// Car処理のソースファイル
#include "car.h"

//外部参照
extern Map map;                    // マップ（地形や障害物情報）
RigidBody rigidbody;               // 車体の物理挙動を担当するクラス
SuspensionSpring suspensionSpring; // サスペンション用のスプリングクラス

//コンストラクタ
Car::Car()
    : motorForce(0.0f), steeringAngle(0.0f), carModelHandle(-1)
{
    // タイヤのモデルハンドルと位置・回転・オフセット初期化
    for (int i = 0; i < WheelNum; i++)
    {
        wheelModelHandles[i] = -1;
        wheelPositions[i] = VGet(0, 0, 0);
        wheelRotations[i] = VGet(0, 0, 0);
        wheelOffsets[i] = VGet(0, 0, 0);
    }
}

// デストラクタ
Car::~Car()
{
    // 車体モデルの削除
    if (carModelHandle != -1)
    {
        MV1DeleteModel(carModelHandle);// モデルなのでMV1DeleteModelを使う
    }

    // タイヤモデルの削除
    for (int i = 0; i < WheelNum; i++)
    {
        if (wheelModelHandles[i] != -1)
        {
            MV1DeleteModel(wheelModelHandles[i]);
        }
    }
}

void Car::Init()
{
    //モデルの読み込み
    carModelHandle = MV1LoadModel("fbx/Silvia/SILVIABody.fbx");
    if (carModelHandle == -1)
    {
        printfDx("Error: 車体モデル読み込み失敗！\n");
        return;
    }

    for (int i = 0; i < WheelNum; i++)
    {
        wheelModelHandles[i] = MV1LoadModel("fbx/Silvia/SILVIAWheel.fbx");
        if (wheelModelHandles[i] == -1)
        {
            printfDx("Error: タイヤモデル読み込み失敗！（%d番目）\n", i);
            return;
        }
    }

    //車体とRigidbodyの初期位置設定
    VECTOR startPos = VGet(3000.0f, -30.0f, -3000.0f);

    rigidbody.position = startPos;               // Rigidbodyにスタート位置を設定
    rigidbody.velocity = VGet(0.0f, 0.0f, 0.0f); // 速度も初期化
    rigidbody.isGravity = true;                  // 重力有効
    rigidbody.isGrounded = false;                // 地面に接地していない状態

    carBodyPosition = startPos;                  // 車体の位置もセット
    carBodyRotation = VGet(0.0f, 1.5f, 0.0f);    // 向きもリセット（正面向き）

    //タイヤオフセットの設定（車体基準座標）
    wheelOffsets[FrontLeft] = VGet(-75.0f, -1.0f, 145.0f); // 左前
    wheelOffsets[FrontRight] = VGet(75.0f, -1.0f, 145.0f); // 右前
    wheelOffsets[RearLeft] = VGet(-75.0f, -1.0f, -101.0f); // 左後ろ
    wheelOffsets[RearRight] = VGet(75.0f, -1.0f, -101.0f); // 右後ろ
}

void Car::Update(float deltaTime)
{
    WheelCollider wheels[WheelNum]; // タイヤごとの当たり判定用コライダー

    ControllerUpdate();        //コントローラーの入力状態更新
    float axisX = GetAxisX();  // アナログスティックの値を取得して表示

    // 入力初期化
    accelInput = 0.0f;
    steerInput = 0.0f;
    sideBrakeInput = 0.0f;

    //入力取得（キーボードまたはコントローラー）
    if (CheckHitKey(KEY_INPUT_W)|| GetButton(BUTTON_RT)) { accelInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_S)|| GetButton(BUTTON_LT)) { accelInput -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_D)|| axisX >= 0.5f) { steerInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_A)|| axisX <= -0.5f) { steerInput -= 1.0f; }
    //if (CheckHitKey(KEY_INPUT_SPACE)) { sideBrakeInput = 1.0f; } // サイドブレーキ入力

    // 車体の当たり判定処理
    if (CheckBodyCollision()) {
        // 前後左右それぞれのヒットフラグに応じて速度や入力を制御
        if (hitFront && accelInput > 0.0f) {
            // 正面かぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitBack && accelInput < 0.0f) {
            accelInput = 0.0f; // 後退を止める
            // 背面にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitLeft && steerInput < 0.0f) {
            // 左にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitRight && steerInput > 0.0f) {
            // 右にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }

        // ここで、フラグに応じて車の動きを制御できる
        if (hitFrontDeceleration && accelInput > 0.0f) {
            // 正面か背面にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitBackDeceleration && accelInput < 0.0f) {
            // 正面か背面にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitLeftDeceleration && steerInput < 0.0f) {
            // 左にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitRightDeceleration && steerInput > 0.0f) {
            // 右にぶつかったら速度を大幅に減速
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
    }

    float velocity = VSize(rigidbody.velocity); // 現在速度[m/s]取得

    //// エンジンとトランスミッション更新
    //transmission.Update(velocity);
    //engine.Update(velocity, transmission);

    //// エンジントルク
    float baseEngineForce = 100000.0f * accelInput;
    //float driveForce = transmission.CarculateEngineForce(baseEngineForce);

    // 車体への推進力適用
    VECTOR forward = VGet(sinf(carBodyRotation.y), 0, cosf(carBodyRotation.y));
    rigidbody.AddForce(VScale(forward, baseEngineForce * deltaTime));

    //地面の摩擦による減速処理
    rigidbody.velocity = VScale(rigidbody.velocity, 0.98f); // フリクション

    //ステアリング処理（速度に応じて旋回する）
    steeringAngle = 40.0f * steerInput * DX_PI_F / 180.0f; // ハンドル角（ラジアン）
    const float steeringSpeedThreshold = 2.0f;             // 旋回可能な最低速度[m/s]
    if (velocity > steeringSpeedThreshold)carBodyRotation.y += steeringAngle * deltaTime;// 車体回転

    //位置更新
    rigidbody.Update(deltaTime);
    carBodyPosition = rigidbody.position; // 車体の位置同期

    //タイヤの位置・回転更新
    for (int i = 0; i < WheelNum; i++)
    {
        VECTOR rotatedOffset = VTransform(wheelOffsets[i], MGetRotY(carBodyRotation.y));// 車体回転を考慮したタイヤ位置
        wheelPositions[i] = VAdd(carBodyPosition, rotatedOffset);

        wheelRotations[i] = VGet(0, carBodyRotation.y, 0);// タイヤ回転（車体回転に合わせる）

        // 前輪だけステアリング
        if (i == FrontLeft || i == FrontRight)
        {
            wheelRotations[i].y += steeringAngle;
        }

        //地面接触チェック
        VECTOR wheelPos = VAdd(carBodyPosition, wheelPositions[i]);
        VECTOR hitPoint, hitNormal;
        float compression;
        if (wheels[i].GetGroundHit(wheelPos, hitPoint, hitNormal, compression)) {
            rigidbody.isGravity = false;// 接地しているので重力をオフ
            rigidbody.isGrounded = true;
        }
        //else
        //{
        //    rigidbody.isGravity = true;
        //    rigidbody.isGrounded = false;
        //}

    }

   // DrawFormatString(10, 260, GetColor(255, 255, 255), "Speed: %.1f m/s", velocity);
}

// 描画処理
void Car::Draw()
{
    // 車体モデル描画
    if (carModelHandle != -1)
    {
        MV1SetPosition(carModelHandle, carBodyPosition);
        MV1SetRotationXYZ(carModelHandle, carBodyRotation);
        MV1DrawModel(carModelHandle);
    }

    // タイヤモデル描画
    for (int i = 0; i < WheelNum; i++)
    {
        if (wheelModelHandles[i] != -1)
        {
            MV1SetPosition(wheelModelHandles[i], wheelPositions[i]);
            MV1SetRotationXYZ(wheelModelHandles[i], wheelRotations[i]);
            MV1DrawModel(wheelModelHandles[i]);
        }
    }

    // デバッグ情報
    {
        // --- デバッグログ出力 ---
        //DrawFormatString(10, 200, GetColor(255, 255, 255), "Mode: %s", transmission.mode == Manual ? "Manual" : "Auto");
        //DrawFormatString(10, 220, GetColor(255, 255, 255), "Gear: %d", transmission.GetGear());
        //DrawFormatString(10, 240, GetColor(255, 255, 255), "Engine RPM: %.0f", engine.GetRPM());

        //char carPosLog[256];
        //sprintf_s(carPosLog, "Car Pos: X=%.2f Y=%.2f Z=%.2f", carBodyPosition.x, carBodyPosition.y, carBodyPosition.z);
        //DrawString(10, 10, carPosLog, GetColor(255, 255, 255));

        //char carRotLog[256];
        //sprintf_s(carRotLog, "Car Rot: X=%.2f Y=%.2f Z=%.2f", carBodyRotation.x, carBodyRotation.y, carBodyRotation.z);
        //DrawString(10, 30, carRotLog, GetColor(255, 255, 255));

        //char rigidBodyVelocity[256];
        //sprintf_s(rigidBodyVelocity, "rigidBody velocity: X=%.2f Y=%.2f Z=%.2f", rigidbody.velocity.x, rigidbody.velocity.y, rigidbody.velocity.z);
        //DrawString(10, 50, rigidBodyVelocity, GetColor(255, 255, 255));

        //DrawLine3D(frontStartDeceleration, frontEndDeceleration, GetColor(255, 0, 0)); // 前ライン 赤
        //DrawLine3D(backStartDeceleration, backEndDeceleration, GetColor(0, 255, 0));   // 後ライン 緑
        //DrawLine3D(leftStartDeceleration, leftEndDeceleration, GetColor(0, 0, 255));   // 左ライン 青
        //DrawLine3D(rightStartDeceleration, rightEndDeceleration, GetColor(255, 255, 0));// 右ライン 黄色

        //// --- 車体ボックス描画（線分）
        //// 上面
        //DrawLine3D(boxVertices[0], boxVertices[1], GetColor(255, 255, 255)); // 左前上 → 右前上
        //DrawLine3D(boxVertices[1], boxVertices[3], GetColor(255, 255, 255)); // 右前上 → 右後上
        //DrawLine3D(boxVertices[3], boxVertices[2], GetColor(255, 255, 255)); // 右後上 → 左後上
        //DrawLine3D(boxVertices[2], boxVertices[0], GetColor(255, 255, 255)); // 左後上 → 左前上

        //// 下面
        //DrawLine3D(boxVertices[4], boxVertices[5], GetColor(255, 255, 255)); // 左前下 → 右前下
        //DrawLine3D(boxVertices[5], boxVertices[7], GetColor(255, 255, 255)); // 右前下 → 右後下
        //DrawLine3D(boxVertices[7], boxVertices[6], GetColor(255, 255, 255)); // 右後下 → 左後下
        //DrawLine3D(boxVertices[6], boxVertices[4], GetColor(255, 255, 255)); // 左後下 → 左前下

        //// 縦の線
        //DrawLine3D(boxVertices[0], boxVertices[4], GetColor(255, 255, 255)); // 左前上 → 左前下
        //DrawLine3D(boxVertices[1], boxVertices[5], GetColor(255, 255, 255)); // 右前上 → 右前下
        //DrawLine3D(boxVertices[2], boxVertices[6], GetColor(255, 255, 255)); // 左後上 → 左後下
        //DrawLine3D(boxVertices[3], boxVertices[7], GetColor(255, 255, 255)); // 右後上 → 右後下
    }

}

// 車のレイキャスト（地面や壁とのヒットをチェックする関数）
bool Car::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal) {
    // 仮実装：車体位置を中心としたAABB（軸平行バウンディングボックス）を手動で作成
    VECTOR min = VSub(carBodyPosition, VGet(1.5f, 0.5f, 3.0f)); // 車体中心から幅・高さ・奥行き分だけ引いた最小点
    VECTOR max = VAdd(carBodyPosition, VGet(1.5f, 0.5f, 3.0f)); // 車体中心に幅・高さ・奥行きを加えた最大点s

    // レイ（直線）とAABBとの交差判定（スラブ法）
    float tmin = 0.0f;       // レイの最近接交点（初期値0）
    float tmax = maxDistance; // レイの最大到達距離（引数で指定された最大値）

    // x, y, z 各軸ごとに交差判定
    for (int i = 0; i < 3; ++i) {
        float originComp = (&origin.x)[i]; // originのx,y,z成分を順に取得
        float dirComp = (&direction.x)[i]; // directionのx,y,z成分を順に取得
        float minComp = (&min.x)[i];       // AABBの最小座標成分
        float maxComp = (&max.x)[i];       // AABBの最大座標成分

        if (fabs(dirComp) < 1e-6f) { // 方向ベクトルがほぼ0なら（軸に平行）
            if (originComp < minComp || originComp > maxComp) return false; // AABBの範囲外なら交差なし
        }
        else {
            float ood = 1.0f / dirComp; // 方向成分の逆数
            float t1 = (minComp - originComp) * ood; // 最小面との交点距離
            float t2 = (maxComp - originComp) * ood; // 最大面との交点距離

            if (t1 > t2) std::swap(t1, t2); // 必ず t1 <= t2 に揃える

            tmin = (std::max)(tmin, t1);    // tminを更新（より後ろの交点）
            tmax = (std::min)(tmax, t2);    // tmaxを更新（より手前の交点）

            if (tmin > tmax) return false; // 矛盾が出たら交差しない
        }
    }

    // レイがヒットした場合、ヒット座標を計算
    hitPoint = VAdd(origin, VScale(direction, tmin));

    // ヒット時の法線（とりあえず上向き固定。本当は衝突面の向きを取るべき）
    hitNormal = VGet(0, 1, 0);

    return true; // ヒット成功
}

// 車体の衝突チェックを行う関数
bool Car::CheckBodyCollision()
{
    // 衝突判定フラグを初期化（最初はすべてfalse）
    hitFront = false;
    hitBack = false;
    hitLeft = false;
    hitRight = false;

    //車体ボックスの頂点を計算する処理

    // 車体の向きから前方向ベクトルを生成（XZ平面上）
    VECTOR forward = VNorm(VGet(sinf(carBodyRotation.y), 0, cosf(carBodyRotation.y)));
    // 前方向ベクトルから右方向ベクトルを作成（直交ベクトル）
    VECTOR right = VNorm(VGet(forward.z, 0, -forward.x));

    // 車体中心から前後中心点を計算
    VECTOR frontCenter = VAdd(carBodyPosition, VScale(forward, boxHalfLength)); // 前方中心
    VECTOR backCenter = VAdd(carBodyPosition, VScale(forward, -boxHalfLength)); // 後方中心

    // 前後中心から左右方向にオフセットして4隅の点を取得
    VECTOR frontLeft = VAdd(frontCenter, VScale(right, -boxHalfWidth));  // 左前
    VECTOR frontRight = VAdd(frontCenter, VScale(right, boxHalfWidth));  // 右前
    VECTOR backLeft = VAdd(backCenter, VScale(right, -boxHalfWidth));    // 左後
    VECTOR backRight = VAdd(backCenter, VScale(right, boxHalfWidth));    // 右後

    // 上面の高さ（車体中心Y＋高さ）
    float y = carBodyPosition.y + boxHeight;

    // 上面の頂点設定
    boxVertices[0] = VGet(frontLeft.x, y, frontLeft.z);   // 左前上
    boxVertices[1] = VGet(frontRight.x, y, frontRight.z); // 右前上
    boxVertices[2] = VGet(backLeft.x, y, backLeft.z);     // 左後上
    boxVertices[3] = VGet(backRight.x, y, backRight.z);   // 右後上

    // 下面の頂点設定
    boxVertices[4] = VGet(frontLeft.x, carBodyPosition.y, frontLeft.z);  // 左前下
    boxVertices[5] = VGet(frontRight.x, carBodyPosition.y, frontRight.z); // 右前下
    boxVertices[6] = VGet(backLeft.x, carBodyPosition.y, backLeft.z);    // 左後下
    boxVertices[7] = VGet(backRight.x, carBodyPosition.y, backRight.z);  // 右後下

    //衝突判定用のライン（前後左右）を作成

    VECTOR frontStart = boxVertices[0]; // 前面左上
    VECTOR frontEnd = boxVertices[1];   // 前面右上

    VECTOR backStart = boxVertices[2];  // 後面左上
    VECTOR backEnd = boxVertices[3];    // 後面右上

    VECTOR leftStart = boxVertices[0];  // 左面前上
    VECTOR leftEnd = boxVertices[2];    // 左面後上

    VECTOR rightStart = boxVertices[1]; // 右面前上
    VECTOR rightEnd = boxVertices[3];   // 右面後上

    // 車体周囲の衝突判定実施

    MV1_COLL_RESULT_POLY hitResult;

    // 前方チェック
    hitResult = MV1CollCheck_Line(map.modelHandle, -1, frontStart, frontEnd);
    if (hitResult.HitFlag == TRUE) hitFront = true;

    // 後方チェック
    hitResult = MV1CollCheck_Line(map.modelHandle, -1, backStart, backEnd);
    if (hitResult.HitFlag == TRUE) hitBack = true;

    // 左側チェック
    hitResult = MV1CollCheck_Line(map.modelHandle, -1, leftStart, leftEnd);
    if (hitResult.HitFlag == TRUE) hitLeft = true;

    // 右側チェック
    hitResult = MV1CollCheck_Line(map.modelHandle, -1, rightStart, rightEnd);
    if (hitResult.HitFlag == TRUE) hitRight = true;

    //減速用ライン設定

    const float halfWidthDeceleration = 150.0f;    // 左右の減速判定範囲
    const float halfLengthDeceleration = 450.0f;   // 前後の減速判定範囲

    VECTOR center = carBodyPosition;               // 車体中心
    MATRIX rotY = MGetRotY(carBodyRotation.y);      // 車体回転行列（Y軸回転）

    // ローカル座標→ワールド座標へ変換して前後左右の減速用オフセットを求める
    VECTOR frontPos = VTransform(VGet(0.0f, 0.0f, halfLengthDeceleration), rotY);
    VECTOR backPos = VTransform(VGet(0.0f, 0.0f, -halfLengthDeceleration + 200), rotY);
    VECTOR leftPos = VTransform(VGet(-halfWidthDeceleration, 0.0f, 0.0f), rotY);
    VECTOR rightPos = VTransform(VGet(halfWidthDeceleration, 0.0f, 0.0f), rotY);

    // 減速ラインの開始点（地面から少し浮かせる）
    const float lineHeight = 20.0f;

    frontStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    frontEndDeceleration = VAdd(frontStartDeceleration, frontPos);

    backStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    backEndDeceleration = VAdd(backStartDeceleration, backPos);

    leftStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    leftEndDeceleration = VAdd(leftStartDeceleration, leftPos);

    rightStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    rightEndDeceleration = VAdd(rightStartDeceleration, rightPos);

    // 減速用衝突フラグ初期化
    hitFrontDeceleration = false;
    hitBackDeceleration = false;
    hitLeftDeceleration = false;
    hitRightDeceleration = false;

    //減速ラインでの衝突判定

    MV1_COLL_RESULT_POLY result;

    // 前方減速判定
    result = MV1CollCheck_Line(map.modelHandle, -1, frontStartDeceleration, frontEndDeceleration);
    if (result.HitFlag == 1) hitFrontDeceleration = true;

    // 後方減速判定
    result = MV1CollCheck_Line(map.modelHandle, -1, backStartDeceleration, backEndDeceleration);
    if (result.HitFlag == 1) hitBackDeceleration = true;

    // 左方減速判定
    result = MV1CollCheck_Line(map.modelHandle, -1, leftStartDeceleration, leftEndDeceleration);
    if (result.HitFlag == 1) hitLeftDeceleration = true;

    // 右方減速判定
    result = MV1CollCheck_Line(map.modelHandle, -1, rightStartDeceleration, rightEndDeceleration);
    if (result.HitFlag == 1) hitRightDeceleration = true;

    // 衝突 or 減速ラインがどれかヒットしていればtrue
    return (hitFront || hitBack || hitLeft || hitRight ||
        hitFrontDeceleration || hitBackDeceleration || hitLeftDeceleration || hitRightDeceleration);
}