open Jest;
open Expect;
open Formats;


describe("#encode", () => {
  describe("Address", () => {
    test("address", () =>
      expect(encode(Address("0x01234"))) |> toEqual(Js.Json.string("0x01234"))
    );
  });

  describe("Quantity", () => {
    test("small number", () => 
      expect(encode(Quantity(1234))) |> toEqual(Js.Json.string("0x4d2")));

    test("zero", () => 
      expect(encode(Quantity(0))) |> toEqual(Js.Json.string("0x0")));
  });
  
  describe("BlockTag", () => {
    test("latest", () => {
      expect(encode(BlockTag("latest"))) |> toEqual(Js.Json.string("latest"));
    })
  });
});

describe("Decode", () => {
  describe("quantity", () => {
    test("smallish number", () => 
      expect(Decode.quantity("0x4d2")) |> toEqual(1234))
  })
});

