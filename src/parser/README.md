# ZetPkg 파서

이 디렉토리에는 기존 PixiPkg(ZetPkg로 이름 변경) 방식의 파싱 라이브러리가 들어갑니다.

## 구조

```
src/parser/
├── zetpkg/            # ZetPkg 파서 구현
│   ├── loader.ts      # 패키지 로더
│   ├── detector.ts    # 포맷 감지
│   └── validator.ts   # 유효성 검증
├── types.ts           # 타입 정의
└── index.ts           # 메인 익스포트
```

## 설명

ZetPkg는 기존 PixiPkg의 후속 규격입니다.
- 포맷명이 PixiPkg에서 ZetPkg로 변경되었습니다.
- 기존 PixiPkg와의 호환성을 유지합니다.
- 향후 확장 가능한 구조로 설계되었습니다.

## 마이그레이션

기존 PixiPkg 코드는 이 디렉토리로 이동하고, 포맷명을 ZetPkg로 변경해야 합니다.
