/**
 * ZetPkg Runtime Loader
 * 
 * 기존 PixiPkg의 후속 규격인 ZetPkg를 로드하는 클래스입니다.
 * PixiPkg와의 호환성을 유지하면서 확장된 기능을 제공합니다.
 */

import crypto from 'crypto';

// Base6x encoding (JSON-safe, URL-safe)
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

  static decode(str: string): Buffer {
    const charMap = new Map(
      this.CHARSET.split('').map((c, i) => [c, i])
    );

    const bytes: number[] = [];
    let bits = 0;
    let value = 0;

    for (const char of str) {
      const charValue = charMap.get(char);
      if (charValue === undefined) continue;

      value = (value << 6) | charValue;
      bits += 6;

      if (bits >= 8) {
        bits -= 8;
        bytes.push((value >> bits) & 0xff);
      }
    }

    return Buffer.from(bytes);
  }
}

export interface ZetPkgMetadata {
  format: string; // 'ZetPkg' 또는 'PixiPkg' (호환성)
  version: string;
  entry: string;
  created: string;
  encryption: string;
  assets: {
    [filename: string]: string; // filename -> hash
  };
}

export interface ZetPkgPayload {
  [filename: string]: string; // filename -> encrypted content
}

export class ZetPkgLoader {
  /**
   * Parse and validate ZetPkg file
   * 
   * PixiPkg 형식도 호환성을 위해 지원합니다.
   */
  static parse(content: string): { metadata: ZetPkgMetadata; payload: ZetPkgPayload } {
    // Step 1: Split metadata and payload
    const parts = content.split('\n---PAYLOAD---\n');
    if (parts.length !== 2) {
      throw new Error('Invalid ZetPkg format: missing payload separator');
    }

    // Step 2: Parse metadata header
    let metadata: ZetPkgMetadata;
    try {
      metadata = JSON.parse(parts[0]);
    } catch (err) {
      throw new Error('Invalid ZetPkg: metadata header is not valid JSON');
    }

    // Step 3: Validate metadata (ZetPkg 또는 PixiPkg 허용)
    if (metadata.format !== 'ZetPkg' && metadata.format !== 'PixiPkg') {
      throw new Error(`Invalid format: expected "ZetPkg" or "PixiPkg", got "${metadata.format}"`);
    }

    if (metadata.encryption !== 'base6x-json-safe') {
      throw new Error(`Unsupported encryption: ${metadata.encryption}`);
    }

    // Step 4: Parse payload
    let payload: ZetPkgPayload;
    try {
      payload = JSON.parse(parts[1]);
    } catch (err) {
      throw new Error('Invalid ZetPkg: payload is not valid JSON');
    }

    return { metadata, payload };
  }

  /**
   * Decrypt and verify asset
   */
  static decryptAsset(
    encrypted: string,
    expectedHash: string,
    filename: string
  ): Buffer {
    // Step 1: Decrypt
    const decrypted = Base6x.decode(encrypted);

    // Step 2: Calculate hash
    const hash = crypto.createHash('sha256').update(decrypted).digest();
    const hashB6x = Base6x.encode(hash);

    // Step 3: Verify
    if (hashB6x !== expectedHash) {
      throw new Error(
        `Hash mismatch for ${filename}: expected ${expectedHash}, got ${hashB6x}`
      );
    }

    return decrypted;
  }

  /**
   * Load and verify entire ZetPkg
   */
  static async load(content: string): Promise<Map<string, Buffer>> {
    console.log('📦 Loading ZetPkg...');

    // Parse
    const { metadata, payload } = this.parse(content);
    const formatName = metadata.format === 'PixiPkg' ? 'PixiPkg (legacy)' : 'ZetPkg';
    console.log(`✅ Metadata validated (format: ${formatName} v${metadata.version})`);

    // Decrypt and verify all assets
    const assets = new Map<string, Buffer>();
    const filenames = Object.keys(metadata.assets);

    console.log(`🔐 Decrypting ${filenames.length} assets...`);

    for (const filename of filenames) {
      const encrypted = payload[filename];
      if (!encrypted) {
        throw new Error(`Missing asset: ${filename}`);
      }

      const expectedHash = metadata.assets[filename];
      const decrypted = this.decryptAsset(encrypted, expectedHash, filename);
      assets.set(filename, decrypted);

      console.log(`  ✓ ${filename} (${(decrypted.length / 1024).toFixed(2)} KB)`);
    }

    console.log('✅ All assets loaded and verified\n');

    return assets;
  }

  /**
   * Calculate package hash
   */
  static calculateHash(content: string): string {
    return crypto.createHash('sha256').update(content).digest('hex');
  }
}

export default ZetPkgLoader;
