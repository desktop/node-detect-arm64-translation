const nativeModule =
  process.platform === 'win32'
    ? require('../../build/Release/detect-arm64-translation.node')
    : null

export function isRunningUnderARM64Translation(): boolean | undefined {
  return nativeModule !== null
    ? nativeModule.isRunningUnderARM64Translation()
    : undefined
}
