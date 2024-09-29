/// <reference types="vitest" />

import { execSync } from 'node:child_process'
import { resolve } from 'node:path'
import { defineConfig, type Plugin } from 'vite'
import type { LogitechEnvironment } from './lib/types/env'

import externals from 'rollup-plugin-node-externals'
import dts from 'vite-plugin-dts'

const { npm_package_version: PACKAGE_VERSION } = process.env

/**
 * Vite before build plugin
 * @param name  instance name
 * @param cb    callback
 * @returns     vite plugin
 */
export const before = (name: string, cb: () => any): Plugin => ({
  name: `before-${name}`,
  buildStart: cb,
})

/** Vite config
 * @see https://vitejs.dev/
 */
export default defineConfig({
  define: {
    PACKAGE_VERSION: JSON.stringify(PACKAGE_VERSION),
  } satisfies LogitechEnvironment,
  build: {
    lib: {
      entry: [
        resolve('./lib/index.ts'),
        resolve('./lib/gkey.ts'),
        resolve('./lib/lcd.ts'),
        resolve('./lib/led.ts'),
      ],
      formats: ['cjs', 'es'],
      name: 'logi-js',
    },
    rollupOptions: {
      external: ['bindings'],
    },
  },
  plugins: [
    before('cmake-js', () => void execSync('cmake-js build')),
    externals(),
    dts({
      copyDtsFiles: true,
    }),
  ],
  test: {
    include: ['./lib/test/*.test.ts'],
  },
})
