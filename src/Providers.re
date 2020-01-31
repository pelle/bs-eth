open Belt.Result;

type requestPayload = Js.Dict.t(Js.Json.t);

let toRequest = (method, params): requestPayload => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "jsonrpc", Js.Json.string("2.0"));
  Js.Dict.set(payload, "id", Js.Json.number(1.0));
  Js.Dict.set(payload, "method", Js.Json.string(method));
  Js.Dict.set(payload, "params", Js.Json.array(params));
  payload;
};

type rpcResult = Belt.Result.t(Js.Json.t, string);

exception JsonRpcError(string);

let decodeRpcResult = json =>
  switch (Js.Json.classify(json)) {
  | Js.Json.JSONObject(value) =>
    switch (Js.Dict.get(value, "result")) {
    | Some(result) => Ok(result)
    | None => Error("No result")
    }
  | _ => Error("Invalid response")
  };

let handleResponse = (json): rpcResult => {
  open Json.Decode;
  let error = optional(at(["error", "message"], string), json);
  switch (error) {
  | Some(message) => Error(message)
  | None => decodeRpcResult(json)
  };
};

type provider = (string, Js.Array.t(Js.Json.t)) => Promise.t(rpcResult);

let http = (url): provider =>
  (method, params) =>
    ReFetch.fetchJson(url, toRequest(method, params))
    ->Promise.map(response =>
        switch (response) {
        | Ok(json) => handleResponse(json)
        | Error(error) => Error(error)
        }
      );

module Web3provider = {
  type t;
  type callback = (Js.Nullable.t(string), Js.Nullable.t(Js.Json.t)) => unit;
  [@bs.send] external sendAsync: (t, requestPayload, callback) => unit = "";
};

let web3 = (web3p: Web3provider.t, method, params) => {
  let (p, resolve_p) = Promise.pending();
  Web3provider.sendAsync(web3p, toRequest(method, params), (error, response) =>
    resolve_p(
      switch (Js.Nullable.toOption(error), Js.Nullable.toOption(response)) {
      | (_, Some(result)) => handleResponse(result)
      | (Some(msg), _) => Error(msg)
      | (_, _) => Error("Something went wrong")
      },
    )
  );
  p;
};
