/**
 * ZetPkg 포맷 감지기
 * 
 * 파일 내용을 분석하여 ZetPkg 또는 PixiPkg 형식인지 감지합니다.
 */

export type ClipType = 'zetpkg' | 'pixipkg' | 'swf' | 'video' | 'unknown';

export interface ClipInfo {
  type: ClipType;
  mimeType: string;
  isExecutable: boolean;
}

/**
 * Detect from content (magic bytes / signature)
 */
export function detectFromContent(content: Buffer | string): ClipInfo {
  const str = typeof content === 'string' ? content : content.toString('utf-8', 0, 200);

  // Check for ZetPkg JSON header
  if (str.includes('"format":"ZetPkg"') || str.includes('"format": "ZetPkg"')) {
    return {
      type: 'zetpkg',
      mimeType: 'application/x-zetpkg',
      isExecutable: true,
    };
  }

  // Check for PixiPkg JSON header (legacy compatibility)
  if (str.includes('"format":"PixiPkg"') || str.includes('"format": "PixiPkg"')) {
    return {
      type: 'pixipkg',
      mimeType: 'application/x-pixipkg',
      isExecutable: true,
    };
  }

  // Check for SWF magic bytes (FWS or CWS or ZWS)
  if (typeof content !== 'string') {
    const magic = content.toString('utf-8', 0, 3);
    if (magic === 'FWS' || magic === 'CWS' || magic === 'ZWS') {
      return {
        type: 'swf',
        mimeType: 'application/x-shockwave-flash',
        isExecutable: true,
      };
    }

    // Check for video containers (ftyp for MP4, webm signature)
    const hex = content.toString('hex', 0, 12);
    if (hex.includes('66747970')) { // 'ftyp' in hex
      return {
        type: 'video',
        mimeType: 'video/mp4',
        isExecutable: false,
      };
    }
    if (hex.startsWith('1a45dfa3')) { // WebM/Matroska signature
      return {
        type: 'video',
        mimeType: 'video/webm',
        isExecutable: false,
      };
    }
  }

  return {
    type: 'unknown',
    mimeType: 'application/octet-stream',
    isExecutable: false,
  };
}

/**
 * Validate ZetPkg format
 */
export function validateZetPkg(content: string): { valid: boolean; error?: string } {
  try {
    // Check for JSON header
    const headerEnd = content.indexOf('\n---PAYLOAD---\n');
    if (headerEnd === -1) {
      return { valid: false, error: 'Missing payload separator' };
    }

    const headerJson = content.substring(0, headerEnd);
    const metadata = JSON.parse(headerJson);

    // Validate required fields (ZetPkg 또는 PixiPkg 허용)
    if (metadata.format !== 'ZetPkg' && metadata.format !== 'PixiPkg') {
      return { valid: false, error: 'Invalid format field' };
    }

    if (!metadata.version) {
      return { valid: false, error: 'Missing version field' };
    }

    if (metadata.encryption !== 'base6x-json-safe') {
      return { valid: false, error: 'Unsupported encryption' };
    }

    if (!metadata.assets || typeof metadata.assets !== 'object') {
      return { valid: false, error: 'Missing or invalid assets field' };
    }

    return { valid: true };
  } catch (err) {
    return { valid: false, error: `Parse error: ${err}` };
  }
}
