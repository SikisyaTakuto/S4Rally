//�􉽊w�i�}�`�j�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include"geometory.h"

//�֐�

//X�n�_��Y�n�_����POINT�^���ꎞ�I�Ɏ擾
POINT GetPoint(int x, int y)
{
	POINT pt;

	pt.x = x;
	pt.y = y;

	return pt;
}

//�_�Ɠ_���������Ă��邩�H
BOOL CollPointToPoint(POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.x)
	{
		//�������Ă���
		return TRUE;
	}

	//�������ĂȂ�
	return FALSE;
}

//����E������RECT�^���ꎞ�I�Ɏ擾
RECT GetRect(int left, int top, int right, int bottom)
{
	RECT rect;

	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	return rect;
}

//��`�Ƌ�`���������Ă��邩�H
BOOL CollRectToRect(RECT a, RECT b)
{
	if (a.left <= b.right
		&& a.top <= b.bottom
		&& a.right >= b.left
		&& a.bottom >= b.top)
	{
		//�������Ă���
		return TRUE;
	}

	//�������ĂȂ�
	return FALSE;
}

//��`�Ɠ_���������Ă��邩�H
BOOL CollRectToPoint(RECT rect, POINT pt)
{
	if (rect.left <= pt.x
		&& rect.top <= pt.y
		&& rect.right >= pt.x
		&& rect.bottom >= pt.y)
	{
		//�������Ă���
		return TRUE;
	}

	//�������ĂȂ�
	return FALSE;
}

//RECT�^�ŋ�`��`��
VOID DrawRect(RECT rect, unsigned int color, BOOL fill)
{
	//��`��`��
	DrawBox(
		rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		color,
		fill
	);

	return;
}

//Circle�^���ꎞ�I�Ɏ擾
CIRCLE GetCircle(POINT pt, float rad)
{
	CIRCLE c;

	c.pt = pt;
	c.radius = rad;

	return c;
}

//�~�Ɖ~���������Ă��邩�H
BOOL CollCircletoCircle(CIRCLE C1, CIRCLE C2)
{
	//�O�p�`�����
	float a = 0.0f;
	float b = 0.0f;
	float c = 0.0f;

	a = C1.pt.x - C2.pt.x;
	b = C1.pt.y - C2.pt.y;
	a = sqrt(a*a+b*b);

	if (c <= C1.radius + C2.radius)
	{
		//�������Ă���
		return TRUE;
	}

	//�������Ă��Ȃ�
	return FALSE;
}

//�~�Ɠ_���������Ă��邩�H
BOOL CollCircletoPoint(CIRCLE circle, POINT pt)
{
	//�O�p�`�����
	float dx = 0.0f;
	float dy = 0.0f;
	float dist = 0.0f;

	dx = circle.pt.x -pt.x;
	dy = circle.pt.y - pt.y;
	dist = sqrt(dx * dx + dy * dy);

	if (dist <= circle.radius)
	{
		//�������Ă���
		return TRUE;
	}

	//�������Ă��Ȃ�
	return FALSE;
}

//CIRCLE�^�ŕ`��
VOID DrawEn(CIRCLE circle, unsigned int color, BOOL fill)
{
	//�~��`��
	DrawCircle(circle.pt.x, circle.pt.y, circle.radius, color, fill);

	return;
}