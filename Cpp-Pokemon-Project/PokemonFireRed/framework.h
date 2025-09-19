#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// --- GDI+ 사용을 위한 필수 요소들 ---
#include <gdiplus.h>                    // GDI+ 헤더 파일
#pragma comment(lib, "gdiplus.lib")     // GDI+ 라이브러리 링크
using namespace Gdiplus;                // Gdiplus 네임스페이스 사용

// --- 프로젝트 전역에서 사용하는 표준 라이브러리 ---
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <memory>