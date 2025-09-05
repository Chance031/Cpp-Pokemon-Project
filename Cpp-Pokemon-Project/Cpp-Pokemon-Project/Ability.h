#pragma once

#include <string>

enum class AbilityTrigger 
{
    ON_ENTRY,         // ���� �� (��: ����)
    ON_CONTACT,       // ���� �� (��: ������)
    PASSIVE           // ���� ȿ�� (��: ����)
};

class Ability
{
public:
    // ������ ����
    Ability() : id_(0), name_(""), description_(""), triggerType_(AbilityTrigger::PASSIVE) {}
    Ability(int id, const std::string& name, const std::string& description);

    // Getter �Լ�
    int GetId() const { return id_; }
    std::string GetName() const { return name_; }
    std::string GetDescription() const { return description_; }

private:
    int id_;
    std::string name_;
    std::string description_;
    AbilityTrigger triggerType_;
};

