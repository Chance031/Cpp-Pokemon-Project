#pragma once

#include <string>
#include "Enums.h"

class Move {
public:
    // �⺻ ������
    Move() : id_(0), power_(0), accuracy_(0), maxPp_(0), currentPp_(0), priority_(0),
        isContact_(false), isSoundMove_(false), isSpreadMove_(false),
        isProtectable_(true), canBeRedirected_(true) {
    }

    // �����ͺ��̽��� ������
    Move(int id, const std::string& name, const std::string& description,
        Type type, MoveCategory category, int power, int accuracy, int pp,
        int priority, MoveTarget target, bool isContact, bool isSoundMove,
        bool isSpreadMove, bool isProtectable, bool canBeRedirected);

    // --- Getter �Լ� ---
    int GetId() const { return id_; }
    std::string GetName() const { return name_; }
    Type GetType() const { return type_; }
    MoveCategory GetCategory() const { return category_; }
    int GetPower() const { return power_; }
    int GetAccuracy() const { return accuracy_; }
    int GetCurrentPp() const { return currentPp_; }
    int GetMaxPp() const { return maxPp_; }
    int GetPriority() const { return priority_; }
    MoveTarget GetMoveTarget() const { return moveTarget_; }

    // �÷���(Flag) Getter
    bool GetIsContact() const { return isContact_; } // �̸� ����: IsContact -> GetIsContact
    bool GetIsSoundMove() const { return isSoundMove_; }
    bool GetIsSpreadMove() const { return isSpreadMove_; }
    bool GetIsProtectable() const { return isProtectable_; }
    bool GetCanBeRedirected() const { return canBeRedirected_; }

    // --- ��� �Լ� ---
    void DecrementPp(); // PP�� 1 ���ҽ�Ű�� �Լ�

private:
    int id_;
    std::string name_;
    std::string description_;

    Type type_;
    MoveCategory category_;
    int power_;
    int accuracy_;
    int maxPp_;
    int currentPp_;
    int priority_;

    MoveTarget moveTarget_;

    bool isContact_;
    bool isSoundMove_;
    bool isSpreadMove_;
    bool isProtectable_;
    bool canBeRedirected_;

    MoveExecutionType executionType_;
};