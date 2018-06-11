// Generated by BUCKLESCRIPT VERSION 3.1.5, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Fetch = require("bs-fetch/src/Fetch.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");

((require('isomorphic-fetch')));

function jsonRpcRequestPayload(method_, params) {
  var payload = { };
  payload["jsonrpc"] = "2.0";
  payload["id"] = 1.0;
  payload["method"] = method_;
  payload["params"] = params;
  return payload;
}

var JsonRpcError = Caml_exceptions.create("JsonRpc.JsonRpcError");

function handleResponse(json) {
  var error = Json_decode.optional(Json_decode.at(/* :: */[
            "error",
            /* :: */[
              "message",
              /* [] */0
            ]
          ], Json_decode.string), json);
  if (error) {
    return /* Error */Block.__(1, [error[0]]);
  } else {
    return /* Ok */Block.__(0, [Json_decode.field("result", Json_decode.string, json)]);
  }
}

function jsonRpcRequest(method_, params) {
  return fetch("https://mainnet.infura.io/re-eth", Fetch.RequestInit[/* make */0](/* Some */[/* Post */2], /* Some */[{
                          "Content-Type": "application/json"
                        }], /* Some */[JSON.stringify(jsonRpcRequestPayload(method_, params))], /* None */0, /* None */0, /* None */0, /* None */0, /* None */0, /* None */0, /* None */0, /* None */0)(/* () */0)).then((function (prim) {
                  return prim.json();
                })).then((function (json) {
                var match = handleResponse(json);
                if (match.tag) {
                  return Promise.reject([
                              JsonRpcError,
                              match[0]
                            ]);
                } else {
                  return Promise.resolve(match[0]);
                }
              }));
}

exports.jsonRpcRequestPayload = jsonRpcRequestPayload;
exports.JsonRpcError = JsonRpcError;
exports.handleResponse = handleResponse;
exports.jsonRpcRequest = jsonRpcRequest;
/*  Not a pure module */
