open Jest;
open Expect;
open Eth;
open Belt.Result;

[@bs.module "ganache-cli"]
external ganache: unit => Providers.Web3provider.t = "provider";

let provider = Providers.web3(ganache());
/* let provider = Providers.http("http://localhost:7545"); */
let infura = Providers.http("https://mainnet.infura.io/bs-eth");

let randomAccount = "0x2036c6cd85692f0fb2c26e6c6b2eced9e4478dfd";
let primaryAddress = ref(randomAccount);
let allAccounts = ref([|randomAccount|]);
let lastTx = ref("0x");

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

describe("#transactionCount", () => {
  testPromise("with 1 transaction", () =>
    transactionCount(~provider, ~account=primaryAddress^, ())
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) => result |> expect |> toEqual(0)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("with no transactions", () =>
    transactionCount(
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
          ~to_=randomAccount,
          ~from=primaryAddress^,
          ~value=BigInt.fromString("10000000000000000000", 10),
          ~nonce=0,
          (),
        ),
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(result) =>
           lastTx := result;
           String.length(result) |> expect |> toBe(66);
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#sendRawTransaction", () => {
  testPromise("sends eth", () =>
    sendRawTransaction(
      ~provider,
      ~tx=
        "0xf864808207d08301e0f3942036c6cd85692f0fb2c26e6c6b2eced9e4478dfd8204cf001ca0f5d033ee6f1be4cca13379137234e7d1e620c4d268c679c560d5609dac6b3f15a06738dc64a3d7f4f419461b3dbfb0f0a26433ac4adbc45d636ba5bc81dc9f1028",
    )
    |> Repromise.map(result =>
         switch (result) {
         | Ok(hash) =>
           hash
           |> expect
           |> toEqual(
                "0x218f5bab3339d03f52a202fb571c4a259482569163af0b61396b9d4b2e169985",
              )
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("fails with invalid transaction", () =>
    sendRawTransaction(~provider, ~tx="0x0")
    |> Repromise.map(result =>
         switch (result) {
         | Ok(_) => fail("Bad transaction should not be accepted")
         | Error(msg) => msg |> expect |> toEqual("Invalid Signature")
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );
});

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
    |> Repromise.map(result =>
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

describe("#transactionByHash", () =>
  testPromise("fetches TX", () =>
    transactionByHash(~provider, ~txHash=lastTx^)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(resp) =>
           let pt: Formats.postedTx = resp;
           pt.from |> expect |> toEqual(primaryAddress^);
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#transactionReceipt", () =>
  testPromise("fetches TX Receipt", () =>
    transactionReceipt(~provider, ~txHash=lastTx^)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(resp) =>
           let pt: Formats.receipt = resp;
           pt.from |> expect |> toEqual(primaryAddress^);
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);

describe("#blocksByXXX", () => {
  let lastBlock = ref(None);

  testPromise("fetches Block by number", () =>
    blockByNumber(~provider, ~blockNumber=2, ~deep=true)
    |> Repromise.map(blockResult =>
         switch (blockResult) {
         | Ok(blk) =>
           lastBlock := Some(blk);
           blk.Formats.number |> expect |> toEqual(2);
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  );

  testPromise("fetches Block by hash", () =>
    switch (lastBlock^) {
    | Some(blk1) =>
      blockByHash(~provider, ~blockHash=blk1.Formats.hash, ~deep=true)
      |> Repromise.map(blockResult =>
           switch (blockResult) {
           | Ok(blk) => blk |> expect |> toEqual(blk1)
           | Error(msg) => fail(msg)
           }
         )
      |> Repromise.Rejectable.toJsPromise
    | None =>
      Repromise.Rejectable.resolved(fail("did not have ablock"))
      |> Repromise.Rejectable.toJsPromise
    }
  );
});

describe("#netVersion", () =>
  testPromise("fetches version", () =>
    netVersion(infura)
    |> Repromise.map(result =>
         switch (result) {
         | Ok(netId) => expect(netId) |> toEqual(1)
         | Error(msg) => fail(msg)
         }
       )
    |> Repromise.Rejectable.toJsPromise
  )
);
