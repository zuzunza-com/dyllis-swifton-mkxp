# 코어 플레이어 엔진 공통 명세

이 디렉토리에는 코어 플레이어 엔진의 공통 명세만 정의합니다.
실제 구현은 아직 제작하지 않습니다.

## 구조

```
core/
├── spec/              # 공통 명세 문서
│   ├── interface.md  # 엔진 인터페이스 명세
│   ├── lifecycle.md  # 생명주기 명세
│   └── api.md        # API 명세
└── types.ts          # 타입 정의
```

## 설명

코어 플레이어 엔진은 모든 엔진이 공통으로 구현해야 하는 인터페이스를 정의합니다.
이 명세를 통해 다양한 엔진을 통합적으로 관리할 수 있습니다.

## 공통 인터페이스

모든 엔진은 다음 인터페이스를 구현해야 합니다:

```typescript
interface CoreEngine {
  // 초기화
  init(config: EngineConfig): Promise<void>;
  
  // 종료
  cleanup(): Promise<void>;
  
  // 게임 로드
  loadGame(data: GameData): Promise<void>;
  
  // 프레임 업데이트
  update(deltaTime: number): void;
  
  // 렌더링
  render(): void;
  
  // 입력 처리
  handleInput(event: InputEvent): void;
  
  // 리소스 해제
  dispose(): void;
}
```

## 구현 예정

실제 코어 플레이어 엔진 구현은 추후 진행됩니다.
현재는 공통 명세만 정의합니다.
