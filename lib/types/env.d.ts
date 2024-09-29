import 'vite/client'

export interface LogitechEnvironment {
  readonly PACKAGE_VERSION: string
}

declare global {
  const PACKAGE_VERSION: LogitechEnvironment['PACKAGE_VERSION']
}
