// Generated by BUCKLESCRIPT VERSION 3.1.5, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var BnJs = require("bn.js");
var Printf = require("bs-platform/lib/js/printf.js");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");

var BN = /* module */[];

function strip0x(value) {
  return value.replace((/^0x/), "");
}

var quantity = Caml_format.caml_int_of_string;

var nonce = Caml_format.caml_int_of_string;

var block = Caml_format.caml_int_of_string;

function amount(result) {
  return new BnJs(strip0x(result), 16);
}

var Decode = /* module */[
  /* quantity */quantity,
  /* nonce */nonce,
  /* block */block,
  /* amount */amount
];

var addressMatcher = (/^0x[0-9a-fA-F]{40}$/);

var InvalidAddress = Caml_exceptions.create("Formats.InvalidAddress");

function validateAddress(value) {
  return addressMatcher.test(value);
}

function toHex(amount) {
  return Curry._1(Printf.sprintf(/* Format */[
                  /* String_literal */Block.__(11, [
                      "0x",
                      /* Int */Block.__(4, [
                          /* Int_x */6,
                          /* No_padding */0,
                          /* No_precision */0,
                          /* End_of_format */0
                        ])
                    ]),
                  "0x%x"
                ]), amount);
}

var toDict = function (obj){ return obj };

function address(addr) {
  return addr;
}

var block$1 = toHex;

var transaction = toDict;

function blockOrTag(value) {
  if (typeof value === "number") {
    switch (value) {
      case 0 : 
          return "earliest";
      case 1 : 
          return "latest";
      case 2 : 
          return "pending";
      
    }
  } else {
    return Curry._1(Printf.sprintf(/* Format */[
                    /* String_literal */Block.__(11, [
                        "0x",
                        /* Int */Block.__(4, [
                            /* Int_x */6,
                            /* No_padding */0,
                            /* No_precision */0,
                            /* End_of_format */0
                          ])
                      ]),
                    "0x%x"
                  ]), value[0]);
  }
}

var Encode = /* module */[
  /* address */address,
  /* block */block$1,
  /* transaction */transaction,
  /* blockOrTag */blockOrTag
];

exports.BN = BN;
exports.strip0x = strip0x;
exports.Decode = Decode;
exports.addressMatcher = addressMatcher;
exports.InvalidAddress = InvalidAddress;
exports.validateAddress = validateAddress;
exports.toHex = toHex;
exports.toDict = toDict;
exports.Encode = Encode;
/* addressMatcher Not a pure module */
