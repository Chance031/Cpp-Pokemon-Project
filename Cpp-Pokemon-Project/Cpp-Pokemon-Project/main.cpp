#include <iostream>

#include "Pokemon.h" // 첫 번째 작업에서 만든 헤더 파일
#include "Skill.h"   // 첫 번째 작업에서 만든 헤더 파일

int main()
{
    // 빌드 환경이 정상임을 확인하기 위한 메시지 출력
    std::cout << "Hello World! Build environment is working." << std::endl;

    // 콘솔 창이 바로 닫히는 것을 방지 (Windows 환경)
    system("pause");

    return 0;
}