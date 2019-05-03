type t;
type class_;
[@bs.send] external toString: (t, int) => string = "";
[@bs.module "jsbi"] external jsbi: class_ = "default";
[@bs.send] external __toInt: (class_, t) => int = "toNumber";
let toInt = (bi: t): int => __toInt(jsbi, bi);

[@bs.send] external __fromString: (class_, string, int) => t = "__fromString";
let fromString = (value: string, radix: int): t =>
  __fromString(jsbi, value, radix);

[@bs.send] external __fromInt: (class_, int) => t = "BigInt";
let fromInt = (value: int): t => __fromInt(jsbi, value);

[@bs.send] external __fromFloat: (class_, float) => t = "BigInt";
let fromFloat = (value: float): t => __fromFloat(jsbi, value);
