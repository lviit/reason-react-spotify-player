open Webapi;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

type item = {
  id: string,
  name: string,
};

type state = {
  count: int,
  show: bool,
  data: item,
};

type response = {item};

/* Action declaration */
type action =
  | Click
  | Toggle
  | FetchData
  | FetchDataFulfilled(response);

module Decode = {
  let item = json =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
    };

  let response = json => Json.Decode.{item: json |> field("item", item)};
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Click => {...state, count: state.count + 1}
        | Toggle => {...state, show: !state.show}
        | FetchDataFulfilled(data) => {...state, data: data.item} 
        },
      {count: 0, show: true, data: { id: "", name: ""}},
    );

  React.useEffect1(() => {
    Js.log("fetching data");
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
      |> then_(json => Decode.response(json) |> resolve)
      |> then_(data => dispatch(FetchDataFulfilled(data)) |> resolve)
    );
    Some(() => ());
  }, [||]);

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

  let text = "now playing" ++ state.data.name;

  <div>
    <button onClick={_event => dispatch(Click)}>
      {ReasonReact.string(message)}
    </button>
    <button onClick={_event => dispatch(Toggle)}>
      {ReasonReact.string("Toggle greeting")}
    </button>
    <h3>{ReasonReact.string(text)}</h3>
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