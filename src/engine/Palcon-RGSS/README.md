# Palcon-RGSS 엔진

RGSS(Ruby Game Scripting System) 게임을 구동하는 엔진입니다.

## 구조

```
Palcon-RGSS/
├── src/                    # 핵심 엔진 소스
│   ├── binding/            # Ruby 바인딩
│   │   ├── binding-mri/    # MRI 바인딩
│   │   └── binding-mruby/  # mruby 바인딩
│   ├── graphics/           # 그래픽스 시스템
│   ├── audio/              # 오디오 시스템
│   ├── input/              # 입력 시스템
│   ├── filesystem/         # 파일시스템
│   ├── platform/           # 플랫폼 추상화
│   │   ├── win32-emscripten.h
│   │   └── win32-emscripten.c
│   └── rgss.c              # RGSS 코어
├── wasm/                   # WASM 빌드용 래퍼
│   ├── wrapper.c           # Emscripten 인터페이스
│   └── emscripten-bridge.c # Emscripten 브릿지
└── build/                  # 빌드 시스템
    ├── CMakeLists.txt      # CMake 설정
    ├── Makefile            # MakeCompiler 양식
    └── build.sh            # 빌드 스크립트
```

## 설명

Palcon-RGSS는 RPG Maker 게임을 실행하는 엔진입니다.
- RGSS 스크립트 실행 (MRI 또는 mruby)
- WASM으로 빌드됨
- C언어 Wrapper 사용
- Win32 API 호환 레이어 (Emscripten)
- MakeCompiler 양식 제공

## 빌드

### 요구사항

- Emscripten SDK
- CMake 3.10 이상
- MRI 또는 mruby 라이브러리

### 빌드 방법

#### CMake 사용 (권장)

```bash
cd build
make build BINDING=MRUBY    # mruby 바인딩
make build BINDING=MRI      # MRI 바인딩
```

#### 빌드 스크립트 사용

```bash
cd build
./build.sh MRUBY Release    # mruby 바인딩, Release 빌드
./build.sh MRI Debug        # MRI 바인딩, Debug 빌드
```

### 빌드 옵션

- `BINDING`: 바인딩 타입 (MRI 또는 MRUBY)
- `BUILD_TYPE`: 빌드 타입 (Release 또는 Debug)

## 참고

이 엔진은 `refference/mkxp` 소스코드를 참고하여 구현되었습니다.
mkxp의 구조와 API를 기반으로 하되, WASM 환경에 맞게 수정되었습니다.

## TASK

- [ ] Graphics 시스템 구현
- [ ] Audio 시스템 구현
- [ ] Input 시스템 구현
- [ ] Filesystem 구현
- [ ] MRI/mruby 바인딩 완성
- [ ] RGSS 스크립트 로더 구현
- [ ] 게임 데이터 파서 구현
