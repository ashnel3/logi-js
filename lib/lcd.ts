import bindings from 'bindings'
import type { Box } from './types'

export interface LcdSdkSizes {
  readonly color: Box
  readonly mono: Box
}

export interface LcdSdkTypes {
  readonly color: number
  readonly mono: number
}

export interface LcdSdk {
  readonly sizes: LcdSdkSizes
  readonly types: LcdSdkTypes
  readonly LcdIsConnected: (type: number) => boolean
  readonly LcdInit: (name: string, type: number) => boolean
  readonly LcdUpdate: () => void
  readonly LcdShutdown: () => void
}

export default bindings('liblcd.node') as LcdSdk
