open Utils;
open SettingsProvider;

type item = {
  id: string,
  name: string,
};

type state = {
  data: item,
  loading: bool,
  playing: bool,
};

type response = {item};

/* Action declaration */
type action =
  | TogglePlay
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
let make = () => {
  let { authHeader } = React.useContext(settingsContext);

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | TogglePlay => {...state, playing: !state.playing}
        | FetchDataPending => {...state, loading: true}
        | FetchDataFulfilled(data) => {
            ...state,
            loading: false,
            data: data.item,
          }
        },
      {
        loading: false,
        playing: false,
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
    [|state.playing|],
  );

  React.useEffect1(
    () => {
      Js.log("play toggle effect firing");
      let url = "https://api.spotify.com/v1/me/player/" ++ (state.playing ? "pause" : "play");
      Js.Promise.(
        Fetch.fetchWithInit(
          url,
          Fetch.RequestInit.make(
            ~headers=Fetch.HeadersInit.make({"Authorization": authHeader}),
            ~method_=Put,
            (),
          ),
        )
        |> then_(resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [|state.playing|],
  );

  //React.useContext

  let text = "now playing" ++ state.data.name;

  <div>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string("previous")}
    </button>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string(state.playing ? "pause" : "play")}
    </button>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string("next")}
    </button>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
    <h3> {ReasonReact.string(text)} </h3>
  </div>;
};