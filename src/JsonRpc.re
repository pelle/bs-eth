[%raw "require('isomorphic-fetch')"]

open Belt;

let jsonRpcRequestPayload = (method, params) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "jsonrpc", Js.Json.string("2.0"));
  Js.Dict.set(payload, "id", Js.Json.number(1.0));
  Js.Dict.set(payload, "method", Js.Json.string(method));
  Js.Dict.set(payload, "params", Js.Json.array(params));
  payload
};

type jsonRpcResult = Result.t(string, string);

exception JsonRpcError(string);

let handleResponse = (json) : jsonRpcResult => {
  open Json.Decode;
  let error = optional(at(["error", "message"], string), json);
  switch (error) {
  | Some(message) => Result.Error(message)
  | None => Result.Ok(field("result", string, json))
  }
};

let jsonRpcRequest = (method, params) => {
  Js.Promise.(
    Fetch.fetchWithInit(
      "https://mainnet.infura.io/re-eth",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(jsonRpcRequestPayload(method, params)))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        ()
      )
    )
    |> then_(Fetch.Response.json)
    |> then_((json) => {
      switch(handleResponse(json)) {
      | Result.Error(message) => reject(JsonRpcError(message))
      | Result.Ok(data) => resolve(data)
      }
    }
  )
  )
};
