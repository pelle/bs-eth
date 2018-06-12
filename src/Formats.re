module BN = {
  type t;
  [@bs.new] [@bs.module] external create : (string, int) => t = "bn.js";
  [@bs.send] external toString : (t, int) => string = "";
  [@bs.send] external toNumber : (t) => int = "";
}

type blockNumber = int;
type nonce = int;
type quantityResponse = string;
type blockOrTag = Block(blockNumber) | Earliest | Latest | Pending ;

type address = string;
type data = string;

type wei = BN.t;
type gas = int;
type eth = BN.t;

[@bs.deriving abstract]
type transaction = {
  [@bs.as "to"] contract: address,
  [@bs.optional] from: address,
  [@bs.optional] gas: gas,
  [@bs.optional] gasPrice: wei,
  [@bs.optional] value: wei,
  [@bs.optional] data: data,
  [@bs.optional] nonce: nonce
};

let strip0x = value => Js.String.replaceByRe([%bs.re "/^0x/"], "", value);
/* Js.String.sliceToEnd(value, ~from=2); */

module Decode = {
  let quantity = result => int_of_string(result);
  let nonce = (result): nonce => quantity(result);
  let block = (result): blockNumber => quantity(result);
  let amount = result: wei => BN.create(strip0x(result), 16);
};

let addressMatcher = [%bs.re "/^0x[0-9a-fA-F]{40}$/"];

exception InvalidAddress(address);

let validateAddress = value => Js.Re.test(value, addressMatcher);

let toHex = amount => Js.Json.string(Printf.sprintf("0x%x", amount));

let toDict = [%raw obj => {| return obj |}];

module Encode = {
  let address = (addr) => Js.Json.string(addr);
  let block = (blk: blockNumber) => toHex(blk);
  let transaction = (tx: transaction) => Js.Json.object_(toDict(tx));

  let blockOrTag = value => switch (value) {
    | Block(amount) => Js.Json.string(Printf.sprintf("0x%x", amount))
    | Latest => Js.Json.string("latest")
    | Pending => Js.Json.string("pending")
    | Earliest => Js.Json.string("earliest")
    };
}

/* 
curl -X POST --data '{"jsonrpc":"2.0","method":"eth_call","params":[{"to":"0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c", "data":"0x06fdde03"],"id":1}' https://mainnet.infura.io/re-eth
*/
