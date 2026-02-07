# 코어 플레이어 엔진 생명주기 명세

## 개요

엔진의 생명주기와 상태 전이를 정의합니다.

## 상태

### UNINITIALIZED
엔진이 초기화되지 않은 상태입니다.

### INITIALIZING
엔진이 초기화 중인 상태입니다.

### READY
엔진이 준비되었고 게임을 로드할 수 있는 상태입니다.

### LOADING
게임이 로딩 중인 상태입니다.

### RUNNING
게임이 실행 중인 상태입니다.

### PAUSED
게임이 일시정지된 상태입니다.

### ERROR
에러가 발생한 상태입니다.

## 상태 전이

```
1. UNINITIALIZED
   └─> init() 호출
       └─> INITIALIZING
           └─> 초기화 완료
               └─> READY

2. READY
   └─> loadGame() 호출
       └─> LOADING
           └─> 로드 완료
               └─> RUNNING

3. RUNNING
   ├─> pause() 호출 -> PAUSED
   ├─> 에러 발생 -> ERROR
   └─> cleanup() 호출 -> UNINITIALIZED

4. PAUSED
   ├─> resume() 호출 -> RUNNING
   └─> cleanup() 호출 -> UNINITIALIZED

5. ERROR
   └─> cleanup() 호출 -> UNINITIALIZED
```

## 생명주기 이벤트

### 초기화
1. `init()` 호출
2. 리소스 할당
3. 상태를 READY로 변경

### 게임 로드
1. `loadGame()` 호출
2. 상태를 LOADING으로 변경
3. 게임 데이터 파싱
4. 리소스 로드
5. 상태를 RUNNING으로 변경

### 실행
1. `update()` 반복 호출
2. `render()` 반복 호출
3. 입력 처리

### 정리
1. `cleanup()` 또는 `dispose()` 호출
2. 리소스 해제
3. 상태를 UNINITIALIZED로 변경
