# Palcon-Flash 엔진

Adobe Flash를 에뮬레이팅하는 자체 제작 엔진입니다.

## 구조

```
Palcon-Flash/
├── src/               # 소스코드
│   ├── wasm/          # WASM 빌드 소스 (C/C++/Rust)
│   ├── js/            # JavaScript 런타임
│   └── spec/          # 엔진 명세
├── build/             # 빌드 스크립트
└── README.md          # 이 파일
```

## 설명

Palcon-Flash는 Adobe Flash Player를 완전히 에뮬레이팅하는 엔진입니다.
- ActionScript 1.0/2.0/3.0 지원
- SWF 파일 포맷 파싱
- Flash API 구현
- 자체 제작 (Ruffle 등 외부 라이브러리 의존 없음)

## 빌드

빌드 방법은 추후 문서화됩니다.
