# @alibrary-illusion/compiler-zetpkg

ZetPkg 전용 빌더 패키지 - ZetPkg 형식으로 패키징하는 전용 빌더입니다.

## 설치

```bash
pnpm add @alibrary-illusion/compiler-zetpkg
```

## 사용법

```typescript
import { ZetPkgBuilder, type FileNode, type BuildOptions } from '@alibrary-illusion/compiler-zetpkg';

const files: FileNode[] = [
  { path: 'main.js', content: Buffer.from('console.log("Hello");') },
  { path: 'assets/image.png', content: fs.readFileSync('image.png') }
];

const options: BuildOptions = {
  entry: 'main.js',
  version: '1.0.0',
  metadata: {
    author: 'Your Name',
    description: 'My Game'
  }
};

const result = await ZetPkgBuilder.build(files, options);
if (result.success) {
  // 빌드된 패키지 저장
  fs.writeFileSync('game.zetpkg', result.content!);
  console.log('패키지 해시:', result.hash);
}
```

## API

### ZetPkgBuilder

#### `build(files: FileNode[], options: BuildOptions): Promise<BuildResult>`

파일 트리를 ZetPkg 형식으로 빌드합니다.

- `files`: 패키징할 파일 목록
- `options.entry`: 진입점 파일 경로
- `options.version`: 패키지 버전 (기본값: "1.0.0")
- `options.metadata`: 추가 메타데이터

#### `validate(content: string): Promise<{ valid: boolean; error?: string }>`

빌드된 패키지의 유효성을 검증합니다.

## 타입

- `FileNode`: 파일 노드 인터페이스 (path, content, type)
- `BuildOptions`: 빌드 옵션 (entry, version, metadata)
- `BuildResult`: 빌드 결과 (success, error, content, hash, size)

## 의존성

- `@alibrary-illusion/parser`: ZetPkg 파서 (내부 패키지)

## 참고

이 패키지는 `@alibrary-illusion/compiler`의 하위 모듈로, ZetPkg 빌더만을 독립적으로 사용하고 싶을 때 사용합니다.
