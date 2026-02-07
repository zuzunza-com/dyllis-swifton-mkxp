# 엔진 디렉토리

이 디렉토리에는 지원하는 엔진의 명세와 구동 wasm, 구동 JS 등이 빌드되기 위한 소스코드가 들어갑니다.

## 구조

```
src/engine/
├── Palcon-Flash/      # Adobe Flash 에뮬레이팅 엔진
├── PixiPkg/           # PixiJS 실행 엔진 (기존 PixiPkg)
├── Palcon-RGSS/       # RGSS 구동 엔진
└── core/              # 코어 플레이어 엔진 공통 명세 (구현 없음)
```

## 엔진별 설명

### Palcon-Flash
Adobe Flash를 에뮬레이팅하는 자체 제작 엔진입니다.

### PixiPkg
기존 PixiJS 기반 실행 패키지 엔진입니다.

### Palcon-RGSS
RGSS(Ruby Game Scripting System) 게임을 구동하는 엔진입니다.
WASM으로 빌드되며, C언어 Wrapper를 사용합니다.

### Core (공통 명세)
코어 플레이어 엔진의 공통 명세만 정의합니다. 실제 구현은 아직 제작하지 않습니다.
