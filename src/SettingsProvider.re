open Webapi;
open Utils;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

type settings = {
  authHeader: string,
  deviceId: string,
};

type artist = {
  name: string
}

type item = {
  id: string,
  name: string,
  progress_ms: int,
  artists: list(artist)
};

type state = {
  data: item,
  loading: bool,
  playing: bool,
  currentSong: string,
  albumDataLoading: bool,
  albumData: list(AlbumData.album),
};

type response = {item};

let url = Url.URLSearchParams.make(hash);
let accessToken = Url.URLSearchParams.get("#access_token", url);
let authHeader =
  switch (accessToken) {
  | None => "none"
  | Some(accessToken) => "Bearer " ++ accessToken
  };

let deviceId =
  switch (Url.URLSearchParams.get("device_id", url)) {
  | None => "none"
  | Some(deviceId) => deviceId
  };

/* Action declaration */
type action =
  | TogglePlay
  | Play(string)
  | FetchDataPending
  | FetchDataFulfilled(response)
  | FetchAlbumDataPending
  | FetchAlbumDataFulfilled(AlbumData.response);

module Decode = {
  let artist = json =>
    Json.Decode.{
      name: json |> field("name", string),
    };

  let item = json =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      progress_ms: json |> field("progress_ms", int),
      artists: json |> field("artists", list(artist))
    };

  let response = json => Json.Decode.{item: json |> field("item", item)};
};

let reducer = (state, action) =>
  switch (action) {
  | Play(uri) => {...state, currentSong: uri}
  | TogglePlay => {...state, playing: !state.playing}
  | FetchDataPending => {...state, loading: true}
  | FetchDataFulfilled(data) => {...state, loading: false, data: data.item}
  | FetchAlbumDataPending => {...state, albumDataLoading: true}
  | FetchAlbumDataFulfilled(data) => {
      ...state,
      albumDataLoading: false,
      albumData: data.albums.items,
    }
  };

let initialState = {
  loading: false,
  playing: false,
  data: {
    id: "",
    name: "",
    progress_ms: 0,
    artists: [],
  },
  albumDataLoading: false,
  albumData: [],
  currentSong: "",
};

let settingsContext = React.createContext((initialState, action => ()));
module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let make = React.Context.provider(settingsContext);
};

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  // play song
  React.useEffect1(
    () => {
      if (state.currentSong !== "") {
        let payload = Js.Dict.empty();
        Js.Dict.set(payload, "context_uri", Js.Json.string(state.currentSong));

        Js.Promise.(
          putWithAuth(
            "https://api.spotify.com/v1/me/player/play?device_id=" ++ deviceId,
            authHeader,
            payload,
          )
          |> then_(resolve)
        )
        |> ignore;
      };
      Some(() => ());
    },
    [|state.currentSong|],
  );

  // fetch player data
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
    [|state.currentSong|],
  );

  // toggle play/pause
  React.useEffect1(
    () => {
      Js.log("play toggle effect firing");
      let url =
        "https://api.spotify.com/v1/me/player/"
        ++ (state.playing ? "pause" : "play");
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
  // fetch album data
  React.useEffect1(
    () => {
      dispatch(FetchAlbumDataPending);

      Js.Promise.(
        fetchWithAuth(
          "https://api.spotify.com/v1/browse/new-releases",
          authHeader,
        )
        |> then_(Fetch.Response.json)
        |> then_(json => json |> AlbumData.Decode.response |> resolve)
        |> then_(data => FetchAlbumDataFulfilled(data) |> dispatch |> resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [||],
  );

  <Provider value=(state, dispatch)> children </Provider>;
};