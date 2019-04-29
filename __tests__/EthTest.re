open Jest;
open Expect;
open Eth;
open Belt.Result;

describe("#blockNumber", () => {
  testPromise("fetches block", () =>
    blockNumber() 
    |> Repromise.Rejectable.map(result => switch (result) {
      | Ok(block) => expect(block) |> toBeGreaterThan(5768135)
      | Error(msg) => fail("No error should happen")
    })
    |> Repromise.Rejectable.toJsPromise
  );
});

describe("#getBalance", () => {
  testPromise("invalid address", () =>
    getBalance(~account="0x1234", ()) 
    |> Repromise.Rejectable.map(result => switch (result) {
      | Ok(_) => fail("this should not work")
      | Error(msg) => msg |> expect |> toEqual("Invalid Address: 0x1234")
    })
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("empty Balance", () =>
    getBalance(~account="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ())
    |> Repromise.Rejectable.map(result => switch (result) {
      | Ok(amount) => expect(Bn.toNumber(amount)) |> toEqual(0.0)
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("With Balance at block", () =>
    getBalance(~account="0x76CF5100f62BaBd1574c3d2082f01c6bb3b420f8", ~from=Formats.Block(5768167), ()) 
    |> Repromise.Rejectable.map(result => switch (result) {
      | Ok(amount) => Bn.toString(~base=10, amount) |> expect |> toEqual("6000000000000000")
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  );

});

describe("#getTransactionCount", () => {
  /* Don't know how to test this correclty yet  */
  /* testPromise("invalid address", () =>
    getBalance("0x1234", ()) |> Js.Promise.catch(error => expect(error) |> toBe(Formats.InvalidAddress("0x1234")) |> Js.Promise.resolve)
  ); */


  testPromise("with 1 transaction", () =>
    getTransactionCount(~account="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()) 
    |> Repromise.Rejectable.map(result => switch(result) {
      | Ok(result) => result |> expect |> toEqual(1)
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise

  );

  testPromise("with no transactions", () =>
    getTransactionCount(~account="0x76CF5100f62BaBd1574c3d2082f01c6bb3b420f8", ~from=Formats.Block(5768167), ()) 
    |> Repromise.Rejectable.map(result => switch(result) {
      | Ok(result) => result |> expect |> toEqual(0)
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  );

});

describe("#gasPrice", () => {
  testPromise("fetches gas price in wei", () =>
    gasPrice() 
    |> Repromise.Rejectable.map(result => switch(result) {
      | Ok(result) => Bn.toNumber(result) |> expect |> toBeGreaterThanOrEqual(2000000000.0)
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  )
});

describe("#call", () => {
  testPromise("calls contract with no result", () =>
    call(~tx=Formats.transaction(~contract="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()), ()) 
    |> Repromise.Rejectable.map(result => switch(result) {
      | Ok(result) => result |> expect |> toBe("0x")
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("calls contract with data", () =>
    call(~tx=Formats.transaction(~contract="0x0D8775F648430679A709E98d2b0Cb6250d2887EF", ~data="0x95d89b41", ()), ~from=Formats.Block(5768167), ()) 
    |> Repromise.Rejectable.map(result => switch(result) {
      | Ok(result) => result |> expect |> toBe("0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000034241540000000000000000000000000000000000000000000000000000000000")
      | Error(msg) => fail(msg)
    })
    |> Repromise.Rejectable.toJsPromise
  );

});
