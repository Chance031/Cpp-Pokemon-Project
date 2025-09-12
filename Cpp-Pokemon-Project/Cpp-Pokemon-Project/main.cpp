#include <iostream>
#include <string>
#include <vector>
#include <numeric> // std::accumulate를 위해 추가

#include "DataManager.h"
#include "PokemonData.h"
#include "Enums.h"
#include "StringUtils.h"

// Stat enum을 문자열로 바꿔주는 헬퍼 함수 (출력용)
std::string StatToString(Stat stat) {
    static const std::map<Stat, std::string> statMap = {
        {Stat::HP, "HP"}, {Stat::ATTACK, "ATTACK"}, {Stat::DEFENSE, "DEFENSE"},
        {Stat::SPECIAL_ATTACK, "SPECIAL_ATTACK"}, {Stat::SPECIAL_DEFENSE, "SPECIAL_DEFENSE"},
        {Stat::SPEED, "SPEED"}
    };
    auto it = statMap.find(stat);
    return (it != statMap.end()) ? it->second : "NONE";
}

/**
 * @brief 단일 포켓몬 종족 데이터의 유효성을 검사하는 함수
 * @param species 검사할 포켓몬 종족 데이터
 * @return 유효하면 true, 아니면 false
 */
bool ValidatePokemonSpecies(const PokemonSpecies& species)
{
    std::vector<std::string> errors;

    // 1. 기본 정보 검사
    if (species.name_kr.empty()) errors.push_back("한글 이름이 비어있습니다.");
    if (species.id <= 0 || species.id > 151) errors.push_back("ID가 유효 범위를 벗어났습니다.");

    // 2. 능력치 검사 (모든 스탯이 0보다 커야 함)
    if (species.baseStats.size() != 6) errors.push_back("기초 능력치 개수가 6개가 아닙니다.");
    for (const auto& pair : species.baseStats) {
        if (pair.second <= 0) {
            errors.push_back(StatToString(pair.first) + " 능력치가 0 이하입니다.");
        }
    }

    // 3. 타입 검사
    if (species.type1 == Type::NONE) errors.push_back("타입1이 NONE으로 설정되어 있습니다.");

    // 4. 진화 정보 검사
    if (species.evolutionLevel > 0 && species.evolutionTargetId <= 0) {
        errors.push_back("진화 레벨은 있으나, 진화 대상 ID가 없습니다.");
    }
    // 이브이(133)는 예외 케이스로 진화 ID가 0이어야 함
    if (species.id == 133 && species.evolutionTargetId != 0) {
        errors.push_back("이브이의 evolutionTargetId는 0이어야 합니다 (특수 진화).");
    }

    // 5. 성장 데이터 검사
    if (species.catch_rate <= 0) errors.push_back("포획률이 0 이하입니다.");
    if (species.base_exp_yield <= 0) errors.push_back("기초 경험치가 0 이하입니다.");
    if (species.ev_yields.empty()) {
        errors.push_back("노력치(EV) 정보가 비어있습니다.");
    }
    else {
        int total_evs = 0;
        for (const auto& ev : species.ev_yields) {
            total_evs += ev.amount;
        }
        if (total_evs <= 0) errors.push_back("노력치(EV) 총량이 0 이하입니다.");
    }

    // 오류가 발견된 경우, 상세 내용을 출력
    if (!errors.empty()) {
        std::cout << "----------------------------------------\n";
        std::cout << "[오류] ID " << species.id << " - " << species.name_kr << " 데이터 검증 실패:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
        return false;
    }

    return true;
}

int main()
{
    // 1. 모든 데이터 로드
    try {
        DataManager::GetInstance().LoadAllData();
    }
    catch (const std::exception& e) {
        std::cerr << "[치명적 오류] 데이터 로딩 중 예외 발생: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n--- pokemon_species.csv 전체(1-151) 데이터 자동 검증 시작 ---\n" << std::endl;

    int passed_count = 0;
    const int total_pokemon = 151;

    // 2. 1번부터 151번까지 모든 포켓몬을 순회하며 검증
    for (int i = 1; i <= total_pokemon; ++i)
    {
        try {
            const PokemonSpecies& species = DataManager::GetInstance().GetPokemonSpecies(i);
            if (ValidatePokemonSpecies(species)) {
                passed_count++;
            }
        }
        catch (const std::out_of_range& e) {
            std::cout << "----------------------------------------\n";
            std::cout << "[치명적 오류] ID " << i << "번 포켓몬 데이터를 찾을 수 없습니다.\n";
        }
    }

    // 3. 최종 결과 요약
    std::cout << "\n========================================\n";
    std::cout << "          최종 검증 결과 요약\n";
    std::cout << "========================================\n";
    std::cout << "총 " << total_pokemon << "개의 포켓몬 중 " << passed_count << "개가 검증을 통과했습니다.\n";

    if (passed_count == total_pokemon) {
        std::cout << "🎉 축하합니다! 모든 포켓몬 데이터가 완벽합니다!" << std::endl;
    }
    else {
        std::cout << "❌ " << (total_pokemon - passed_count) << "개의 포켓몬에서 오류가 발견되었습니다. 위의 로그를 확인해주세요." << std::endl;
    }
    std::cout << "========================================\n";

    return 0;
}