#include "map.h"

// �R���X�g���N�^
Map::Map()
    : modelHandle(-1) // ������
{
}

// �f�X�g���N�^
Map::~Map()
{
    if (modelHandle != -1)
    {
        DeleteGraph(modelHandle);
    }
}

// �}�b�v������
void Map::Init()
{
    // Map.cpp �� Init() �ɂ�
    VECTOR lightDirection = VGet(1000.0f, 10000.0f, 0.0f);
    SetLightDirection(lightDirection);
    SetLightEnable(TRUE); // ���C�g��L�����i�O�̂��߁j
    SetLightAmbColor(GetColorF(0.3f, 0.3f, 0.3f, 1.0f)); // �����̐F��������Ǝ��

    // ���f���ǂݍ���
    modelHandle = MV1LoadModel("fbx/Stage/ContainerTerminal.fbx"); // ���p�X�͂��Ȃ��̃��f���f�[�^�ɍ��킹�ĂˁI

    MV1SetPosition(modelHandle, VGet(0.0f,50.0f, 0.0f));
}

// �}�b�v�X�V�i����͉����Ȃ��j
void Map::Update()
{
    // �K�v�Ȃ炱���ɍX�V����������
}

// �}�b�v�`��
void Map::Draw()
{
    // ���f���`��
    if (modelHandle != -1)
    {
        MV1DrawModel(modelHandle);
    }

    // �f�o�b�O�p�̃��C���`��
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