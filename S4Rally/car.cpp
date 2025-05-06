// Car.cpp
#include "car.h"

// --- 外部参照
extern Map map;
RigidBody rigidbody;
SuspensionSpring suspensionSpring;

Car::Car()
    : motorForce(0.0f), steeringAngle(0.0f), carModelHandle(-1)
{
    for (int i = 0; i < WheelNum; i++)
    {
        wheelModelHandles[i] = -1;
        wheelPositions[i] = VGet(0, 0, 0);
        wheelRotations[i] = VGet(0, 0, 0);
        wheelOffsets[i] = VGet(0, 0, 0);
    }
}

Car::~Car()
{
    if (carModelHandle != -1)
    {
        MV1DeleteModel(carModelHandle); // <- モデルなのでDeleteGraphじゃなくてMV1DeleteModel！
    }

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

    VECTOR startPos = VGet(3000.0f, -30.0f, -3000.0f); // 例えば (X=0, Y=2, Z=0) の位置にスタート

    rigidbody.position = startPos;  // まずRigidbodyにセット
    rigidbody.velocity = VGet(0.0f, 0.0f, 0.0f); // 速度も初期化
    rigidbody.isGravity = true;     // 重力オン
    rigidbody.isGrounded = false;   // 最初は地面に接してない

    carBodyPosition = startPos;     // 車体の位置もセット
    carBodyRotation = VGet(0.0f, 1.5f, 0.0f); // 向きもリセット（正面向き）

    // --- タイヤのオフセット設定 ---
    wheelOffsets[FrontLeft] = VGet(-75.0f, -1.0f, 145.0f); // 左前
    wheelOffsets[FrontRight] = VGet(75.0f, -1.0f, 145.0f); // 右前
    wheelOffsets[RearLeft] = VGet(-75.0f, -1.0f, -101.0f); // 左後ろ
    wheelOffsets[RearRight] = VGet(75.0f, -1.0f, -101.0f); // 右後ろ
}

void Car::Update(float deltaTime)
{
    WheelCollider wheels[WheelNum];  //WheelCollider クラスの配列

    ControllerUpdate();//コントローラーの更新処理

    // アナログスティックの値を取得して表示
    float axisX = GetAxisX();

    // 入力処理
    accelInput = 0.0f;
    steerInput = 0.0f;
    sideBrakeInput = 0.0f;

    //キー入力取得
    if (CheckHitKey(KEY_INPUT_W)|| GetButton(BUTTON_RT)) { accelInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_S)|| GetButton(BUTTON_LT)) { accelInput -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_D)|| axisX >= 0.5f) { steerInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_A)|| axisX <= -0.5f) { steerInput -= 1.0f; }
    //if (CheckHitKey(KEY_INPUT_SPACE)) { sideBrakeInput = 1.0f; } // サイドブレーキ入力

    // 車体の当たり判定
    if (CheckBodyCollision()) {
        // ここで、フラグに応じて車の動きを制御できる
        if (hitFront && accelInput > 0.0f) {
            //accelInput = 0.0f; // 前進を止める
            // 正面か背面にぶつかったら完全停止
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitBack && accelInput < 0.0f) {
            accelInput = 0.0f; // 後退を止める
            // 正面か背面にぶつかったら完全停止
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitLeft && steerInput < 0.0f) {
            //steerInput = 0.0f; // 左折を止める
            // 左右にぶつかったら速度を大幅に減速（例えば30%に）
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }
        if (hitRight && steerInput > 0.0f) {
            //steerInput = 0.0f; // 右折を止める
            // 左右にぶつかったら速度を大幅に減速（例えば30%に）
            rigidbody.velocity = VScale(rigidbody.velocity, 0.3f);
        }

        // ここで、フラグに応じて車の動きを制御できる
        if (hitFrontDeceleration && accelInput > 0.0f) {
            // 正面か背面にぶつかったら完全停止
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitBackDeceleration && accelInput < 0.0f) {
            // 正面か背面にぶつかったら完全停止
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitLeftDeceleration && steerInput < 0.0f) {
            // 左右にぶつかったら速度を大幅に減速（例えば30%に）
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
        if (hitRightDeceleration && steerInput > 0.0f) {
            // 左右にぶつかったら速度を大幅に減速（例えば30%に）
            rigidbody.velocity = VScale(rigidbody.velocity, 0.1f);
        }
    }

    float velocity = VSize(rigidbody.velocity);

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
    const float steeringSpeedThreshold = 2.0f; // 旋回可能な最低速度[m/s]

    if (velocity > steeringSpeedThreshold)carBodyRotation.y += steeringAngle * deltaTime;

    //位置更新
    rigidbody.Update(deltaTime);
    carBodyPosition = rigidbody.position;

    //タイヤの位置・回転更新
    for (int i = 0; i < WheelNum; i++)
    {
        VECTOR rotatedOffset = VTransform(wheelOffsets[i], MGetRotY(carBodyRotation.y));
        wheelPositions[i] = VAdd(carBodyPosition, rotatedOffset);

        wheelRotations[i] = VGet(0, carBodyRotation.y, 0);

        // 前輪だけステアリング
        if (i == FrontLeft || i == FrontRight)
        {
            wheelRotations[i].y += steeringAngle;
        }

        VECTOR wheelPos = VAdd(carBodyPosition, wheelPositions[i]);
        VECTOR hitPoint, hitNormal;
        float compression;
        if (wheels[i].GetGroundHit(wheelPos, hitPoint, hitNormal, compression)) {
            rigidbody.isGravity = false;
            rigidbody.isGrounded = true;
        }
        //else
        //{
        //    rigidbody.isGravity = true;
        //    rigidbody.isGrounded = false;
        //}

    }

    DrawFormatString(10, 260, GetColor(255, 255, 255), "Speed: %.1f m/s", velocity);
}

