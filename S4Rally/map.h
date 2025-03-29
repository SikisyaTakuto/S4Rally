//�}�b�v�����̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "DxLib.h"
#include "car.h"

#define LINE_AREA_SIZE 1000000.0f// ���C����`���͈�
#define LINE_NUM 1000            // ���C���̐�

#define MAP_SIZE	64			// �}�b�v�`�b�v��̃h�b�g�T�C�Y
#define MAP_WIDTH	50			// �}�b�v�̕�
#define MAP_HEIGHT	50			// �}�b�v�̏c����

// �}�N�����`���ĕ\���̊J�n�ʒu�𒲐�
#define BASE_Y_POS 90
#define Y_OFFSET 40

//�\����
typedef struct _Stage {

	int mapHandle;
	VECTOR SpherePos;							//���̂̈ʒu�����߂�
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;		//���̂ƃ��f���̓����蔻��

	VECTOR frontLeftPos;			//�O�ցi�E�j�̈ʒu�����߂�
	VECTOR frontRightPos;			//�O�ցi���j�̈ʒu�����߂�
	VECTOR rearLeftPos;				//��ցi�E�j�̈ʒu�����߂�
	VECTOR rearRightPos;			//��ցi���j�̈ʒu�����߂�
	VECTOR carBodyPos1, carBodyPos2;//�ԑ̂̈ʒu�����߂�

    MV1_COLL_RESULT_POLY_DIM frontLeftHitPolyDim;  //�O�ցi�E�j�̓����蔻��
	MV1_COLL_RESULT_POLY_DIM frontRightHitPolyDim; //�O�ցi���j�̓����蔻��
    MV1_COLL_RESULT_POLY_DIM rearLeftHitPolyDim;   //��ցi�E�j�̓����蔻��
    MV1_COLL_RESULT_POLY_DIM rearRightHitPolyDim;  //��ցi���j�̓����蔻��
	MV1_COLL_RESULT_POLY_DIM carBodyHitPolyDim;	   //�ԑ̂̓����蔻��

	VECTOR frontLeftHitPos;		//�O�ցi�E�j�̓����������W���擾
	VECTOR frontRightHitPos;	//�O�ցi���j�̓����������W���擾
	VECTOR rearLeftHitPos;		//��ցi�E�j�̓����������W���擾
	VECTOR rearRightHitPos;		//��ցi���j�̓����������W���擾
	VECTOR carBodyHitPos;		//�ԑ̂̓����������W���擾

	VECTOR frontLeftNormal;		//�O�ցi�E�j�̖@���x�N�g�����擾
	VECTOR frontRightNormal;	//�O�ցi���j�̖@���x�N�g�����擾
	VECTOR rearLeftNormal;		//��ցi�E�j�̖@���x�N�g�����擾
	VECTOR rearRightNormal;		//��ցi���j�̖@���x�N�g�����擾
	VECTOR carBodyNormal;		//�ԑ̖̂@���x�N�g�����擾

	bool frontLeftWallHit;   //�O�ցi�E�j�̕ǂ̓����蔻��
	bool frontRightWallHit;  //�O�ցi���j�̕ǂ̓����蔻��
	bool rearLeftWallHit;    //��ցi�E�j�̕ǂ̓����蔻��
	bool rearRightWallHit;   //��ցi���j�̕ǂ̓����蔻��
	bool carBodyWallHit;	 //�ԑ̂̕ǂ̓����蔻��

	bool lockLeft;
	bool lockRight;

}Stage;

extern Stage stage;

extern VOID MapInit(VOID);      //������
extern VOID GraphDraw(VOID);	// �}�b�v�ƃv���C���[�̕`��֐�
extern VOID HitDetection(VOID); //�����蔻��