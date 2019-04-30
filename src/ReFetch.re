[%raw "require('isomorphic-fetch')"];

open Belt.Result;

let fetchJson = (url, payload) => {
  Repromise.Rejectable.fromJsPromise(
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
  |> Repromise.Rejectable.andThen(r =>
       Repromise.Rejectable.fromJsPromise(Fetch.Response.json(r))
     )
  |> Repromise.Rejectable.map(json => Ok(json))
  |> Repromise.Rejectable.catch(_ =>
       Repromise.Rejectable.resolved(Error("Error"))
     );
};
