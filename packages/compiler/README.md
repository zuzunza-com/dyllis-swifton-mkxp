# @alibrary-illusion/compiler

ZetPkg 컴파일러 패키지 - 소스 파일들을 ZetPkg 형식으로 패키징합니다.

## 설치

```bash
pnpm add @alibrary-illusion/compiler
```

## 사용법

```typescript
import { ZetPkgBuilder, type FileNode, type BuildOptions } from '@alibrary-illusion/compiler';

const files: FileNode[] = [
  { path: 'main.js', content: Buffer.from('console.log("Hello");') },
  { path: 'config.json', content: Buffer.from('{"version": "1.0.0"}') }
];

const options: BuildOptions = {
  entry: 'main.js',
  version: '1.0.0'
};

const result = await ZetPkgBuilder.build(files, options);
if (result.success) {
  console.log('빌드 성공:', result.hash);
  console.log('패키지 내용:', result.content);
}
```

## API

### ZetPkgBuilder

#### `build(files: FileNode[], options: BuildOptions): Promise<BuildResult>`

파일 트리를 ZetPkg 형식으로 빌드합니다.

#### `validate(content: string): Promise<{ valid: boolean; error?: string }>`

빌드된 패키지의 유효성을 검증합니다.

## 타입

- `FileNode`: 파일 노드 인터페이스
- `BuildOptions`: 빌드 옵션
- `BuildResult`: 빌드 결과

## 의존성

- `@alibrary-illusion/parser`: ZetPkg 파서 (내부 패키지)
