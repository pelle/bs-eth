open Formats;

let getBalance = (~account: address, ~from=Latest, ()) => {
  if (validateAddress(account)) {
   let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    Js.Promise.(
      JsonRpc.jsonRpcRequest("eth_getBalance", params)
      |>then_(result => Decode.amount(result) |> resolve)
    )
  } else Js.Promise.reject(InvalidAddress(account))
};

let getTransactionCount = (~account: address, ~from=Latest, ()) => {
  if (validateAddress(account)) {
   let params = [|Encode.address(account), Encode.blockOrTag(from)|];
    Js.Promise.(
      JsonRpc.jsonRpcRequest("eth_getTransactionCount", params)
      |>then_(result => Decode.nonce(result) |> resolve)
    )
  } else Js.Promise.reject(InvalidAddress(account))
};

let blockNumber = () => {
  Js.Promise.(
    JsonRpc.jsonRpcRequest("eth_blockNumber", [||])
    |>then_(result => Decode.block(result) |> resolve)
  )
};

let gasPrice = () => {
  Js.Promise.(
    JsonRpc.jsonRpcRequest("eth_gasPrice", [||])
    |>then_(result => Decode.amount(result) |> resolve)
  )
};

let call = (~tx: transaction, ~from=Latest, ()) => {
  let address = contract(tx);
  if (validateAddress(address)) {
   let params = [|Encode.transaction(tx), Encode.blockOrTag(from)|];
   JsonRpc.jsonRpcRequest("eth_call", params);
  } else Js.Promise.reject(InvalidAddress(address))
};


