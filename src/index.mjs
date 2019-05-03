import * as Eth from './Eth.bs'
import { bnZero, Decode, Encode } from './Formats.bs'
import { wrapProvider, fetchProvider } from './JsonRpc.bs'

const JsonRpc = {
  wrapProvider,
  fetchProvider
}
const Formats = {
  bnZero,
  Decode,
  Encode
}

export { Eth, JsonRpc, Formats }
