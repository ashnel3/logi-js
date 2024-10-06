import bindings from 'bindings'
import type { Box } from './types'

const addon: LcdSdk = bindings('liblcd.node')

export interface LcdSdkGroup {
  readonly isConnected: () => boolean
  readonly setText: (line: number, text: string) => boolean
  readonly size: Box
  readonly type: number
}

export interface LcdSdk {
  readonly TYPE_COLOR: number
  readonly TYPE_MONO: number
  readonly color: LcdSdkGroup
  readonly mono: LcdSdkGroup

  readonly isConnected: (type: number) => boolean
  readonly init: (name: string, type: number) => boolean
  readonly update: () => void
  readonly shutdown: () => void
}

export default addon
