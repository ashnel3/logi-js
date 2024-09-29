import bindings from 'bindings'

export interface GKeySdk {}

export default bindings('libgkey.node') as GKeySdk
