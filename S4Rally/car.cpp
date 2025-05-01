// Car.cpp
#include "car.h"

// --- 外部参照
extern Map map;
RigidBody rigidbody;
Engine engine;
Transmission transmission;

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

    carBodyPosition = VGet(0.0f, 2.0f, 0.0f);  // 少し高い位置にスタート
    carBodyRotation = VGet(0.0f, 0.0f, 0.0f);

    carBodyPosition = rigidbody.position;
    rigidbody.velocity = VGet(0, 0, 0);   // rigidbodyの速度も初期化

    // --- タイヤのオフセット設定 ---
    wheelOffsets[FrontLeft] = VGet(-75.0f, -1.0f, 145.0f); // 左前
    wheelOffsets[FrontRight] = VGet(75.0f, -1.0f, 145.0f); // 右前
    wheelOffsets[RearLeft] = VGet(-75.0f, -1.0f, -101.0f); // 左後ろ
    wheelOffsets[RearRight] = VGet(75.0f, -1.0f, -101.0f); // 右後ろ
}

void Car::Update(float deltaTime)
{
    WheelCollider wheels[WheelNum];  //WheelCollider クラスの配列

    // 入力処理
    accelInput = 0.0f;
    steerInput = 0.0f;
    sideBrakeInput = 0.0f;

    //キー入力取得
    if (CheckHitKey(KEY_INPUT_W)) { accelInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_S)) { accelInput -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_D)) { steerInput += 1.0f; }
    if (CheckHitKey(KEY_INPUT_A)) { steerInput -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_SPACE)) { sideBrakeInput = 1.0f; } // サイドブレーキ入力

    float velocity = VSize(rigidbody.velocity);

    // エンジンとトランスミッション更新
    transmission.Update(velocity, engine.GetRPM());
    engine.Update(velocity, transmission);

    // エンジントルク（仮の単純なトルク計算）
    float baseEngineForce = 5000.0f * accelInput;
    float driveForce = transmission.CarculateEngineForce(baseEngineForce);

    // エンジンブレーキ効果（アクセルオフ時）
    if (accelInput == 0.0f) {
        driveForce -= engine.GetBrakeCoefficient() * velocity;
    }

    // 車体への推進力適用
    VECTOR forward = VGet(sinf(carBodyRotation.y), 0, cosf(carBodyRotation.y));
    rigidbody.AddForce(VScale(forward, driveForce * deltaTime));

    //地面の摩擦による減速処理
    rigidbody.velocity = VScale(rigidbody.velocity, 0.98f); // フリクション

    //ステアリング処理（速度に応じて旋回する）
    steeringAngle = 40.0f * steerInput * DX_PI_F / 180.0f; // ハンドル角（ラジアン）
    const float steeringSpeedThreshold = 2.0f; // 旋回可能な最低速度[m/s]

    if (velocity > steeringSpeedThreshold)carBodyRotation.y += steeringAngle * deltaTime;

    //位置更新
    rigidbody.Update(deltaTime);
    carBodyPosition = rigidbody.position;

    // 車体の移動前に当たり判定チェック
    if (CheckBodyCollision()) {
        printfDx("地面に車体が当たっています！\n");
        // 衝突応答処理（位置を戻すなど）
    }

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

        VECTOR wheelPos = VAdd(carBodyPosition, wheelOffsets[i]);
        VECTOR hitPoint, hitNormal;
        float compression;
        if (wheels[i].GetGroundHit(wheelPos, hitPoint, hitNormal, compression)) {
            DrawSphere3D(hitPoint, 1000.0f, 10, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
        }
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
    DrawFormatString(10, 200, GetColor(255, 255, 255), "Mode: %s", transmission.mode == Manual ? "Manual" : "Auto");
    DrawFormatString(10, 220, GetColor(255, 255, 255), "Gear: %d", transmission.GetGear());
    DrawFormatString(10, 240, GetColor(255, 255, 255), "Engine RPM: %.0f", engine.GetRPM());

    char carPosLog[256];
    sprintf_s(carPosLog, "Car Pos: X=%.2f Y=%.2f Z=%.2f", carBodyPosition.x, carBodyPosition.y, carBodyPosition.z);
    DrawString(10, 10, carPosLog, GetColor(255, 255, 255));

    char carRotLog[256];
    sprintf_s(carRotLog, "Car Rot: X=%.2f Y=%.2f Z=%.2f", carBodyRotation.x, carBodyRotation.y, carBodyRotation.z);
    DrawString(10, 30, carRotLog, GetColor(255, 255, 255));

    char rigidBodyVelocity[256];
    sprintf_s(rigidBodyVelocity, "rigidBody velocity: X=%.2f Y=%.2f Z=%.2f", rigidbody.velocity.x, rigidbody.velocity.y, rigidbody.velocity.z);
    DrawString(10, 50, rigidBodyVelocity, GetColor(255, 255, 255));

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
    const float width = 75.0f;
    const float length = 145.0f;
    const float height = 30.0f;

    // 車体の中心位置からAABBの8頂点を計算
    VECTOR center = carBodyPosition;
    VECTOR boxCorners[8];

    boxCorners[0] = VGet(center.x - width, center.y, center.z - length); // 左前下
    boxCorners[1] = VGet(center.x + width, center.y, center.z - length); // 右前下
    boxCorners[2] = VGet(center.x - width, center.y, center.z + length); // 左後下
    boxCorners[3] = VGet(center.x + width, center.y, center.z + length); // 右後下

    boxCorners[4] = VGet(center.x - width, center.y + height, center.z - length); // 左前上
    boxCorners[5] = VGet(center.x + width, center.y + height, center.z - length); // 右前上
    boxCorners[6] = VGet(center.x - width, center.y + height, center.z + length); // 左後上
    boxCorners[7] = VGet(center.x + width, center.y + height, center.z + length); // 右後上

    // 線分で当たり判定（12本）
    const int lines[12][2] = {
        {0,1},{1,3},{3,2},{2,0}, // 下の四辺
        {4,5},{5,7},{7,6},{6,4}, // 上の四辺
        {0,4},{1,5},{2,6},{3,7}  // 縦の辺
    };

    for (int i = 0; i < 12; ++i) {
        MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(map.modelHandle, -1, boxCorners[lines[i][0]], boxCorners[lines[i][1]]);
        if (result.HitFlag) {
            return true;
        }
    }

    return false;
}