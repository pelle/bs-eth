open Formats;
open Belt.Result;

let coinbase = (provider: Providers.provider) =>
  provider("eth_coinbase", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.address(data))
       | Error(msg) => Error(msg)
       }
     );

let accounts = (provider: Providers.provider) =>
  provider("eth_accounts", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.accounts(data))
       | Error(msg) => Error(msg)
       }
     );

let balanceOf =
    (~provider: Providers.provider, ~account: address, ~from=Latest, ()) =>
  if (validateAddress(account)) {
    let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    provider("eth_getBalance", params)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(data) => Ok(Decode.amount(data))
         | Error(msg) => Error(msg)
         }
       );
  } else {
    Repromise.resolved(Error("Invalid Address: " ++ account));
  };

let getTransactionCount =
    (~provider: Providers.provider, ~account: address, ~from=Latest, ()) =>
  if (validateAddress(account)) {
    let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    provider("eth_getTransactionCount", params)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(data) => Ok(Decode.nonce(data))
         | Error(msg) => Error(msg)
         }
       );
  } else {
    Repromise.resolved(Error("Invalid Address: " ++ account));
  };

let blockNumber = (provider: Providers.provider) =>
  provider("eth_blockNumber", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.blockNumber(data))
       | Error(msg) => Error(msg)
       }
     );

let gasPrice = (provider: Providers.provider) =>
  provider("eth_gasPrice", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.amount(data))
       | Error(msg) => Error(msg)
       }
     );

let sendTransaction = (~provider: Providers.provider, ~tx, ()) => {
  let params = [|Encode.transaction(tx)|];
  provider("eth_sendTransaction", params)
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.data(data))
       | Error(msg) => Error(msg)
       }
     );
};

let transactionByHash =
    (~provider: Providers.provider, ~txHash: Formats.txHash) =>
  provider("eth_getTransactionByHash", [|Encode.data(txHash)|])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.transaction(data))
       | Error(msg) => Error(msg)
       }
     );

let estimateGas = (~provider: Providers.provider, ~tx, ~from=Latest, ()) => {
  let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
  provider("eth_estimateGas", params)
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.amount(data))
       | Error(msg) => Error(msg)
       }
     );
};

let call = (~provider: Providers.provider, ~tx, ~from=Latest, ()) => {
  let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
  provider("eth_call", params)
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.data(data))
       | Error(msg) => Error(msg)
       }
     );
};

let mineBlock = (provider: Providers.provider) =>
  provider("evm_mine", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.string(data))
       | Error(msg) => Error(msg)
       }
     );

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
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.block(data))
       | Error(msg) => Error(msg)
       }
     );

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
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.block(data))
       | Error(msg) => Error(msg)
       }
     );
