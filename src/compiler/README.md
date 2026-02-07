# ZetPkg 컴파일러

이 디렉토리에는 ZetPkg로 컴파일해주는 헬퍼가 들어갑니다.

## 구조

```
src/compiler/
├── zetpkg/            # ZetPkg 컴파일러
│   ├── builder.ts     # 빌더 로직
│   ├── packager.ts    # 패키징 로직
│   └── validator.ts  # 빌드 검증
├── types.ts           # 타입 정의
└── index.ts           # 메인 익스포트
```

## 설명

ZetPkg 컴파일러는 소스 파일들을 ZetPkg 형식으로 패키징합니다.

## 기능

- 파일 트리를 ZetPkg 형식으로 변환
- 에셋 암호화 및 해시 검증
- 메타데이터 생성
- 빌드 검증
