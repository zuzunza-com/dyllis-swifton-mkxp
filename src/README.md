# Palcon 소스 디렉토리

이 디렉토리에는 Palcon의 핵심 소스코드가 들어갑니다.

## 구조

```
src/
├── engine/           # 게임 엔진 구현
│   ├── Palcon-Flash/    # Adobe Flash 에뮬레이팅 엔진
│   ├── PixiPkg/         # PixiJS 실행 엔진
│   ├── Palcon-RGSS/     # RGSS 구동 엔진
│   └── core/            # 코어 플레이어 엔진 공통 명세
├── lib/              # Palcon 라이브러리
├── parser/           # ZetPkg 파싱 라이브러리
└── compiler/         # ZetPkg 컴파일 헬퍼
```

## 디렉토리 설명

### engine/
지원하는 엔진의 명세와 구동 wasm, 구동 JS 등이 빌드되기 위한 소스코드가 들어갑니다.

- **Palcon-Flash**: Adobe Flash를 에뮬레이팅하는 자체 제작 엔진
- **PixiPkg**: 기존 PixiJS 기반 실행 패키지 엔진
- **Palcon-RGSS**: RGSS 게임을 구동하는 엔진 (WASM 빌드)
- **core**: 코어 플레이어 엔진 공통 명세 (구현 없음)

### lib/
Palcon이 동작하기 위한 라이브러리가 들어갑니다.

### parser/
기존 PixiPkg(ZetPkg로 이름 변경) 방식의 파싱 라이브러리가 들어갑니다.

### compiler/
ZetPkg로 컴파일해주는 헬퍼가 들어갑니다.

## 패키지

### @alibrary-illusion
Palcon 라이브러리의 공개 패키지입니다.
`packages/@alibrary-illusion/` 디렉토리에 위치합니다.

## 빌드

각 엔진별 빌드 방법은 해당 디렉토리의 README를 참조하세요.

- Palcon-RGSS: `src/engine/Palcon-RGSS/build/Makefile` 또는 `build.sh` 사용
