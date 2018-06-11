
type quantity = int;
type quantityResponse = string;
type quantityOrTag = Quantity(int) | BlockTag(string) ;

type address = Address(string);

type jsonRpcParamFormat = Address(string) | Quantity(int) | BlockTag(string);

module Decode = {
  let quantity = result => int_of_string(result);
};

let encode = value => switch (value) {
| Address(address) => Js.Json.string(address)
| Quantity(amount) => Js.Json.string(Printf.sprintf("0x%x", amount))
| BlockTag(tag) => Js.Json.string(tag)
};

