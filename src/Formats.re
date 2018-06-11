open Belt;

type blockNumber = int;
type quantityResponse = string;
type blockOrTag = Block(blockNumber) | Earliest | Latest | Pending ;

type address = string;

type wei = int;
type gas = int;
type eth = float;

module Decode = {
  let quantity = result => int_of_string(result);
  let block = result: blockNumber => quantity(result);
  let amount = result: wei => quantity(result);
};

let addressMatcher = [%bs.re "/^0x[0-9a-fA-F]{40}$/"];

exception InvalidAddress(address);

let validateAddress = value => Js.Re.test(value, addressMatcher);

let toHex = amount => Js.Json.string(Printf.sprintf("0x%x", amount));

module Encode = {
  let address = (addr) => Js.Json.string(addr);
  let block = (blk: blockNumber) => toHex(blk);

  let blockOrTag = value => switch (value) {
    | Block(amount) => Js.Json.string(Printf.sprintf("0x%x", amount))
    | Latest => Js.Json.string("latest")
    | Pending => Js.Json.string("pending")
    | Earliest => Js.Json.string("earliest")
    };
}

