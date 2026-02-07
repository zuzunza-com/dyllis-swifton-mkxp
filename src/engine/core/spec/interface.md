# 코어 플레이어 엔진 인터페이스 명세

## 개요

모든 엔진이 구현해야 하는 공통 인터페이스를 정의합니다.

## 인터페이스

### CoreEngine

```typescript
interface CoreEngine {
  init(config: EngineConfig): Promise<void>;
  cleanup(): Promise<void>;
  loadGame(data: GameData, callbacks?: EngineCallbacks): Promise<void>;
  update(deltaTime: number): void;
  render(): void;
  handleInput(event: InputEvent): void;
  dispose(): void;
  getState(): EngineState;
}
```

## 메서드 설명

### init(config: EngineConfig): Promise<void>

엔진을 초기화합니다.

**매개변수:**
- `config`: 엔진 설정

**예외:**
- 초기화 실패 시 Error를 throw합니다.

### cleanup(): Promise<void>

엔진을 정리합니다. 리소스를 해제하고 상태를 초기화합니다.

### loadGame(data: GameData, callbacks?: EngineCallbacks): Promise<void>

게임을 로드합니다.

**매개변수:**
- `data`: 게임 데이터
- `callbacks`: 이벤트 콜백 (선택)

**예외:**
- 로드 실패 시 Error를 throw합니다.

### update(deltaTime: number): void

게임 로직을 업데이트합니다.

**매개변수:**
- `deltaTime`: 이전 프레임부터 경과한 시간 (밀리초)

### render(): void

화면을 렌더링합니다.

### handleInput(event: InputEvent): void

입력을 처리합니다.

**매개변수:**
- `event`: 입력 이벤트

### dispose(): void

리소스를 해제합니다. cleanup()과 유사하지만 더 강력한 정리를 수행합니다.

### getState(): EngineState

현재 엔진 상태를 반환합니다.

## 상태 전이

```
UNINITIALIZED -> INITIALIZING -> READY -> LOADING -> RUNNING
                                              |
                                              v
                                           PAUSED
                                              |
                                              v
                                            ERROR
```
