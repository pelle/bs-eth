# Really Basic ReasonML web3 like interface

The goal is for this to support most common web3 functionality in ReasonML as well as some less common functionality that I need.

I am currently learning ReasonML and using this as an exercise at implementing something I understand fairly well (web3) using ReasonMl.

This means the API is likely going to change a lot as I learn more about idiomatic ReasonML and fix stupid beginners mistakes.

## Setting up a Provider

There is currently support for the following providers that both live in the `Providers` namespace:

- `Providers.web3(web3provider)` which wraps an existing web3 provider eg. MetaMask
- `Providers.http(url)` which is configured with a url to a http based provider like [infura](https://infura.io)

Once you have a provider you pass it as a parameter to all the functions in the `Eth` namespace.

```reasonml
let provider = Providers.http("https://mainnet.infura.io/bs-eth");
netVersion(provider)
|> Repromise.wait(result => switch (result) {
| Ok(netId) => Js.log("we are on the " ++ string_of_int(netId) ++ " chain")
| Error(msg) => Js.log(msg)
});
```

## Calling functions

All functions in the `Eth` module return typed [`Belt.Result`](https://bucklescript.github.io/bucklescript/api/Belt.Result.html) in a [Repromise](https://aantron.github.io/repromise/). So instead of looking out for rejected calls, do a pattern match.

See the [List of Methods](#ethereum-json-rpc-implemented-methods) implemented for more details.

## Roadmap

- [x] Fetch based https provider interface
- [x] Wrap javascript Web3 provider (a la metamask)
- [x] Most common JSON-RPC methods implemented (In bold below)
- [x] Basic Documentation
- [ ] Solidity ABI codec in native reasonml
- [ ] Add support for WebSocket provider
- [ ] Filter support with support for some sort of reactive streams. Maybe [wonka](https://github.com/kitten/wonka)
- [ ] Implement complete wrappers for the JSON-RPC apps methods listed below
- [ ] [EIP 712 support](https://eips.ethereum.org/EIPS/eip-712)
- [ ] Add RLP encoder to be able to encode transactions for signing
- [ ] Allow signing in ReasonML using WASM
- [ ] Support for bs-native

### Ethereum JSON-RPC Implemented Methods

The RPC methods currently implemented are:

- [x] **`eth_accounts` as `accounts()`**
- [x] **`eth_blockNumber` as `blockNumber()`**
- [x] **`eth_call` as `call()`**
- [x] **`eth_coinbase` as `coinbase()`**
- [x] **`eth_estimateGas` as `estimateGas()`**
- [x] **`eth_gasPrice` as `gasPrice()`**
- [x] **`eth_getBalance` as `balanceOf()`**
- [x] **`eth_getBlockByNumber` as `blockByNumber`**
- [x] **`eth_getBlockByHash` as `blockByHash`**
- [ ] `eth_getBlockTransactionCountByHash`
- [ ] `eth_getBlockTransactionCountByNumber`
- [ ] `eth_getCode`
- [ ] `eth_getCompilers`
- [ ] `eth_getFilterChanges`
- [ ] `eth_getFilterLogs`
- [ ] `eth_getLogs`
- [ ] `eth_getStorageAt`
- [x] **`eth_getTransactionByHash` as `transactionByHash()`**
- [ ] `eth_getTransactionByBlockHashAndIndex`
- [ ] `eth_getTransactionByBlockNumberAndIndex`
- [x] **`eth_getTransactionCount` as `transactionCount()`**
- [x] **`eth_getTransactionReceipt` as `transactionReceipt()**
- [ ] `eth_hashrate`
- [ ] `eth_mining`
- [ ] `eth_newBlockFilter`
- [ ] `eth_newFilter` (includes log/event filters)
- [ ] `eth_protocolVersion`
- [x] **`eth_sendTransaction` as `sendTransaction()`**
- [x] **`eth_sendRawTransaction` as `sendRawTransaction()`**
- [ ] `eth_subscribe` (only for websocket connections. "syncing" subscriptions are not yet supported)
- [ ] `eth_unsubscribe` (only for websocket connections. "syncing" subscriptions are not yet supported)
- [ ] `eth_syncing`
- [ ] `eth_uninstallFilter`
- [ ] `net_listening`
- [ ] `net_peerCount`
- [x] **`net_version` as `netVersion()`**

Non-Standard [ganache-cli](https://github.com/trufflesuite/ganache-cli) methods:

- [ ] `evm_snapshot`
- [ ] `evm_revert`
- [ ] `evm_increaseTime`
- [x] `evm_mine` as `mineBlock()`
