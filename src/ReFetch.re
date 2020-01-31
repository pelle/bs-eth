[%raw "require('isomorphic-fetch')"];

open Belt.Result;

let fetchJson = (url, payload) => {
  Promise.Js.fromBsPromise(
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    ),
  )
  ->Promise.Js.flatMap(r => Promise.Js.fromBsPromise(Fetch.Response.json(r)))
  ->Promise.Js.map(json => Ok(json))
  ->Promise.Js.catch(_ => Promise.Js.resolved(Error("Error")));
};
