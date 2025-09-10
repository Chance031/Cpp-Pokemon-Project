#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// CSV 한 줄을 받아 따옴표를 고려하여 필드 벡터로 분리하는 최종 파서 함수
std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::string field;
    bool in_quotes = false;

    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        }
        else if (c == ',' && !in_quotes) {
            result.push_back(field);
            field.clear();
        }
        else {
            field += c;
        }
    }
    result.push_back(field); // 마지막 필드 추가
    return result;
}

int main()
{
    std::cout << "--- moves.csv 파일 데이터 검증 시작 ---" << std::endl;
    std::string filePath = "./data/moves.csv";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filePath << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    int expectedColumnCount = 31;

    std::getline(file, line); // 헤더 줄은 검사에서 제외
    lineNumber++;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '\r') continue;

        // 새로운 파서 함수로 라인을 필드 벡터로 변환
        std::vector<std::string> fields = parseCsvLine(line);

        if (fields.size() != expectedColumnCount) {
            std::cout << "[오류] " << lineNumber << "번 줄: 컬럼 개수가 " << expectedColumnCount << "개여야 하는데, " << fields.size() << "개입니다." << std::endl;

            // 분할된 모든 필드를 상세히 출력
            std::cout << "    --- 분할된 필드 목록 ---" << std::endl;
            for (size_t i = 0; i < fields.size(); ++i) {
                std::cout << "    필드 " << i + 1 << ": [" << fields[i] << "]" << std::endl;
            }
            std::cout << "    --------------------------" << std::endl;
        }
    }

    file.close();
    std::cout << "\n--- 검증 완료 ---" << std::endl;
    std::cout << "위에 [오류] 메시지가 없다면, 모든 라인이 정상적으로 분리되었습니다." << std::endl;

    return 0;
}