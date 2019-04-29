open Jest;
open Expect;
open Formats;


describe("Encode", () => {
  describe("#address", () => {
    test("address", () =>
      expect(Encode.address("0x01234")) |> toEqual(Js.Json.string("0x01234"))
    );
  });

  describe("#transaction", () => {
    test("tx", () =>
      expect(Encode.address("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae")) |> toEqual(Js.Json.string("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae"))
    );
  });

  describe("#blockOrTag", () => {
    test("small number", () => 
      expect(Encode.block(1234)) |> toEqual(Js.Json.string("0x4d2")));

    test("zero", () => 
      expect(Encode.block(0)) |> toEqual(Js.Json.string("0x0")));
  });

  describe("#blockOrTag", () => {
    test("small number", () => 
      expect(Encode.blockOrTag(Block(1234))) |> toEqual(Js.Json.string("0x4d2")));

    test("zero", () => 
      expect(Encode.blockOrTag(Block(0))) |> toEqual(Js.Json.string("0x0")));

    test("latest", () => {
      expect(Encode.blockOrTag(Latest)) |> toEqual(Js.Json.string("latest"));
    });

    test("pending", () => {
      expect(Encode.blockOrTag(Pending)) |> toEqual(Js.Json.string("pending"));
    });

    test("earliest", () => {
      expect(Encode.blockOrTag(Earliest)) |> toEqual(Js.Json.string("earliest"));
    });

  });
});

describe("Decode", () => {
  describe("quantity", () => {
    test("smallish number", () =>  expect(Decode.quantity("0x4d2")) |> toEqual(1234));
    test("zero", () =>  expect(Decode.quantity("0x0")) |> toEqual(0));
    test("largeish number", () =>  expect(Decode.quantity("0x0D8779a0")) |> toEqual(226982304));
  });

  describe("nonce", () => {
    test("smallish number", () =>  expect(Decode.nonce("0x4d2")) |> toEqual(1234));
    test("zero", () =>  expect(Decode.nonce("0x0")) |> toEqual(0));
    test("largeish number", () =>  expect(Decode.nonce("0x0D8779a0")) |> toEqual(226982304));
  });

  describe("block", () => {
    test("smallish number", () =>  expect(Decode.block("0x4d2")) |> toEqual(1234));
    test("zero", () =>  expect(Decode.block("0x0")) |> toEqual(0));
    test("largeish number", () =>  expect(Decode.block("0x0D8779a0")) |> toEqual(226982304));
  });

  describe("amount", () => {
    test("smallish number", () =>  expect(Bn.toNumber(Decode.amount("0x4d2"))) |> toEqual(1234.0));
    test("zero", () =>  expect(Bn.toNumber(Decode.amount("0x0"))) |> toEqual(0.0));
    test("largeish number", () =>  expect(Bn.toNumber(Decode.amount("0x0D8779a0"))) |> toEqual(226982304.0));
    test("large un-prefixed number", () =>  expect(Bn.toString(~base=10, Decode.amount("6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae"))) |> toEqual("2681210161307671758365144741753253651834466456474188701102"));
    test("large prefixed number", () =>  expect(Bn.toString(~base=10, Decode.amount("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae"))) |> toEqual("2681210161307671758365144741753253651834466456474188701102"));
  });

});

describe("strip0x", () => {
  describe("has prefix", () => {
    test("strips prefix", () => expect(strip0x("0x12314a")) |> toEqual("12314a"))
  });
 
  describe("no prefix", () => {
    test("leaves string alone", () => expect(strip0x("12314a")) |> toEqual("12314a"))
  });

});
