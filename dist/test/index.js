"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const detect_arm64_translation_1 = require("../lib/detect-arm64-translation");
if (process.platform === 'win32') {
    describe('isRunningUnderARM64Translation', () => {
        it('does not crash', () => {
            expect(detect_arm64_translation_1.isRunningUnderARM64Translation()).not.toThrow();
        });
    });
}
//# sourceMappingURL=index.js.map