open Webapi;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

/* State declaration */
type state = {
  count: int,
  show: bool,
};

/* Action declaration */
type action =
  | Click
  | Toggle;

type user = {
  id: int,
  name: string,
};

module Decode = {
  let user = user =>
    Json.Decode.{
      id: field("id", int, user),
      name: field("name", string, user),
    };
  let users = (json): list(user) => Json.Decode.list(user, json);
};

[@react.component]
let make = (~greeting) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Click => {...state, count: state.count + 1}
        | Toggle => {...state, show: !state.show}
        },
      {count: 0, show: true},
    );

  React.useEffect(() => {
    let url = Url.URLSearchParams.make(hash);
    let accessToken = Url.URLSearchParams.get("#access_token", url);
    let authHeader =
      switch (accessToken) {
      | None => "none"
      | Some(accessToken) => "Bearer " ++ accessToken
      };

    Js.Promise.(
      Fetch.fetchWithInit(
        "https://api.spotify.com/v1/me/player",
        Fetch.RequestInit.make(
          ~headers=Fetch.HeadersInit.make({"Authorization": authHeader}),
          (),
        ),
      )
      |> then_(Fetch.Response.json)
      |> then_(json =>
           json |> Decode.users |> (users => Some(users) |> resolve)
         )
      |> catch(_err => resolve(None))
    );
    Some(() => ());
  });

  let authEndpoint = "https://accounts.spotify.com/authorize";
  let clientId = "64d03692241b478cb763ec2a7eed99e0";
  let redirectUri = "http://localhost:8000";
  let scopes = ["user-read-currently-playing", "user-read-playback-state"];

  let rec join = (char: string, list: list(string)): string => {
    switch (list) {
    | [] => raise(Failure("Passed an empty list"))
    | [tail] => tail
    | [head, ...tail] => head ++ char ++ join(char, tail)
    };
  };

  let message =
    "You've clicked this " ++ string_of_int(state.count) ++ " times(s)";
  <div>
    <button onClick={_event => dispatch(Click)}>
      {ReasonReact.string(message)}
    </button>
    <button onClick={_event => dispatch(Toggle)}>
      {ReasonReact.string("Toggle greeting")}
    </button>
    <a
      className="btn btn--loginApp-link"
      href={
        authEndpoint
        ++ "?client_id="
        ++ clientId
        ++ "&redirect_uri="
        ++ redirectUri
        ++ "&scope="
        ++ join("%20", scopes)
        ++ "&response_type=token&show_dialog=true"
      }>
      {ReasonReact.string("Login to Spotify")}
    </a>
  </div>;
};