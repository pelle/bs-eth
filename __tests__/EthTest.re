open Jest;
open Expect;
open Eth;
open Belt.Result;

[@bs.module "ganache-cli"]
external ganache: unit => Providers.Web3provider.t = "provider";

let provider = Providers.web3(ganache());
/* let provider = Providers.http("http://localhost:7545"); */
let infura = Providers.http("https://mainnet.infura.io/bs-eth");

let randomAccount = "0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275";
let primaryAddress = ref(randomAccount);
let allAccounts = ref([|randomAccount|]);

beforeAllPromise(() =>
  coinbase(provider)
  |> Repromise.map(result =>
       switch (result) {
       | Ok(address) =>
         primaryAddress := address;
         /* Js.log("address: " ++ primaryAddress^); */
         ();
       | Error(msg) => Js.log(msg)
       }
     )
  |> Repromise.Rejectable.toJsPromise
);

describe("#blockNumber", () => {
  testPromise("fetches block", () =>
    blockNumber(provider)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(block) => expect(block) |> toBe(0)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  describe("updates block", () => {
    beforeAllPromise(() =>
      mineBlock(provider) |> Repromise.Rejectable.toJsPromise
    );

    testPromise("updates block", () =>
      blockNumber(provider)
      |> Repromise.map(result =>
           switch (result) {
           | Ok(block) => expect(block) |> toBe(1)
           | Error(msg) => fail(msg)
           }
         )
      |> Repromise.Rejectable.toJsPromise
    );
  });

  testPromise("fetches mainnet block", () =>
    blockNumber(infura)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(block) => expect(block) |> toBeGreaterThanOrEqual(7670624)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );
});

describe("accounts", () =>
  testPromise("fetches accounts", () =>
    accounts(provider)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(accounts) =>
           allAccounts := accounts;
           expect(Belt.Array.length(accounts)) |> toEqual(10);
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#balanceOf", () => {
  testPromise("invalid address", () =>
    balanceOf(~provider, ~account="0x1234", ())
    |> Repromise.map(result =>
         switch (result) {
         | Ok(_) => fail("this should not work")
         | Error(msg) => msg |> expect |> toEqual("Invalid Address: 0x1234")
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("empty Balance", () =>
    balanceOf(~provider, ~account=randomAccount, ())
    |> Repromise.map(result =>
         switch (result) {
         | Ok(amount) => expect(BigInt.toInt(amount)) |> toEqual(0)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("With Balance at block", () =>
    balanceOf(
      ~provider,
      ~account=primaryAddress^,
      ~from=Formats.Block(0),
      (),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(amount) =>
           BigInt.toString(amount, 10)
           |> expect
           |> toEqual("100000000000000000000")
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );
});

describe("#getTransactionCount", () => {
  testPromise("with 1 transaction", () =>
    getTransactionCount(~provider, ~account=primaryAddress^, ())
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) => result |> expect |> toEqual(0)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("with no transactions", () =>
    getTransactionCount(
      ~provider,
      ~account=primaryAddress^,
      ~from=Formats.Block(0),
      (),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) => result |> expect |> toEqual(0)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );
});

describe("#gasPrice", () =>
  testPromise("fetches gas price in wei", () =>
    gasPrice(provider)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) =>
           BigInt.toInt(result)
           |> expect
           |> toBeGreaterThanOrEqual(2000000000)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#sendTransaction", () =>
  testPromise("sends eth", () =>
    sendTransaction(
      ~provider,
      ~tx=
        Formats.tx(
          ~to_="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275",
          ~from=primaryAddress^,
          ~value=BigInt.fromString("10000000000000000000", 10),
          ~nonce=0,
          (),
        ),
      (),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) => String.length(result) |> expect |> toBe(66)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#estimateGas", () =>
  testPromise("receives estimation", () =>
    estimateGas(
      ~provider,
      ~tx=
        Formats.tx(
          ~to_="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275",
          ~from=primaryAddress^,
          ~value=BigInt.fromString("10000000000000000000", 10),
          ~nonce=1,
          (),
        ),
      (),
    )
    |> Repromise.map(result
         /* Js.log(result); */
         =>
           switch (result) {
           | Ok(result) =>
             BigInt.toString(result, 10) |> expect |> toBe("21000")
           | Error(msg) => fail(msg)
           }
         )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#call", () => {
  testPromise("calls contract with no result", () =>
    call(
      ~provider,
      ~tx=Formats.tx(~to_="0x160c5ce58e2cc4fe7cc45a9dd569a10083b2a275", ()),
      (),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) => result |> expect |> toBe("0x")
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("calls contract with data", () =>
    call(
      ~provider=infura,
      ~tx=
        Formats.tx(
          ~to_="0x0D8775F648430679A709E98d2b0Cb6250d2887EF",
          ~data="0x95d89b41",
          (),
        ),
      ~from=Formats.Block(5768167),
      (),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) =>
           result
           |> expect
           |> toBe(
                "0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000034241540000000000000000000000000000000000000000000000000000000000",
              )
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );
});
