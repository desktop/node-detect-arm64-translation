"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.isRunningUnderARM64Translation = void 0;
const nativeModule = process.platform === 'win32'
    ? require('../../build/Release/detect-arm64-translation.node')
    : null;
function isRunningUnderARM64Translation() {
    return nativeModule !== null
        ? nativeModule.isRunningUnderARM64Translation()
        : undefined;
}
exports.isRunningUnderARM64Translation = isRunningUnderARM64Translation;
//# sourceMappingURL=detect-arm64-translation.js.map