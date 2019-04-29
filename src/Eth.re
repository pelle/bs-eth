open Formats;
open Belt.Result;

let getBalance = (~account: address, ~from=Latest, ()) => {
  if (validateAddress(account)) {
    let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    JsonRpc.jsonRpcRequest("eth_getBalance", params)
    |> Repromise.map(result => switch(result) {
      | Ok(data) => Ok(Decode.amount(data))
      | Error(msg) => Error(msg)
    })
  } else Repromise.resolved(Error("Invalid Address: " ++ account))
};

let getTransactionCount = (~account: address, ~from=Latest, ()) => {
  if (validateAddress(account)) {
   let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    JsonRpc.jsonRpcRequest("eth_getTransactionCount", params)
    |> Repromise.map(result => switch(result) {
      | Ok(data) => Ok(Decode.nonce(data))
      | Error(msg) => Error(msg)
    })
  } else Repromise.resolved(Error("Invalid Address: " ++ account))
};

let blockNumber = () => {
  JsonRpc.jsonRpcRequest("eth_blockNumber", [||])
  |> Repromise.map(result => switch(result) {
    | Ok(data) => Ok(Decode.block(data))
    | Error(msg) => Error(msg)
  })
};

let gasPrice = () => {
  JsonRpc.jsonRpcRequest("eth_gasPrice", [||])
  |> Repromise.map(result => switch(result) {
    | Ok(data) => Ok(Decode.amount(data))
    | Error(msg) => Error(msg)
  })
};

let call = (~tx: transaction, ~from=Latest, ()) => {
  let address = contract(tx);
  if (validateAddress(address)) {
   let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
   JsonRpc.jsonRpcRequest("eth_call", params);
  } else Repromise.Rejectable.rejected(InvalidAddress(address))
};