void Car::Draw()
{
    if (carModelHandle != -1)
    {
        MV1SetPosition(carModelHandle, carBodyPosition);
        MV1SetRotationXYZ(carModelHandle, carBodyRotation);
        MV1DrawModel(carModelHandle);
    }

    for (int i = 0; i < WheelNum; i++)
    {
        if (wheelModelHandles[i] != -1)
        {
            MV1SetPosition(wheelModelHandles[i], wheelPositions[i]);
            MV1SetRotationXYZ(wheelModelHandles[i], wheelRotations[i]);
            MV1DrawModel(wheelModelHandles[i]);
        }
    }

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

bool Car::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal) {
    // 仮実装：AABB（バウンディングボックス）を手動で作る
    VECTOR min = VSub(carBodyPosition, VGet(1.5f, 0.5f, 3.0f)); // サイズ仮
    VECTOR max = VAdd(carBodyPosition, VGet(1.5f, 0.5f, 3.0f));

    // レイとAABBの交差判定
    float tmin = 0.0f, tmax = maxDistance;
    for (int i = 0; i < 3; ++i) {
        float originComp = (&origin.x)[i];
        float dirComp = (&direction.x)[i];
        float minComp = (&min.x)[i];
        float maxComp = (&max.x)[i];

        if (fabs(dirComp) < 1e-6f) {
            if (originComp < minComp || originComp > maxComp) return false;
        }
        else {
            float ood = 1.0f / dirComp;
            float t1 = (minComp - originComp) * ood;
            float t2 = (maxComp - originComp) * ood;
            if (t1 > t2) std::swap(t1, t2);
            tmin = (std::max)(tmin, t1);
            tmax = (std::min)(tmax, t2);
            if (tmin > tmax) return false;
        }
    }

    hitPoint = VAdd(origin, VScale(direction, tmin));
    hitNormal = VGet(0, 1, 0); // 適当な法線（本来は接触面向きにすべき）
    return true;
}

