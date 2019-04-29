open Belt.Result;

let jsonRpcRequestPayload = (method, params) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "jsonrpc", Js.Json.string("2.0"));
  Js.Dict.set(payload, "id", Js.Json.number(1.0));
  Js.Dict.set(payload, "method", Js.Json.string(method));
  Js.Dict.set(payload, "params", Js.Json.array(params));
  payload;
};
type jsonRpcResult = Belt.Result.t(string, string);

exception JsonRpcError(string);

let handleResponse = json : jsonRpcResult => {
  open Json.Decode;
  let error = optional(at(["error", "message"], string), json);
  switch (error) {
  | Some(message) => Error(message)
  | None => Ok(field("result", string, json))
  };
};

let jsonRpcRequest = (method, params) =>
  /* Js.log(jsonRpcRequestPayload(method, params)); */
  ReFetch.fetchJson(
    "https://mainnet.infura.io/bs-eth",
    jsonRpcRequestPayload(method, params),
  )
  |> Repromise.Rejectable.map(response => {
    switch(response) {
    | Ok(json) => handleResponse(json)
    | Error(error) => Error(error)
    }
  });
