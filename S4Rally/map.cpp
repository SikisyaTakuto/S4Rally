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
    // ���C�g�̐ݒ�
    VECTOR lightDirection = VGet(0.0f, 100.0f, 100.0f);
    SetLightDirection(lightDirection);

    // ���f���ǂݍ���
    modelHandle = MV1LoadModel("fbx/Stage/TestTrack.fbx"); // ���p�X�͂��Ȃ��̃��f���f�[�^�ɍ��킹�ĂˁI
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

bool Map::Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal) {
    // �������F�n��Y=0 �����Map�̕\�ʂŃq�b�g�������Ƃɂ���
    // �{����Mesh���C�L���X�g���邯�ǁA�ȒP��

  // �⓹�FZ�������ɍ������オ��Y=Z*0.5�̍�
    if (direction.y < 0.0f) {
        float expectedY = origin.z * 0.5f;
        float distanceToGround = (origin.y - expectedY) / (-direction.y);
        if (distanceToGround >= 0.0f && distanceToGround <= maxDistance) {
            hitPoint = VAdd(origin, VScale(direction, distanceToGround));
            hitPoint.y = hitPoint.z * 0.5f;
            hitNormal = VNorm(VGet(0.0f, 1.0f, -0.5f)); // ��̖@��
            return true;
        }
    }

    // �ǁiZ=30�̈ʒu�ɂ���j
    if (fabs(direction.z) > 0.001f) {
        float t = (30.0f - origin.z) / direction.z;
        if (t >= 0.0f && t <= maxDistance) {
            hitPoint = VAdd(origin, VScale(direction, t));
            hitNormal = VGet(0.0f, 0.0f, -1.0f); // �ǂ̖@��
            return true;
        }
    }

    return false;
}