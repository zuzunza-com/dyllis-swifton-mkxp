# 코어 플레이어 엔진 API 명세

## 개요

코어 플레이어 엔진의 공개 API를 정의합니다.

## 타입 정의

### EngineConfig

엔진 초기화 설정입니다.

```typescript
interface EngineConfig {
  width: number;              // 화면 너비
  height: number;             // 화면 높이
  canvas?: HTMLCanvasElement; // 캔버스 요소 (선택)
  options?: Record<string, any>; // 추가 옵션
}
```

### GameData

게임 데이터입니다.

```typescript
interface GameData {
  data: string | ArrayBuffer | Blob; // 게임 파일 경로 또는 데이터
  type: 'flash' | 'pixipkg' | 'rgss' | 'zetpkg'; // 게임 타입
  metadata?: Record<string, any>; // 메타데이터
}
```

### InputEvent

입력 이벤트입니다.

```typescript
interface InputEvent {
  type: 'keydown' | 'keyup' | 'mousedown' | 'mouseup' | 'mousemove' | 'wheel';
  code: number;
  x?: number;
  y?: number;
  delta?: number;
}
```

### EngineCallbacks

엔진 이벤트 콜백입니다.

```typescript
interface EngineCallbacks {
  onLoaded?: () => void;
  onError?: (error: Error) => void;
  onProgress?: (progress: number) => void;
}
```

### EngineState

엔진 상태 열거형입니다.

```typescript
enum EngineState {
  UNINITIALIZED = 'uninitialized',
  INITIALIZING = 'initializing',
  READY = 'ready',
  LOADING = 'loading',
  RUNNING = 'running',
  PAUSED = 'paused',
  ERROR = 'error',
}
```

## 인터페이스

### CoreEngine

모든 엔진이 구현해야 하는 공통 인터페이스입니다.

#### init(config: EngineConfig): Promise<void>

엔진을 초기화합니다.

**매개변수:**
- `config`: 엔진 설정

**반환값:**
- Promise<void>

**예외:**
- 초기화 실패 시 Error를 throw합니다.

**예제:**
```typescript
await engine.init({ width: 800, height: 600 });
```

#### cleanup(): Promise<void>

엔진을 정리합니다.

**반환값:**
- Promise<void>

**예제:**
```typescript
await engine.cleanup();
```

#### loadGame(data: GameData, callbacks?: EngineCallbacks): Promise<void>

게임을 로드합니다.

**매개변수:**
- `data`: 게임 데이터
- `callbacks`: 이벤트 콜백 (선택)

**반환값:**
- Promise<void>

**예외:**
- 로드 실패 시 Error를 throw합니다.

**예제:**
```typescript
await engine.loadGame({
  data: gameFile,
  type: 'zetpkg'
}, {
  onProgress: (progress) => console.log(`Loading: ${progress}%`),
  onLoaded: () => console.log('Game loaded!'),
  onError: (error) => console.error('Load error:', error)
});
```

#### update(deltaTime: number): void

게임 로직을 업데이트합니다.

**매개변수:**
- `deltaTime`: 이전 프레임부터 경과한 시간 (밀리초)

**예제:**
```typescript
let lastTime = Date.now();
function gameLoop() {
  const now = Date.now();
  const deltaTime = now - lastTime;
  lastTime = now;
  
  engine.update(deltaTime);
  engine.render();
  
  requestAnimationFrame(gameLoop);
}
```

#### render(): void

화면을 렌더링합니다.

**예제:**
```typescript
engine.render();
```

#### handleInput(event: InputEvent): void

입력을 처리합니다.

**매개변수:**
- `event`: 입력 이벤트

**예제:**
```typescript
canvas.addEventListener('keydown', (e) => {
  engine.handleInput({
    type: 'keydown',
    code: e.keyCode
  });
});
```

#### dispose(): void

리소스를 해제합니다.

**예제:**
```typescript
engine.dispose();
```

#### getState(): EngineState

현재 엔진 상태를 반환합니다.

**반환값:**
- EngineState

**예제:**
```typescript
const state = engine.getState();
if (state === EngineState.RUNNING) {
  console.log('Game is running');
}
```

## 사용 예제

```typescript
import { CoreEngine } from '@alibrary-illusion';

// 엔진 초기화
const engine = new MyEngine();
await engine.init({ width: 800, height: 600 });

// 게임 로드
await engine.loadGame({
  data: gameData,
  type: 'zetpkg'
}, {
  onProgress: (progress) => updateProgressBar(progress),
  onLoaded: () => console.log('Ready!'),
  onError: (error) => showError(error)
});

// 게임 루프
let lastTime = Date.now();
function gameLoop() {
  const now = Date.now();
  const deltaTime = now - lastTime;
  lastTime = now;
  
  engine.update(deltaTime);
  engine.render();
  
  requestAnimationFrame(gameLoop);
}

gameLoop();

// 정리
window.addEventListener('beforeunload', () => {
  engine.dispose();
});
```
