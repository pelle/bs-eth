type blockNumber = int;
type nonce = int;
type quantityResponse = string;
type blockOrTag =
  | Block(blockNumber)
  | Earliest
  | Latest
  | Pending;

type address = string;
type data = string;

type wei = Bn.t;
type gas = int;
type eth = Bn.t;

[@bs.deriving abstract]
type transaction = {
  [@bs.as "to"]
  contract: address,
  [@bs.optional]
  from: address,
  [@bs.optional]
  gas,
  [@bs.optional]
  gasPrice: wei,
  [@bs.optional]
  value: wei,
  [@bs.optional]
  data,
  [@bs.optional]
  nonce,
};

let strip0x = value => Js.String.replaceByRe([%bs.re "/^0x/"], "", value);
/* Js.String.sliceToEnd(value, ~from=2); */

let bnZero = Bn.fromFloat(0.0);

module Decode = {
  open Belt.Result;

  let quantity = result => switch (Js.Json.classify(result)) {
  | Js.Json.JSONNumber(value) => int_of_float(value)
  | Js.Json.JSONString(hex) => int_of_float(Bn.toNumber(Bn.fromString(~base=16, strip0x(hex))))
  | _ => 0
  };

  let nonce = quantity;
  let block = quantity;
  let amount = result => switch(Js.Json.classify(result)) {
    | Js.Json.JSONNumber(number) => Bn.fromFloat(number)
    | Js.Json.JSONString(hex) => Bn.fromString(~base=16, strip0x(hex))
    | _ => bnZero
  };
  let address = result : address => switch (Js.Json.decodeString(result)) {
    | Some(address) => address
    | None => ""
    };
  let accounts = result => switch (Js.Json.classify(result)) {
    | Js.Json.JSONArray(addresses) => Belt.Array.map(addresses, address)
    | _ => [||]
    };
  let string = result => switch (Js.Json.decodeString(result)) {
    | Some(data) => data
    | None => ""
    };
  let data = string
};

let addressMatcher = [%bs.re "/^0x[0-9a-fA-F]{40}$/"];

let validateAddress = value => Js.Re.test_(addressMatcher, value);

let toHex = amount => Js.Json.string(Printf.sprintf("0x%x", amount));

let toDict = [%raw obj => {| return obj |}];

module Encode = {
  let address = addr => Js.Json.string(addr);
  let block = (blk: blockNumber) => toHex(blk);
  let transaction = (tx: transaction) => Js.Json.object_(toDict(tx));

  let blockOrTag = value =>
    switch (value) {
    | Block(amount) => Js.Json.string(Printf.sprintf("0x%x", amount))
    | Latest => Js.Json.string("latest")
    | Pending => Js.Json.string("pending")
    | Earliest => Js.Json.string("earliest")
    };
};

/*
 curl -X POST --data '{"jsonrpc":"2.0","method":"eth_call","params":[{"to":"0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c", "data":"0x06fdde03"],"id":1}' https://mainnet.infura.io/re-eth
 */