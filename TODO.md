# Kick It Up C++ — 할 일 (TODO)

## 긴급 (버그)

- [ ] 게임 플레이 시 노트-오디오 싱크 정밀 조정
- [ ] 종료 시 segfault 원인 파악 및 수정
- [ ] 2P 모드 실제 동작 검증
- [ ] Coop/Double 모드 게임플레이 테스트

## 필수

- [ ] 사운드 위치 및 연주 시간 관련 라이브러리는 반드시 libmpg123를 사용할것.
- [ ] 변수명은 카멜케이스를 사용할것.
- [ ] class명, struct 첫문자는 대문자를 사용할것.
- [ ] 함수명은 소문자로 시작할것.
- [ ] `_strupr`/`_strdup` 메모리 누수 수정 (select.cpp)
- [ ] Song.mp3 파일명 → song.mp3 (대소문자 통일)
- [ ] `SDL_OpenAudioDevice` 다중 호출 문제 해결 (Sound + CMedia 충돌)
- [ ] `#include` 순환참조 해소 (main.h ↔ config.h)
- [ ] Windows.h 의존성 완전 제거 (WindowProc, DefWindowProc 등)
- [ ] `compile_commands.json` 생성 (IDE 지원)

## 코드 품질

- [ ] KIU_STAGE 함수 분할 (2200줄 → 5~6개 함수)
- [ ] DrawArrow1p/2p 공통화 (700줄×2 → 1개 함수)
- [ ] 헝가리안 표기법 완전 제거 (dwState, lpData, szFile 등)
- [ ] 한국어 깨진 주석 모두 제거
- [ ] `using namespace` 대신 명시적 std:: 사용
- [ ] `sprintf` → `snprintf` 버퍼 오버플로우 방지
- [ ] `rand()` → `<random>` (C++11)

## 테스트

- [ ] CMakeLists.txt에 CTest + Google Test 연동
- [ ] Player::judgeAnaly() 단위 테스트
- [ ] Animator 시간 기반 테스트
- [ ] Renderer::drawGauge() 출력 검증
- [ ] TimingEngine::arrowY() 수식 검증

## 기능

- [ ] 조이스틱 입력 지원
- [ ] 사운드 볼륨 조절
- [ ] FPS 독립적 게임 루프 (현재 60fps 가정)
- [ ] 전체화면/창모드 전환
- [ ] BGA(배경 애니메이션) 지원

## 배포

- [ ] CMake install 타겟
- [ ] 리소스 파일 포함 패키징
- [ ] Flatpak/AppImage 빌드 스크립트
- [ ] GitHub Actions CI (빌드 + 테스트)
