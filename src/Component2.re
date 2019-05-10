open Utils;

type item = {
  id: string,
  name: string,
};

type state = {
  count: int,
  show: bool,
  data: item,
  loading: bool,
};

type response = {item};

/* Action declaration */
type action =
  | Click
  | Toggle
  | FetchDataPending
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
let make = (~authHeader) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Click => {...state, count: state.count + 1}
        | Toggle => {...state, show: !state.show}
        | FetchDataPending => {...state, loading: true}
        | FetchDataFulfilled(data) => {
            ...state,
            loading: false,
            data: data.item,
          }
        },
      {
        count: 0,
        show: true,
        loading: false,
        data: {
          id: "",
          name: "",
        },
      },
    );

  React.useEffect1(
    () => {
      dispatch(FetchDataPending);

      Js.Promise.(
        fetchWithAuth("https://api.spotify.com/v1/me/player", authHeader)
        |> then_(Fetch.Response.json)
        |> then_(json => json |> Decode.response |> resolve)
        |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [||],
  );

  let authEndpoint = "https://accounts.spotify.com/authorize";
  let clientId = "64d03692241b478cb763ec2a7eed99e0";
  let redirectUri = "http://localhost:8000";
  let scopes = ["user-read-currently-playing", "user-read-playback-state"];
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
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
    <h3> {ReasonReact.string(text)} </h3>
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