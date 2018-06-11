
let getBalance = (address) => {  
  let params = [|Formats.encode(Formats.Address(address))|];
  Js.Promise.(
    JsonRpc.jsonRpcRequest("eth_getBalance", params)
    |>then_(result => Formats.Decode.quantity(result) |> resolve)
  )
};
