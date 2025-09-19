#include "framework.h"
#include "BattleManager.h"
#include "DataManager.h"
#include "Move.h"
#include "StringUtils.h"

BattleManager::BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty)
    : playerParty_(playerParty), opponentParty_(opponentParty)
{
    std::random_device rd;
    randomNumberEngine_.seed(rd());
}

void BattleManager::Start(std::vector<TurnEvent>& events)
{
    // �� ��Ƽ�� ù ��° ���ϸ��� ��Ʋ �ʵ��� Ȱ�� ���ϸ����� ����
    playerActivePokemon_ = &playerParty_[0];
    opponentActivePokemon_ = &opponentParty_[0];

    // ���� �� �ߵ��ϴ� Ư�� ó�� (���ǵ尡 ���� ���ϸ����)
    if (playerActivePokemon_->GetStat(Stat::SPEED) >= opponentActivePokemon_->GetStat(Stat::SPEED))
    {
        TriggerSwitchInAbilities(playerActivePokemon_, opponentActivePokemon_, events);
        TriggerSwitchInAbilities(opponentActivePokemon_, playerActivePokemon_, events);
    }
    else
    {
        TriggerSwitchInAbilities(opponentActivePokemon_, playerActivePokemon_, events);
        TriggerSwitchInAbilities(playerActivePokemon_, opponentActivePokemon_, events);
    }
}

TurnResult BattleManager::ProcessTurn(const BattleAction& playerAction)
{
    TurnResult result; // �� ���� ��� ����� ���� ��ü

    // AI�� �ൿ�� ����
    BattleAction opponentAction = SelectOpponentAction();

    // �켱���� ���ǵ带 ���Ͽ� ���� ���� �ൿ���� ����
    auto actionOrder = DetermineActionOrder(playerAction, opponentAction);
    Pokemon* firstAttacker = actionOrder.first;
    Move* firstMove = actionOrder.second;
    Pokemon* secondAttacker = (firstAttacker == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
    Move* secondMove = (firstAttacker == playerActivePokemon_) ? opponentAction.move : playerAction.move;

    // ù ��° ���ϸ� �ൿ ���� �� �̺�Ʈ ���
    Pokemon* firstTarget = (firstAttacker == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
    ExecuteAction(firstAttacker, firstTarget, firstMove, result.events);

    // �ൿ �� ��밡 �����ߴ��� Ȯ��
    if (!firstTarget->IsFainted())
    {
        Pokemon* secondTarget = firstAttacker;
        ExecuteAction(secondAttacker, secondTarget, secondMove, result.events);
    }

    // �� ���� ȿ�� ó�� (��, ȭ�� ������ ��)
    if (!playerActivePokemon_->IsFainted())
    {
        ProcessEndOfTurnEffects(playerActivePokemon_, result.events);
    }
    if (!opponentActivePokemon_->IsFainted())
    {
        ProcessEndOfTurnEffects(opponentActivePokemon_, result.events);
    }

    // ��� �̺�Ʈ�� ��ϵ� ���� ����� BattleScene�� ��ȯ
    return result;
}

bool BattleManager::IsBattleOver()
{
    bool playerAllFainted = std::all_of(playerParty_.begin(), playerParty_.end(), [](const Pokemon& p) { return p.IsFainted(); });
    bool opponentAllFainted = std::all_of(opponentParty_.begin(), opponentParty_.end(), [](const Pokemon& p) { return p.IsFainted(); });
    return playerAllFainted || opponentAllFainted;
}

BattleAction BattleManager::SelectOpponentAction()
{
    BattleAction action;
    action.type = PlayerActionType::FIGHT;
    auto& moveset = opponentActivePokemon_->GetMovesetForModify();
    if (moveset.empty()) 
    {
        action.move = nullptr;
        return action;
    }

    std::uniform_int_distribution<> distrib(0, (int)moveset.size() - 1);
    int randomIndex = distrib(randomNumberEngine_);
    action.move = &moveset[randomIndex];

    return action;
}

void BattleManager::ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move, std::vector<TurnEvent>& events)
{
    // ��� ��� �޽����� �̺�Ʈ�� �߰�
    events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, attacker->GetName() + "�� " + move->GetName() + "!" });
    move->DecrementPp();

    if (!HandleMoveAccuracy(attacker, move))
    {
        events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, "������ ����� ��������!" });
        return;
    }
    
    // ������ ��� �� ����
    DamageResult damageResult;
    if (move->GetCategory() != MoveCategory::STATUS)
    {
        damageResult = CalculateAndApplyDamage(attacker, target, move, events);
        if (damageResult.typeEffectiveness > 1.0)
        {
            events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, "ȿ���� �����ߴ�!" });
        }
        if (damageResult.typeEffectiveness < 1.0 && damageResult.typeEffectiveness > 0) 
        {
            events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, "ȿ���� ������ ���ϴ�..." });
        }
        if (damageResult.typeEffectiveness == 0)
        {
            events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, "ȿ���� ���� �� ����..." });
        }
        if (damageResult.damageDealt > 0)
        {
            events.push_back({ TurnEventType::DAMAGE, target, "", damageResult.damageDealt });
        }
    }

    // �������� �� ��, ����ִٸ� �ΰ� ȿ���� �����մϴ�.
    if (!target->IsFainted())
    {
        ApplyMoveEffect(attacker, target, move, events);
    }

    if (target->IsFainted())
    {
        events.push_back({ TurnEventType::FAINT, target, target->GetName() + "��(��) ��������!" });
    }
}

