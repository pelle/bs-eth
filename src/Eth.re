open Formats;
open Belt.Result;

let getCoinbase = (provider: JsonRpc.provider) =>
  provider("eth_coinbase", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.address(data))
       | Error(msg) => Error(msg)
       }
     );

let getAccounts = (provider: JsonRpc.provider) =>
  provider("eth_accounts", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.accounts(data))
       | Error(msg) => Error(msg)
       }
     );

let getBalance =
    (~provider: JsonRpc.provider, ~account: address, ~from=Latest, ()) =>
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
    (~provider: JsonRpc.provider, ~account: address, ~from=Latest, ()) =>
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

let blockNumber = (provider: JsonRpc.provider) =>
  provider("eth_blockNumber", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.block(data))
       | Error(msg) => Error(msg)
       }
     );

let gasPrice = (provider: JsonRpc.provider) =>
  provider("eth_gasPrice", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.amount(data))
       | Error(msg) => Error(msg)
       }
     );

let sendTransaction = (~provider: JsonRpc.provider, ~tx, ()) => {
  let params = [|Encode.transaction(tx)|];
  provider("eth_sendTransaction", params)
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.data(data))
       | Error(msg) => Error(msg)
       }
     );
};

let call = (~provider: JsonRpc.provider, ~tx, ~from=Latest, ()) => {
  let address = recipientGet(tx);
  if (validateAddress(address)) {
    let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
    provider("eth_call", params)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(data) => Ok(Decode.data(data))
         | Error(msg) => Error(msg)
         }
       );
  } else {
    Repromise.resolved(Error("Invalid Address: " ++ address));
  };
};

let newBlock = (provider: JsonRpc.provider) =>
  provider("evm_mine", [||])
  |> Repromise.map(result =>
       switch (result) {
       | Ok(data) => Ok(Decode.string(data))
       | Error(msg) => Error(msg)
       }
     );
