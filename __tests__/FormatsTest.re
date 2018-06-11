open Jest;
open Expect;
open Formats;


describe("Encode", () => {
  describe("#address", () => {
    test("address", () =>
      expect(Encode.address("0x01234")) |> toEqual(Js.Json.string("0x01234"))
    );
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
    test("smallish number", () => 
      expect(Decode.quantity("0x4d2")) |> toEqual(1234))
  })
});