bool BattleManager::HandleMoveAccuracy(Pokemon* attacker, const Move* move)
{
    int moveAccuracy = move->GetAccuracy();
    if (moveAccuracy > 100)
    {
        return true; // ���߱�
    }

    std::uniform_int_distribution<> distrib(1, 100);
    int randomValue = distrib(randomNumberEngine_);

    // TODO: ���߷�/ȸ���� ��ũ ���� ���� �߰�
    return randomValue <= moveAccuracy;
}

void BattleManager::ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events)
{
    int effectId = move->GetEffectId();
    if (move->GetEffectId() == 0)
    {
        return;
    }

    const MoveEffectData& effect = DataManager::GetInstance().GetMoveEffectData(move->GetEffectId());
    int effectChance = move->GetEffectChance();

    if (effectChance > 0)
    {
        std::uniform_int_distribution<> distrib(1, 100);
        if (distrib(randomNumberEngine_) > effectChance) 
        {
            return; // Ȯ�� ���� ����
        }
    }

    switch (effect.category)
    {
    case EffectCategory::STAT_CHANGE:
        for (const auto& change : effect.statChanges)
        {
            target->ApplyStatStageChange(change.stat, change.stages);
            std::string message = target->GetName() + "�� " + StringUtils::StatToKorean(change.stat) + "��(��) " + (change.stages < 0 ? "��������!" : "�ö󰬴�!");
            events.push_back({ TurnEventType::STAT_CHANGE, target, message, 0, change.stat, change.stages });
        }
        break;
    case EffectCategory::PRIMARY_STATUS:
        if (target->GetPrimaryStatus() == StatusCondition::NONE)
        {
            target->SetPrimaryStatus(effect.primaryStatus);
            std::string message = target->GetName() + "��(��) " + StringUtils::StatusConditionToKorean(effect.primaryStatus) + " ���°� �Ǿ���!";
            events.push_back({ TurnEventType::STATUS_EFFECT, target, message, 0, Stat::HP, 0, effect.primaryStatus });
        }
        break;
    default:
        break;
    }
}

DamageResult BattleManager::CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events)
{
    DamageResult result;

    // �޼� ����
    bool isCritical = false;
    std::uniform_int_distribution<> distrib(1, 24);
    if (distrib(randomNumberEngine_) == 1)
    {
        isCritical = true;
        events.push_back({ TurnEventType::TEXT_MESSAGE, nullptr, "�޼ҿ� �¾Ҵ�!" });
    }

    // �⺻ ������ ��꿡 �ʿ��� ���� ����
    int attackerLevel = attacker->GetLevel();
    int movePower = move->GetPower();
    int attackStat, defenseStat;

    auto getStageMultiplier = [](int stage) -> double 
        {
        return (stage > 0) ? (2.0 + stage) / 2.0 : 2.0 / (2.0 - stage);
        };

    if (move->GetCategory() == MoveCategory::PHYSICAL) 
    {
        int attackStage = isCritical && attacker->GetStatStage(Stat::ATTACK) < 0 ? 0 : attacker->GetStatStage(Stat::ATTACK);
        int defenseStage = isCritical && target->GetStatStage(Stat::DEFENSE) > 0 ? 0 : target->GetStatStage(Stat::DEFENSE);
        attackStat = static_cast<int>(attacker->GetStat(Stat::ATTACK) * getStageMultiplier(attackStage));
        defenseStat = static_cast<int>(target->GetStat(Stat::DEFENSE) * getStageMultiplier(defenseStage));
    }
    else 
    {
        int attackStage = isCritical && attacker->GetStatStage(Stat::SPECIAL_ATTACK) < 0 ? 0 : attacker->GetStatStage(Stat::SPECIAL_ATTACK);
        int defenseStage = isCritical && target->GetStatStage(Stat::SPECIAL_DEFENSE) > 0 ? 0 : target->GetStatStage(Stat::SPECIAL_DEFENSE);
        attackStat = static_cast<int>(attacker->GetStat(Stat::SPECIAL_ATTACK) * getStageMultiplier(attackStage));
        defenseStat = static_cast<int>(target->GetStat(Stat::SPECIAL_DEFENSE) * getStageMultiplier(defenseStage));
    }

    if (defenseStat == 0)
    {
        defenseStat = 1;
    }

    // ������ ���� ����
    int baseDamage = ((((attacker->GetLevel() * 2 / 5) + 2) * move->GetPower() * attackStat / std::max(1, defenseStat)) / 50) + 2;

    // ���� ����ġ(modifier) ���
    double modifier = 1.0;
    if (isCritical) modifier *= 1.5;
    if (move->GetType() == attacker->GetType1() || move->GetType() == attacker->GetType2())
    {
        modifier *= 1.5;
    }

    result.typeEffectiveness = DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType1());
    if (target->GetType2() != Type::NONE) 
    {
        result.typeEffectiveness *= DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType2());
    }
    modifier *= result.typeEffectiveness;

    // ���� ������ Ȯ�� �G ����
    result.damageDealt = std::max(1, static_cast<int>(baseDamage * modifier));
    if (result.typeEffectiveness == 0)
    {
        result.damageDealt = 0;
    }

    target->TakeDamage(result.damageDealt);
    return result;
}

