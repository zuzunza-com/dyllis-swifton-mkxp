/**
 * ZetPkg 파서 메인 익스포트
 * 
 * 기존 PixiPkg에서 ZetPkg로 이름이 변경되었습니다.
 */

export { ZetPkgLoader, type ZetPkgMetadata, type ZetPkgPayload } from './zetpkg/loader';
export { detectFromContent, validateZetPkg, type ClipType, type ClipInfo } from './zetpkg/detector';

// 호환성을 위한 별칭 (deprecated)
/** @deprecated Use ZetPkgLoader instead */
export { ZetPkgLoader as PixiPkgLoader } from './zetpkg/loader';
/** @deprecated Use ZetPkgMetadata instead */
export type { ZetPkgMetadata as PixiPkgMetadata } from './zetpkg/loader';
/** @deprecated Use ZetPkgPayload instead */
export type { ZetPkgPayload as PixiPkgPayload } from './zetpkg/loader';
