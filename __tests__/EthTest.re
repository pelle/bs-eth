open Jest;
open Expect;
open Eth;

describe("#blockNumber", () => {
  testPromise("fetches block", () =>
    blockNumber() |> Js.Promise.then_(no => expect(no) |> toBeGreaterThan(5768135) |> Js.Promise.resolve)
  )
});

describe("#getBalance", () => {
  /* Don't know how to test this correclty yet  */
  /* testPromise("invalid address", () =>
    getBalance("0x1234", ()) |> Js.Promise.catch(error => expect(error) |> toBe(Formats.InvalidAddress("0x1234")) |> Js.Promise.resolve)
  ); */


  testPromise("empty Balance", () =>
    getBalance(~account="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()) |> Js.Promise.then_(amount => expect(Formats.BN.toNumber(amount)) |> toBe(0) |> Js.Promise.resolve)
  );

  testPromise("With Balance at block", () =>
    getBalance(~account="0x76CF5100f62BaBd1574c3d2082f01c6bb3b420f8", ~from=Formats.Block(5768167), ()) |> Js.Promise.then_(amount => expect(Formats.BN.toString(amount, 10)) |> toBe("6000000000000000") |> Js.Promise.resolve)
  );

});

describe("#getTransactionCount", () => {
  /* Don't know how to test this correclty yet  */
  /* testPromise("invalid address", () =>
    getBalance("0x1234", ()) |> Js.Promise.catch(error => expect(error) |> toBe(Formats.InvalidAddress("0x1234")) |> Js.Promise.resolve)
  ); */


  testPromise("with 1 transaction", () =>
    getTransactionCount(~account="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()) |> Js.Promise.then_(count => expect(count) |> toBe(1) |> Js.Promise.resolve)
  );

  testPromise("with no transactions", () =>
    getTransactionCount(~account="0x76CF5100f62BaBd1574c3d2082f01c6bb3b420f8", ~from=Formats.Block(5768167), ()) |> Js.Promise.then_(count => expect(count) |> toBe(0) |> Js.Promise.resolve)
  );

});

describe("#gasPrice", () => {
  testPromise("fetches gas price in wei", () =>
    gasPrice() |> Js.Promise.then_(price => expect(Formats.BN.toNumber(price)) |> toBeGreaterThanOrEqual(2000000000) |> Js.Promise.resolve)
  )
});

describe("#call", () => {
  testPromise("calls contract with no result", () =>
    call(~tx=Formats.transaction(~contract="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()), ()) |> Js.Promise.then_(data => expect(data) |> toBe("0x") |> Js.Promise.resolve)
  );

  testPromise("calls contract with data", () =>
    call(~tx=Formats.transaction(~contract="0x0D8775F648430679A709E98d2b0Cb6250d2887EF", ~data="0x95d89b41", ()), ~from=Formats.Block(5768167), ()) |> Js.Promise.then_(data => expect(data) |> toBe("0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000034241540000000000000000000000000000000000000000000000000000000000") |> Js.Promise.resolve)
  );

});
