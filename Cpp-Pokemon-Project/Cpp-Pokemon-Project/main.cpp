#include <iostream>
#include <string>
#include "DataManager.h"
#include "MoveData.h" // MoveData 구조체를 사용하기 위해

int main()
{
    std::cout << "--- 기술 데이터 로드 및 테스트 ---" << std::endl;

    // 1. 모든 CSV 데이터 로드
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    try
    {
        // 2. ID 33번 '몸통박치기' 데이터 가져오기
        const MoveData& tackleData = DataManager::GetInstance().GetMoveData(33);

        // 3. 가져온 데이터의 주요 항목들 출력하여 검증
        std::cout << "### ID 33번 기술 데이터 검증 ###" << std::endl;
        std::cout << "한글 이름: " << tackleData.name_kr << " (예상: 몸통박치기)" << std::endl;
        std::cout << "영어 식별자: " << tackleData.identifier << " (예상: tackle)" << std::endl;
        std::cout << "위력: " << tackleData.power << " (예상: 40)" << std::endl;
        std::cout << "PP: " << tackleData.pp << " (예상: 35)" << std::endl;
        std::cout << "부가 효과 ID: " << tackleData.effect_id << " (예상: NONE)" << std::endl;
        std::cout << "접촉 기술 여부: " << (tackleData.is_contact ? "예" : "아니오") << " (예상: 예)" << std::endl;

        std::cout << "\n-------------------------------------\n" << std::endl;

        // 4. ID 45번 '울음소리' 데이터 가져오기
        const MoveData& growlData = DataManager::GetInstance().GetMoveData(45);

        std::cout << "### ID 45번 기술 데이터 검증 ###" << std::endl;
        std::cout << "한글 이름: " << growlData.name_kr << " (예상: 울음소리)" << std::endl;
        std::cout << "분류: " << (growlData.category == MoveCategory::STATUS ? "변화" : "??") << " (예상: 변화)" << std::endl;
        std::cout << "부가 효과 ID: " << growlData.effect_id << " (예상: STAT_LOWER_ATTACK_1)" << std::endl;
        std::cout << "소리 기술 여부: " << (growlData.is_sound ? "예" : "아니오") << " (예상: 예)" << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cerr << "[실패] 기술 데이터를 가져오는 중 오류 발생: " << e.what() << std::endl;
    }

    std::cout << "\n--- 테스트 종료 ---" << std::endl;
    return 0;
}