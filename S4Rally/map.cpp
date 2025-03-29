#include "map.h"
#include <cfloat>

int Add, y, x, i;
Stage stage;

VOID MapInit(VOID)
{
    // ���C�g�̏�����
    VECTOR lightDirection = VGet(0.0f, 10.0f, 10.0f); // ���C�g�̕���
    SetLightDirection(lightDirection);


    stage.mapHandle = MV1LoadModel("fbx/Stage/SakuraStage4.fbx");
    stage.lockLeft=false;
    stage.lockRight = false;
    MV1SetPosition(stage.mapHandle, VGet(1000.0f,30.0f, 0.0f));

    // ���f���S�̂̃R���W���������\�z
    MV1SetupCollInfo(stage.mapHandle, -1, 16, 16, 16);

    // ���f���̐i�s�������Z�b�g
    Add = 8;

    // ���f���̈ړ��ʒu���Z�b�g
    y = 0;
    x = 250.0f;

	return VOID();
}
// �}�b�v�̕`��֐�
VOID GraphDraw(VOID)
{
    //3D���f���̓����蔻��
    {
        MV1DrawModel(stage.mapHandle);

        //����(�^�C��)�̈ʒu
        stage.frontLeftPos = VGet(car.frontLeft.center.x, car.frontLeft.center.y+35.0f, car.frontLeft.center.z);
        stage.frontRightPos = VGet(car.frontRight.center.x, car.frontRight.center.y + 35.0f, car.frontRight.center.z);
        stage.rearLeftPos = VGet(car.rearLeft.center.x, car.rearLeft.center.y + 35.0f, car.rearLeft.center.z);
        stage.rearRightPos = VGet(car.rearRight.center.x, car.rearRight.center.y + 35.0f, car.rearRight.center.z);

        //���f���Ƌ�(�^�C��)�Ƃ̓����蔻��
        stage.frontLeftHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.frontLeftPos, 35.0f);
        stage.frontRightHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.frontRightPos, 35.0f);
        stage.rearLeftHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.rearLeftPos, 35.0f);
        stage.rearRightHitPolyDim = MV1CollCheck_Sphere(stage.mapHandle, -1, stage.rearRightPos, 35.0f);

        // ���̕`��
        DrawSphere3D(stage.frontLeftPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.frontRightPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.rearLeftPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
        DrawSphere3D(stage.rearRightPos, 35.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);

        //�����蔻��
        HitDetection();

        // �����蔻����̌�n��
        MV1CollResultPolyDimTerminate(stage.frontLeftHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.frontRightHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.rearLeftHitPolyDim);
        MV1CollResultPolyDimTerminate(stage.rearRightHitPolyDim);
    }

    //�f�o�b�N���ʒu��������悤�Ƀ��C��������
    {
        // �ʒu�֌W��������悤�ɒn�ʂɃ��C����`�悷��
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

//���f���Ƌ��̂̓����蔻��
VOID HitDetection(VOID) 
{
    int yOffset = BASE_Y_POS; // �`��ʒu�̃I�t�Z�b�g

    // �Ԃ̌������擾
    MATRIX carMatrix = MGetRotY(carInfo.rotation.y);

    // Z���̕����i�O�����j
    VECTOR carForward = VTransform(VGet(0.0f, 0.0f, 1.0f), carMatrix);

    // X���̕����i�E�����j
    VECTOR carRight = VTransform(VGet(1.0f, 0.0f, 0.0f), carMatrix);

    // �����������ǂ����ŏ����𕪊�
    if (stage.frontLeftHitPolyDim.HitNum >= 1)
    {
        // ���������|���S���̐���`��
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "frontLeft Hit Poly Num   %d", stage.frontLeftHitPolyDim);

        yOffset += Y_OFFSET;

        // ���������|���S���̐������J��Ԃ�
        for (i = 0; i < stage.frontLeftHitPolyDim.HitNum; i++)
        {
            stage.frontLeftHitPos = stage.frontLeftHitPolyDim.Dim[i].HitPosition;  // **�Փˍ��W���擾**
            stage.frontLeftNormal= stage.frontLeftHitPolyDim.Dim[i].Normal;  // **�@���x�N�g�����擾**

            // **���ƕǂ̔���**
            if (stage.frontLeftNormal.y > 0.01f)
            {
                // **���ɏՓ�**
                // **�Փˍ��W����ʂɕ\��**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "frontLeft Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                // **�Փˍ��W�ɋ���`�悵�Ď��o��**
                DrawSphere3D(stage.frontLeftHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **�ǂɏՓ�**
                // **�Փˍ��W����ʂɕ\��**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "frontLeft Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                // **�Փˍ��W�ɋ���`�悵�Ď��o��**
                DrawSphere3D(stage.frontLeftHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.frontLeftWallHit = true;

                // �@���ƑO�����̓��ς��v�Z
                float dot = VDot(stage.frontLeftNormal, carForward);
                float sideDot = VDot(stage.frontLeftNormal, carRight);

                if (dot > 0.7f)  // ����
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "����: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (dot < -0.7f)  // �w��
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "�w��: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (sideDot > 0.7f)  // �E����
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "�E����: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else if (sideDot < -0.7f)  // ������
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "������: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
                else  // ���̑��i�΂߁j
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "�΂�: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontLeftHitPos.x, stage.frontLeftHitPos.y, stage.frontLeftHitPos.z);
                }
            }

            yOffset += Y_OFFSET;

        }
    }

    // �����������ǂ����ŏ����𕪊�
    if (stage.frontRightHitPolyDim.HitNum >= 1)
    {
        // ���������|���S���̐���`��
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "frontRight Hit Poly Num   %d", stage.frontRightHitPolyDim);
        yOffset += Y_OFFSET;

        // ���������|���S���̐������J��Ԃ�
        for (i = 0; i < stage.frontRightHitPolyDim.HitNum; i++)
        {
            stage.frontRightHitPos = stage.frontRightHitPolyDim.Dim[i].HitPosition;
            stage.frontRightNormal = stage.frontRightHitPolyDim.Dim[i].Normal;  // **�@���x�N�g�����擾**

            // **���ƕǂ̔���**
            if (stage.frontRightNormal.y > 0.01f)
            {
                // **���ɏՓ�**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "frontRight Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                DrawSphere3D(stage.frontRightHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **�ǂɏՓ�**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "frontRight Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                DrawSphere3D(stage.frontRightHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.frontRightWallHit = true;

                // �@���ƑO�����̓��ς��v�Z
                float dot = VDot(stage.frontRightNormal, carForward);
                float sideDot = VDot(stage.frontRightNormal, carRight);

                if (dot > 0.7f)  // ����
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "����: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (dot < -0.7f)  // �w��
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "�w��: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (sideDot > 0.7f)  // �E����
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "�E����: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else if (sideDot < -0.7f)  // ������
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "������: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
                else  // ���̑��i�΂߁j
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "�΂�: X=%.2f Y=%.2f Z=%.2f",
                        stage.frontRightHitPos.x, stage.frontRightHitPos.y, stage.frontRightHitPos.z);
                }
            }

            yOffset += Y_OFFSET;

        }
    }

    // �����������ǂ����ŏ����𕪊�
    if (stage.rearLeftHitPolyDim.HitNum >= 1)
    {
        // ���������|���S���̐���`��
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "rearLeft Hit Poly Num   %d", stage.rearLeftHitPolyDim);
        yOffset += Y_OFFSET;

        // ���������|���S���̐������J��Ԃ�
        for (i = 0; i < stage.rearLeftHitPolyDim.HitNum; i++)
        {
            stage.rearLeftHitPos = stage.rearLeftHitPolyDim.Dim[i].HitPosition;
            stage.rearLeftNormal= stage.rearLeftHitPolyDim.Dim[i].Normal;  // **�@���x�N�g�����擾**

            // **���ƕǂ̔���**
            if (stage.rearLeftNormal.y > 0.01f)
            {
                // **���ɏՓ�**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "rearLeft Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                DrawSphere3D(stage.rearLeftHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **�ǂɏՓ�**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "rearLeft Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                DrawSphere3D(stage.rearLeftHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.rearLeftWallHit =true;

                // �@���ƑO�����̓��ς��v�Z
                float dot = VDot(stage.rearLeftNormal, carForward);
                float sideDot = VDot(stage.rearLeftNormal, carRight);

                if (dot > 0.7f)  // ����
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "����: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (dot < -0.7f)  // �w��
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "�w��: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (sideDot > 0.7f)  // �E����
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "�E����: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else if (sideDot < -0.7f)  // ������
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "������: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
                else  // ���̑��i�΂߁j
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "�΂�: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearLeftHitPos.x, stage.rearLeftHitPos.y, stage.rearLeftHitPos.z);
                }
            }
            yOffset += Y_OFFSET;
        }
    }

    // �����������ǂ����ŏ����𕪊�
    if (stage.rearRightHitPolyDim.HitNum >= 1)
    {
        // ���������|���S���̐���`��
        DrawFormatString(0, yOffset, GetColor(255, 255, 255), "rearRight Hit Poly Num   %d", stage.rearRightHitPolyDim);
        yOffset += Y_OFFSET;

        // ���������|���S���̐������J��Ԃ�
        for (i = 0; i < stage.rearRightHitPolyDim.HitNum; i++)
        {
            stage.rearRightHitPos = stage.rearRightHitPolyDim.Dim[i].HitPosition;
            stage.rearRightNormal = stage.rearRightHitPolyDim.Dim[i].Normal;  // **�@���x�N�g�����擾**

            // **���ƕǂ̔���**
            if (stage.rearRightNormal.y > 0.01f)
            {
                // **���ɏՓ�**
                DrawFormatString(0, yOffset, GetColor(0, 255, 0), "rearRight Hit Floor: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                DrawSphere3D(stage.rearRightHitPos, 10.0f, 8, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
            }
            else
            {
                // **�ǂɏՓ�**
                DrawFormatString(0, yOffset, GetColor(255, 0, 0), "rearRight Hit Wall: X=%.2f Y=%.2f Z=%.2f",
                    stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                DrawSphere3D(stage.rearRightHitPos, 10.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
                stage.rearRightWallHit =true;

                // �@���ƑO�����̓��ς��v�Z
                float dot = VDot(stage.rearRightNormal, carForward);
                float sideDot = VDot(stage.rearRightNormal, carRight);

                if (dot > 0.7f)  // ����
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "����: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (dot < -0.7f)  // �w��
                {
                    DrawFormatString(0, yOffset, GetColor(0, 255, 0), "�w��: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (sideDot > 0.7f)  // �E����
                {
                    DrawFormatString(0, yOffset, GetColor(255, 165, 0), "�E����: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else if (sideDot < -0.7f)  // ������
                {
                    DrawFormatString(0, yOffset, GetColor(0, 0, 255), "������: X=%.2f Y=%.2f Z=%.2f",
                        stage.rearRightHitPos.x, stage.rearRightHitPos.y, stage.rearRightHitPos.z);
                }
                else  // ���̑��i�΂߁j
                {
                    DrawFormatString(0, yOffset, GetColor(255, 255, 0), "�΂�: X=%.2f Y=%.2f Z=%.2f",
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

    // �e�^�C���̓����蔻����m�F
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

                if (forwardDot > 0.7f) { // ���ʏՓ�
                    frontCollision = true;
                }
                else if (forwardDot < -0.7f) { // �w�ʏՓ�
                    rearCollision = true;
                }
                else if (sideDot > 0.7f) { // ���Փ�
                    RightCollision = true;
                }
                else if (sideDot < -0.7f)
                {
                    LeftCollision = true;
                }
            }
        }
    }

    // �Փ˂ɂ�鐧������
    if (frontCollision) {
        carInfo.currentSpeed = 0.0f; // �O�i���~�߂�
    }
    if (rearCollision) {
        carInfo.currentSpeed =  0.0f; // ��ނ��~�߂�
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