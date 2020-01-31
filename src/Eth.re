open Formats;
open Belt.Result;

let coinbase = (provider: Providers.provider) =>
  provider("eth_coinbase", [||])->Promise.mapOk(Decode.address);

let accounts = (provider: Providers.provider) =>
  provider("eth_accounts", [||])->Promise.mapOk(Decode.accounts);

let balanceOf =
    (~provider: Providers.provider, ~account: address, ~from=Latest, ()) =>
  if (validateAddress(account)) {
    let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    provider("eth_getBalance", params)->Promise.mapOk(Decode.amount);
  } else {
    Promise.resolved(Error("Invalid Address: " ++ account));
  };

let transactionCount =
    (~provider: Providers.provider, ~account: address, ~from=Latest, ()) =>
  if (validateAddress(account)) {
    let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    provider("eth_getTransactionCount", params)->Promise.mapOk(Decode.nonce);
  } else {
    Promise.resolved(Error("Invalid Address: " ++ account));
  };

let blockNumber = (provider: Providers.provider) =>
  provider("eth_blockNumber", [||])->Promise.mapOk(Decode.blockNumber);

let gasPrice = (provider: Providers.provider) =>
  provider("eth_gasPrice", [||])->Promise.mapOk(Decode.amount);

let sendTransaction = (~provider: Providers.provider, ~tx) => {
  let params = [|Encode.transaction(tx)|];
  provider("eth_sendTransaction", params)->Promise.mapOk(Decode.data);
};

let sendRawTransaction = (~provider: Providers.provider, ~tx: Formats.data) => {
  let params = [|Encode.data(tx)|];
  provider("eth_sendRawTransaction", params)->Promise.mapOk(Decode.data);
};

let transactionByHash =
    (~provider: Providers.provider, ~txHash: Formats.txHash) =>
  provider("eth_getTransactionByHash", [|Encode.data(txHash)|])
  ->Promise.mapOk(Decode.transaction);

let transactionReceipt =
    (~provider: Providers.provider, ~txHash: Formats.txHash) =>
  provider("eth_getTransactionReceipt", [|Encode.data(txHash)|])
  ->Promise.mapOk(Decode.receipt);

let estimateGas = (~provider: Providers.provider, ~tx, ~from=Latest, ()) => {
  let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
  provider("eth_estimateGas", params)->Promise.mapOk(Decode.amount);
};

let call = (~provider: Providers.provider, ~tx, ~from=Latest, ()) => {
  let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
  provider("eth_call", params)->Promise.mapOk(Decode.data);
};

let mineBlock = (provider: Providers.provider) =>
  provider("evm_mine", [||])->Promise.mapOk(Decode.string);

let blockByHash =
    (
      ~provider: Providers.provider,
      ~blockHash: Formats.blockHash,
      ~deep: bool,
    ) =>
  provider(
    "eth_getBlockByHash",
    [|Encode.data(blockHash), Encode.bool(deep)|],
  )
  ->Promise.mapOk(Decode.block);

let blockByNumber =
    (
      ~provider: Providers.provider,
      ~blockNumber: Formats.blockNumber,
      ~deep: bool,
    ) =>
  provider(
    "eth_getBlockByNumber",
    [|Encode.quantity(blockNumber), Encode.bool(deep)|],
  )
  ->Promise.mapOk(Decode.block);

let netVersion = (provider: Providers.provider) =>
  provider("net_version", [||])->Promise.mapOk(Decode.quantity);
