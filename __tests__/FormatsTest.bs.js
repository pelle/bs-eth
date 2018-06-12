// Generated by BUCKLESCRIPT VERSION 3.1.5, PLEASE EDIT WITH CARE
'use strict';

var Jest = require("@glennsl/bs-jest/src/jest.js");
var Formats = require("../src/Formats.bs.js");

describe("Encode", (function () {
        describe("#address", (function () {
                return Jest.test("address", (function () {
                              return Jest.Expect[/* toEqual */12]("0x01234", Jest.Expect[/* expect */0](Formats.Encode[/* address */0]("0x01234")));
                            }));
              }));
        describe("#transaction", (function () {
                return Jest.test("tx", (function () {
                              return Jest.Expect[/* toEqual */12]("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae", Jest.Expect[/* expect */0](Formats.Encode[/* address */0]("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae")));
                            }));
              }));
        describe("#blockOrTag", (function () {
                Jest.test("small number", (function () {
                        return Jest.Expect[/* toEqual */12]("0x4d2", Jest.Expect[/* expect */0](Formats.Encode[/* block */1](1234)));
                      }));
                return Jest.test("zero", (function () {
                              return Jest.Expect[/* toEqual */12]("0x0", Jest.Expect[/* expect */0](Formats.Encode[/* block */1](0)));
                            }));
              }));
        describe("#blockOrTag", (function () {
                Jest.test("small number", (function () {
                        return Jest.Expect[/* toEqual */12]("0x4d2", Jest.Expect[/* expect */0](Formats.Encode[/* blockOrTag */3](/* Block */[1234])));
                      }));
                Jest.test("zero", (function () {
                        return Jest.Expect[/* toEqual */12]("0x0", Jest.Expect[/* expect */0](Formats.Encode[/* blockOrTag */3](/* Block */[0])));
                      }));
                Jest.test("latest", (function () {
                        return Jest.Expect[/* toEqual */12]("latest", Jest.Expect[/* expect */0](Formats.Encode[/* blockOrTag */3](/* Latest */1)));
                      }));
                Jest.test("pending", (function () {
                        return Jest.Expect[/* toEqual */12]("pending", Jest.Expect[/* expect */0](Formats.Encode[/* blockOrTag */3](/* Pending */2)));
                      }));
                return Jest.test("earliest", (function () {
                              return Jest.Expect[/* toEqual */12]("earliest", Jest.Expect[/* expect */0](Formats.Encode[/* blockOrTag */3](/* Earliest */0)));
                            }));
              }));
        return /* () */0;
      }));

describe("Decode", (function () {
        describe("quantity", (function () {
                Jest.test("smallish number", (function () {
                        return Jest.Expect[/* toEqual */12](1234, Jest.Expect[/* expect */0](Formats.Decode[/* quantity */0]("0x4d2")));
                      }));
                Jest.test("zero", (function () {
                        return Jest.Expect[/* toEqual */12](0, Jest.Expect[/* expect */0](Formats.Decode[/* quantity */0]("0x0")));
                      }));
                return Jest.test("largeish number", (function () {
                              return Jest.Expect[/* toEqual */12](226982304, Jest.Expect[/* expect */0](Formats.Decode[/* quantity */0]("0x0D8779a0")));
                            }));
              }));
        describe("nonce", (function () {
                Jest.test("smallish number", (function () {
                        return Jest.Expect[/* toEqual */12](1234, Jest.Expect[/* expect */0](Formats.Decode[/* nonce */1]("0x4d2")));
                      }));
                Jest.test("zero", (function () {
                        return Jest.Expect[/* toEqual */12](0, Jest.Expect[/* expect */0](Formats.Decode[/* nonce */1]("0x0")));
                      }));
                return Jest.test("largeish number", (function () {
                              return Jest.Expect[/* toEqual */12](226982304, Jest.Expect[/* expect */0](Formats.Decode[/* nonce */1]("0x0D8779a0")));
                            }));
              }));
        describe("block", (function () {
                Jest.test("smallish number", (function () {
                        return Jest.Expect[/* toEqual */12](1234, Jest.Expect[/* expect */0](Formats.Decode[/* block */2]("0x4d2")));
                      }));
                Jest.test("zero", (function () {
                        return Jest.Expect[/* toEqual */12](0, Jest.Expect[/* expect */0](Formats.Decode[/* block */2]("0x0")));
                      }));
                return Jest.test("largeish number", (function () {
                              return Jest.Expect[/* toEqual */12](226982304, Jest.Expect[/* expect */0](Formats.Decode[/* block */2]("0x0D8779a0")));
                            }));
              }));
        describe("amount", (function () {
                Jest.test("smallish number", (function () {
                        return Jest.Expect[/* toEqual */12](1234, Jest.Expect[/* expect */0](Formats.Decode[/* amount */3]("0x4d2").toNumber()));
                      }));
                Jest.test("zero", (function () {
                        return Jest.Expect[/* toEqual */12](0, Jest.Expect[/* expect */0](Formats.Decode[/* amount */3]("0x0").toNumber()));
                      }));
                Jest.test("largeish number", (function () {
                        return Jest.Expect[/* toEqual */12](226982304, Jest.Expect[/* expect */0](Formats.Decode[/* amount */3]("0x0D8779a0").toNumber()));
                      }));
                Jest.test("large un-prefixed number", (function () {
                        return Jest.Expect[/* toEqual */12]("2681210161307671758365144741753253651834466456474188701102", Jest.Expect[/* expect */0](Formats.Decode[/* amount */3]("6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae").toString(10)));
                      }));
                return Jest.test("large prefixed number", (function () {
                              return Jest.Expect[/* toEqual */12]("2681210161307671758365144741753253651834466456474188701102", Jest.Expect[/* expect */0](Formats.Decode[/* amount */3]("0x6d5923e6449122cbbcc1b96093e0b7e4fd3e469f58daddae").toString(10)));
                            }));
              }));
        return /* () */0;
      }));

describe("strip0x", (function () {
        describe("has prefix", (function () {
                return Jest.test("strips prefix", (function () {
                              return Jest.Expect[/* toEqual */12]("12314a", Jest.Expect[/* expect */0](Formats.strip0x("0x12314a")));
                            }));
              }));
        describe("no prefix", (function () {
                return Jest.test("leaves string alone", (function () {
                              return Jest.Expect[/* toEqual */12]("12314a", Jest.Expect[/* expect */0](Formats.strip0x("12314a")));
                            }));
              }));
        return /* () */0;
      }));

/*  Not a pure module */
