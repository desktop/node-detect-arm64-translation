import { isRunningUnderARM64Translation } from '../lib/detect-arm64-translation'

if (process.platform === 'win32') {
  describe('isRunningUnderARM64Translation', () => {
    it('does not crash', () => {
      console.log(`process.arch: ${process.arch}`)

      expect(isRunningUnderARM64Translation).not.toThrow()
    })
  })
}
