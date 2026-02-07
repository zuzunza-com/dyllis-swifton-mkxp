/**
 * 코어 플레이어 엔진 공통 타입 정의
 * 
 * 모든 엔진이 공통으로 구현해야 하는 인터페이스를 정의합니다.
 */

/**
 * 엔진 설정
 */
export interface EngineConfig {
  /** 화면 너비 */
  width: number;
  /** 화면 높이 */
  height: number;
  /** 캔버스 요소 */
  canvas?: HTMLCanvasElement;
  /** 추가 옵션 */
  options?: Record<string, any>;
}

/**
 * 게임 데이터
 */
export interface GameData {
  /** 게임 파일 경로 또는 데이터 */
  data: string | ArrayBuffer | Blob;
  /** 게임 타입 */
  type: 'flash' | 'pixipkg' | 'rgss' | 'zetpkg';
  /** 메타데이터 */
  metadata?: Record<string, any>;
}

/**
 * 입력 이벤트
 */
export interface InputEvent {
  /** 이벤트 타입 */
  type: 'keydown' | 'keyup' | 'mousedown' | 'mouseup' | 'mousemove' | 'wheel';
  /** 키 코드 또는 마우스 버튼 */
  code: number;
  /** 좌표 (마우스 이벤트) */
  x?: number;
  y?: number;
  /** 휠 델타 (휠 이벤트) */
  delta?: number;
}

/**
 * 엔진 이벤트 콜백
 */
export interface EngineCallbacks {
  /** 로드 완료 */
  onLoaded?: () => void;
  /** 에러 발생 */
  onError?: (error: Error) => void;
  /** 진행률 업데이트 */
  onProgress?: (progress: number) => void;
}

/**
 * 코어 엔진 인터페이스
 * 
 * 모든 엔진이 구현해야 하는 공통 인터페이스입니다.
 */
export interface CoreEngine {
  /**
   * 엔진 초기화
   */
  init(config: EngineConfig): Promise<void>;

  /**
   * 엔진 종료
   */
  cleanup(): Promise<void>;

  /**
   * 게임 로드
   */
  loadGame(data: GameData, callbacks?: EngineCallbacks): Promise<void>;

  /**
   * 프레임 업데이트
   * @param deltaTime 이전 프레임부터 경과한 시간 (밀리초)
   */
  update(deltaTime: number): void;

  /**
   * 렌더링
   */
  render(): void;

  /**
   * 입력 처리
   */
  handleInput(event: InputEvent): void;

  /**
   * 리소스 해제
   */
  dispose(): void;

  /**
   * 엔진 상태 가져오기
   */
  getState(): EngineState;
}

/**
 * 엔진 상태
 */
export enum EngineState {
  /** 초기화되지 않음 */
  UNINITIALIZED = 'uninitialized',
  /** 초기화 중 */
  INITIALIZING = 'initializing',
  /** 준비됨 */
  READY = 'ready',
  /** 게임 로딩 중 */
  LOADING = 'loading',
  /** 게임 실행 중 */
  RUNNING = 'running',
  /** 일시정지 */
  PAUSED = 'paused',
  /** 에러 */
  ERROR = 'error',
}
