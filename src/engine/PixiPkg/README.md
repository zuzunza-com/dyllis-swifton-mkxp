# PixiPkg 엔진

기존 PixiJS 기반 실행 패키지 엔진입니다.

## 구조

```
PixiPkg/
├── src/               # 소스코드
│   ├── runtime/       # 런타임 구현
│   ├── loader/        # 패키지 로더
│   └── spec/          # 엔진 명세
├── build/             # 빌드 스크립트
└── README.md          # 이 파일
```

## 설명

PixiPkg는 PixiJS를 기반으로 한 게임 실행 패키지 형식입니다.
- PixiJS 기반 렌더링
- ZetPkg 형식 지원
- 기존 PixiPkg 호환성 유지
