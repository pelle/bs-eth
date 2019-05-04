# Really Basic ReasonML web3 like interface

The goal is for this to support most common web3 functionality in ReasonML as well as some less common functionality that I need.

I am currently learning ReasonML and using this as an exercise at implementing something I understand fairly well (web3) using ReasonMl.

This means the API is likely going to change a lot as I learn more about idiomatic ReasonML and fix stupid beginners mistakes.

## Roadmap

- [x] Fetch based https provider interface
- [x] Wrap javascript Web3 provider (a la metamask)
- [ ] Basic Documentation
- [ ] Most common JSON-RPC methods implemented (In bold below)
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
- [ ] **`eth_getBlockByNumber`**
- [ ] **`eth_getBlockByHash`**
- [ ] `eth_getBlockTransactionCountByHash`
- [ ] `eth_getBlockTransactionCountByNumber`
- [ ] `eth_getCode` (only supports block number “latest”)
- [ ] `eth_getCompilers`
- [ ] `eth_getFilterChanges`
- [ ] `eth_getFilterLogs`
- [ ] `eth_getLogs`
- [ ] `eth_getStorageAt`
- [x] **`eth_getTransactionByHash` as `transactionByHash()`**
- [ ] `eth_getTransactionByBlockHashAndIndex`
- [ ] `eth_getTransactionByBlockNumberAndIndex`
- [ ] `eth_getTransactionCount`
- [ ] **`eth_getTransactionReceipt`**
- [ ] `eth_hashrate`
- [ ] `eth_mining`
- [ ] `eth_newBlockFilter`
- [ ] `eth_newFilter` (includes log/event filters)
- [ ] `eth_protocolVersion`
- [x] `eth_sendTransaction` as `sendTransaction()`
- [ ] **`eth_sendRawTransaction`**
- [ ] `eth_sign`
- [ ] `eth_subscribe` (only for websocket connections. "syncing" subscriptions are not yet supported)
- [ ] `eth_unsubscribe` (only for websocket connections. "syncing" subscriptions are not yet supported)
- [ ] `eth_syncing`
- [ ] `eth_uninstallFilter`
- [ ] `net_listening`
- [ ] `net_peerCount`
- [ ] `net_version`
- [ ] `miner_start`
- [ ] `miner_stop`
- [ ] `rpc_modules`
- [ ] **`web3_clientVersion`**

Non-Standard [ganache-cli](https://github.com/trufflesuite/ganache-cli) methods:

- [ ] `evm_snapshot`
- [ ] `evm_revert`
- [ ] `evm_increaseTime`
- [x] `evm_mine` as `mineBlock()`
