import { isRunningUnderARM64Translation } from '../lib/detect-arm64-translation'

if (process.platform === 'win32') {
  describe('isRunningUnderARM64Translation', () => {
    it('does not crash', () => {
      expect(isRunningUnderARM64Translation()).not.toThrow()
    })
  })
}