std::pair<Pokemon*, Move*> BattleManager::DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    if (playerAction.move->GetPriority() != opponentAction.move->GetPriority()) 
    {
        return (playerAction.move->GetPriority() > opponentAction.move->GetPriority()) ? std::make_pair(playerActivePokemon_, playerAction.move) : std::make_pair(opponentActivePokemon_, opponentAction.move);
    }
    return (playerActivePokemon_->GetStat(Stat::SPEED) >= opponentActivePokemon_->GetStat(Stat::SPEED)) ? std::make_pair(playerActivePokemon_, playerAction.move) : std::make_pair(opponentActivePokemon_, opponentAction.move);
}

void BattleManager::ProcessEndOfTurnEffects(Pokemon* pokemon, std::vector<TurnEvent>& events)
{
    if (pokemon->IsFainted())
    {
        return;
    }

    if (pokemon->GetPrimaryStatus() == StatusCondition::POISON) 
    {
        events.push_back({ TurnEventType::TEXT_MESSAGE, pokemon, pokemon->GetName() + "��(��) ���� �������� �Ծ���!" });
        int poisonDamage = std::max(1, pokemon->GetMaxHP() / 8);
        pokemon->TakeDamage(poisonDamage);
        events.push_back({ TurnEventType::DAMAGE, pokemon, "", poisonDamage });
        if (pokemon->IsFainted()) 
        {
            events.push_back({ TurnEventType::FAINT, pokemon, pokemon->GetName() + "��(��) ��������!" });
        }
    }
}

void BattleManager::TriggerSwitchInAbilities(Pokemon* switchedInPokemon, Pokemon* opponent, std::vector<TurnEvent>& events)
{
    if (switchedInPokemon->IsFainted())
    {
        return;
    }
    const AbilityData* ability = switchedInPokemon->GetAbility();
    if (!ability || ability->trigger != AbilityTrigger::ON_SWITCH_IN)
    {
        return;
    }

    if (ability->id == 22) 
    {
        events.push_back({ TurnEventType::TEXT_MESSAGE, switchedInPokemon, switchedInPokemon->GetName() + "�� Ư�� [����]!" });
        opponent->ApplyStatStageChange(Stat::ATTACK, -1);
        std::string message = opponent->GetName() + "�� ������ ��������!";
        events.push_back({ TurnEventType::STAT_CHANGE, opponent, message, 0, Stat::ATTACK, -1 });
    }
}

void BattleManager::ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex, std::vector<TurnEvent>& events)
{
    if (activePokemon && !activePokemon->IsFainted())
    {
        events.push_back({ TurnEventType::TEXT_MESSAGE, activePokemon, activePokemon->GetName() + ", ���ƿ�!" });
        activePokemon->ResetStatStages();
    }
    activePokemon = &party[newPokemonIndex];
    events.push_back({ TurnEventType::TEXT_MESSAGE, activePokemon, "����! " + activePokemon->GetName() + "!" });

    Pokemon* opponent = (activePokemon == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
    TriggerSwitchInAbilities(activePokemon, opponent, events);
}