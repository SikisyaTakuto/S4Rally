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
    // ライティングの計算をしないように設定を変更
    SetUseLighting(FALSE);

    // モデル読み込み
    modelHandle = MV1LoadModel("fbx/Stage/ContainerTerminal.fbx");
    MV1SetPosition(modelHandle, VGet(0.0f, 50.0f, 0.0f));
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