bool Car::CheckBodyCollision()
{
    hitFront = false;
    hitBack = false;
    hitLeft = false;
    hitRight = false;

    // 車体ボックスの頂点を更新
    VECTOR forward = VNorm(VGet(sinf(carBodyRotation.y), 0, cosf(carBodyRotation.y)));
    VECTOR right = VNorm(VGet(forward.z, 0, -forward.x)); // 右ベクトル

    VECTOR frontCenter = VAdd(carBodyPosition, VScale(forward, boxHalfLength));
    VECTOR backCenter = VAdd(carBodyPosition, VScale(forward, -boxHalfLength));

    VECTOR frontLeft = VAdd(frontCenter, VScale(right, -boxHalfWidth));
    VECTOR frontRight = VAdd(frontCenter, VScale(right, boxHalfWidth));
    VECTOR backLeft = VAdd(backCenter, VScale(right, -boxHalfWidth));
    VECTOR backRight = VAdd(backCenter, VScale(right, boxHalfWidth));

    float y = carBodyPosition.y + boxHeight; // 高さを上げる（地面から浮かせる）

    // ボックスの頂点をセット
    boxVertices[0] = VGet(frontLeft.x, y, frontLeft.z);  // 左前上
    boxVertices[1] = VGet(frontRight.x, y, frontRight.z); // 右前上
    boxVertices[2] = VGet(backLeft.x, y, backLeft.z);    // 左後上
    boxVertices[3] = VGet(backRight.x, y, backRight.z);   // 右後上

    boxVertices[4] = VGet(frontLeft.x, carBodyPosition.y, frontLeft.z); // 左前下
    boxVertices[5] = VGet(frontRight.x, carBodyPosition.y, frontRight.z); // 右前下
    boxVertices[6] = VGet(backLeft.x, carBodyPosition.y, backLeft.z);   // 左後下
    boxVertices[7] = VGet(backRight.x, carBodyPosition.y, backRight.z);  // 右後下

    // --- 前後左右の線分を作成
    VECTOR frontStart = boxVertices[0];
    VECTOR frontEnd = boxVertices[1];

    VECTOR backStart = boxVertices[2];
    VECTOR backEnd = boxVertices[3];

    VECTOR leftStart = boxVertices[0];
    VECTOR leftEnd = boxVertices[2];

    VECTOR rightStart = boxVertices[1];
    VECTOR rightEnd = boxVertices[3];

    // --- 衝突判定（Mapに線分判定）
    MV1_COLL_RESULT_POLY hitResult;

    hitResult = MV1CollCheck_Line(map.modelHandle, -1, frontStart, frontEnd);
    if (hitResult.HitFlag == TRUE) hitFront = true;

    hitResult = MV1CollCheck_Line(map.modelHandle, -1, backStart, backEnd);
    if (hitResult.HitFlag == TRUE) hitBack = true;

    hitResult = MV1CollCheck_Line(map.modelHandle, -1, leftStart, leftEnd);
    if (hitResult.HitFlag == TRUE) hitLeft = true;

    hitResult = MV1CollCheck_Line(map.modelHandle, -1, rightStart, rightEnd);
    if (hitResult.HitFlag == TRUE) hitRight = true;



    const float halfWidthDeceleration = 150.0f;
    const float halfLengthDeceleration =450.0f;

    VECTOR center = carBodyPosition;
    MATRIX rotY = MGetRotY(carBodyRotation.y);

    // 進行方向基準のオフセット計算
    VECTOR frontPos = VTransform(VGet(0.0f, 0.0f, halfLengthDeceleration), rotY);
    VECTOR backPos = VTransform(VGet(0.0f, 0.0f, -halfLengthDeceleration+200), rotY);
    VECTOR leftPos = VTransform(VGet(-halfWidthDeceleration, 0.0f, 0.0f), rotY);
    VECTOR rightPos = VTransform(VGet(halfWidthDeceleration, 0.0f, 0.0f), rotY);

    // 壁検出用ラインを設定（地面から少し浮かせた高さにする）
    const float lineHeight = 20.0f;
    frontStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    frontEndDeceleration = VAdd(frontStartDeceleration, frontPos);

    backStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    backEndDeceleration = VAdd(backStartDeceleration, backPos);

    leftStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    leftEndDeceleration = VAdd(leftStartDeceleration, leftPos);

    rightStartDeceleration = VAdd(center, VGet(0, lineHeight, 0));
    rightEndDeceleration = VAdd(rightStartDeceleration, rightPos);

    hitFrontDeceleration = false;
    hitBackDeceleration = false;
    hitLeftDeceleration = false;
    hitRightDeceleration = false;

    // 各方向の衝突判定
    MV1_COLL_RESULT_POLY result;

    result = MV1CollCheck_Line(map.modelHandle, -1, frontStartDeceleration, frontEndDeceleration);
    if (result.HitFlag == 1) hitFrontDeceleration = true;

    result = MV1CollCheck_Line(map.modelHandle, -1, backStartDeceleration, backEndDeceleration);
    if (result.HitFlag == 1) hitBackDeceleration = true;

    result = MV1CollCheck_Line(map.modelHandle, -1, leftStartDeceleration, leftEndDeceleration);
    if (result.HitFlag == 1) hitLeftDeceleration = true;

    result = MV1CollCheck_Line(map.modelHandle, -1, rightStartDeceleration, rightEndDeceleration);
    if (result.HitFlag == 1) hitRightDeceleration = true;

    return (hitFront || hitBack || hitLeft || hitRight||hitFrontDeceleration || hitBackDeceleration || hitLeftDeceleration || hitRightDeceleration);
}