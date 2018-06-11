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

let blockNumber = () => {
  Js.Promise.(
    JsonRpc.jsonRpcRequest("eth_blockNumber", [||])
    |>then_(result => Decode.block(result) |> resolve)
  )
};


