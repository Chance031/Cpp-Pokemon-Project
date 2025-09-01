#include <iostream>

#include "Pokemon.h" // 첫 번째 작업에서 만든 헤더 파일
#include "Skill.h"   // 첫 번째 작업에서 만든 헤더 파일

int main()
{
    Pokemon pikachu("피카츄", 100, 55, 40, 90);
    Skill s1("전기쇼크", 40, 100, 30);
    Skill s2("전광석화", 40, 100, 30);
    Skill s3("아이언테일", 100, 75, 15);
    Skill s4("10만볼트", 90, 100, 15);
    Skill s5("파괴광선", 150, 90, 5); // 5번째 기술

    // 기술 추가 시도 및 결과 확인
    pikachu.addSkill(s1);
    pikachu.addSkill(s2);
    pikachu.addSkill(s3);
    pikachu.addSkill(s4);

    if (pikachu.addSkill(s5)) {
        std::cout << "파괴광선 배우기 성공!" << std::endl;
    }
    else {
        std::cout << "기술이 가득 차서 파괴광선을 배울 수 없다!" << std::endl;
    }

    // 포켓몬 정보 출력
    std::cout << "\n==== 포켓몬 정보 ====" << std::endl;
    std::cout << "--- 기술 목록 ---" << std::endl;
    for (const auto& skill : pikachu.getSkills()) {
        std::cout << "- " << skill.getName() << std::endl;
    }
    std::cout << "====================" << std::endl;

    system("pause");
    return 0;
}