#include "Move.h"
#include "MoveData.h"

// ������
Move::Move(const MoveData& data)
// ��� ���� �ʱ�ȭ
	: data_(&data),
	currentPp_(data.pp),    // ���� PP�� �ִ� PP�� �ʱ�ȭ
	maxPp_(data.pp),        // ���� '�ִ� PP'�� �⺻ PP�� �ʱ�ȭ
	ppUpStages_(0)          // PP ��ȭ Ƚ���� 0���� ����
{
}

// PP�� ���ҽ�Ű�� �Լ�
void Move::DecrementPp(int amount)
{
	currentPp_ -= amount;
	// PP�� 0���� �Ʒ��� �������� �ʵ��� ����
	if (currentPp_ < 0)
	{
		currentPp_ = 0;
	}
}

// PP�� ȸ����Ű�� �Լ�
void Move::RestorePp(int amount)
{
	currentPp_ += amount;
	// ���� PP�� �ִ� PP�� ���� �ʵ��� ����
	if (currentPp_ > maxPp_)
	{
		currentPp_ = maxPp_;
	}
}

// PP�� Ư�� ������ �����ϴ� �Լ�
void Move::SetPp(int value)
{
	currentPp_ = value;
	// ������ ���� 0�� �ִ� PP ���̸� ����� �ʵ��� ����
	if (currentPp_ < 0) currentPp_ = 0;
	if (currentPp_ > maxPp_) currentPp_ = maxPp_;
}