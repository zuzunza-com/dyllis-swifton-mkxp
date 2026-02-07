/**
 * ZetPkg 빌더
 * 
 * 소스 파일들을 ZetPkg 형식으로 패키징합니다.
 */

import crypto from 'crypto';
import { ZetPkgLoader } from '../../parser/zetpkg/loader';

// Base6x encoding (ZetPkgLoader와 동일한 구현)
class Base6x {
  private static readonly CHARSET = 
    'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_';

  static encode(buffer: Buffer): string {
    const bytes = new Uint8Array(buffer);
    let result = '';
    let bits = 0;
    let value = 0;

    for (const byte of bytes) {
      value = (value << 8) | byte;
      bits += 8;

      while (bits >= 6) {
        bits -= 6;
        result += this.CHARSET[(value >> bits) & 0x3f];
      }
    }

    if (bits > 0) {
      result += this.CHARSET[(value << (6 - bits)) & 0x3f];
    }

    return result;
  }
}

export interface FileNode {
  path: string;
  content: Buffer | string;
  type?: 'file' | 'directory';
}

export interface BuildOptions {
  entry: string;
  version?: string;
  metadata?: Record<string, any>;
}

export interface BuildResult {
  success: boolean;
  error?: string;
  content?: string;
  hash?: string;
  size?: number;
}

export class ZetPkgBuilder {
  /**
   * 파일 트리를 ZetPkg 형식으로 빌드
   */
  static async build(
    files: FileNode[],
    options: BuildOptions
  ): Promise<BuildResult> {
    try {
      // 1. 파일 검증
      const entryFile = files.find(f => f.path === options.entry);
      if (!entryFile) {
        return {
          success: false,
          error: `Entry file not found: ${options.entry}`,
        };
      }

      // 2. 메타데이터 생성
      const metadata = {
        format: 'ZetPkg',
        version: options.version || '1.0.0',
        entry: options.entry,
        created: new Date().toISOString(),
        encryption: 'base6x-json-safe',
        assets: {} as Record<string, string>,
        ...options.metadata,
      };

      // 3. 에셋 암호화 및 해시 계산
      const payload: Record<string, string> = {};
      
      for (const file of files) {
        if (file.type === 'directory') continue;

        const content = typeof file.content === 'string' 
          ? Buffer.from(file.content, 'utf-8')
          : file.content;

        // 해시 계산
        const hash = crypto.createHash('sha256').update(content).digest();
        const hashB6x = Base6x.encode(hash);
        metadata.assets[file.path] = hashB6x;

        // 암호화 (Base6x 인코딩)
        const encrypted = Base6x.encode(content);
        payload[file.path] = encrypted;
      }

      // 4. 패키지 조립
      const metadataJson = JSON.stringify(metadata, null, 2);
      const payloadJson = JSON.stringify(payload);
      const content = `${metadataJson}\n---PAYLOAD---\n${payloadJson}`;

      // 5. 패키지 해시 계산
      const hash = ZetPkgLoader.calculateHash(content);

      return {
        success: true,
        content,
        hash,
        size: content.length,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : 'Unknown error',
      };
    }
  }

  /**
   * 빌드 검증
   */
  static async validate(content: string): Promise<{ valid: boolean; error?: string }> {
    try {
      await ZetPkgLoader.load(content);
      return { valid: true };
    } catch (error) {
      return {
        valid: false,
        error: error instanceof Error ? error.message : 'Unknown error',
      };
    }
  }
}
