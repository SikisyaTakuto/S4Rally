#include "map.h"

// コンストラクタ
Map::Map()
    : modelHandle(-1) // 初期化
{
}

// デストラクタ
Map::~Map()
{
    if (modelHandle != -1)
    {
        DeleteGraph(modelHandle);
    }
}

// マップ初期化
void Map::Init()
{
    // ライトの設定
    VECTOR lightDirection = VGet(0.0f, 100.0f, 100.0f);
    SetLightDirection(lightDirection);

    // モデル読み込み
    modelHandle = MV1LoadModel("fbx/Stage/TestTrack.fbx"); // ←パスはあなたのモデルデータに合わせてね！
}

// マップ更新（今回は何もなし）
void Map::Update()
{
    // 必要ならここに更新処理を書く
}

// マップ描画
void Map::Draw()
{
    // モデル描画
    if (modelHandle != -1)
    {
        MV1DrawModel(modelHandle);
    }

    // デバッグ用のライン描画
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

bool Map::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal) {
    // 仮実装：地面Y=0 代わりにMapの表面でヒットしたことにする
    // 本来はMeshレイキャストするけど、簡単化

  // 坂道：Z軸方向に高さが上がるY=Z*0.5の坂
    if (direction.y < 0.0f) {
        float expectedY = origin.z * 0.5f;
        float distanceToGround = (origin.y - expectedY) / (-direction.y);
        if (distanceToGround >= 0.0f && distanceToGround <= maxDistance) {
            hitPoint = VAdd(origin, VScale(direction, distanceToGround));
            hitPoint.y = hitPoint.z * 0.5f;
            hitNormal = VNorm(VGet(0.0f, 1.0f, -0.5f)); // 坂の法線
            return true;
        }
    }

    // 壁（Z=30の位置にある）
    if (fabs(direction.z) > 0.001f) {
        float t = (30.0f - origin.z) / direction.z;
        if (t >= 0.0f && t <= maxDistance) {
            hitPoint = VAdd(origin, VScale(direction, t));
            hitNormal = VGet(0.0f, 0.0f, -1.0f); // 壁の法線
            return true;
        }
    }

    return false;
}