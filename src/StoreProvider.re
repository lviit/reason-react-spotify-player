open Request;
open Spotify;

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";

type artist = {name: string};

type item = {
  id: string,
  name: string,
  artists: list(artist),
};

type player = {
  accessToken: string,
  deviceId: string,
  loading: bool,
};

type state = {
  data: item,
  loading: bool,
  playing: bool,
  progress: int,
  progressTimer: Js.Global.intervalId,
  albumDataLoading: bool,
  albumData: list(AlbumData.album),
  player,
};

type response = {
  item,
  progress_ms: int,
};

type reducer =
  | Play(Js.Global.intervalId)
  | Pause
  | FetchDataPending
  | FetchDataFulfilled(response)
  | FetchAlbumDataPending
  | FetchAlbumDataFulfilled(AlbumData.response)
  | IncrementProgress
  | PlayerLoading(string)
  | PlayerReady(string);

type action =
  | Prev
  | Next
  | PlaySong(string)
  | Play
  | Pause
  | FetchPlayer
  | FetchAlbumDataPending
  | LoadPlayer;

module Decode = {
  let artist = json => Json.Decode.{name: json |> field("name", string)};

  let item = json =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      artists: json |> field("artists", list(artist)),
    };

  let response = json =>
    Json.Decode.{
      item: json |> field("item", item),
      progress_ms: json |> field("progress_ms", int),
    };
};

let reducer = (state, reducer: reducer) =>
  switch (reducer) {
  | Play(timerId) => {...state, playing: true, progressTimer: timerId}
  | Pause => {...state, playing: false}
  | IncrementProgress => {...state, progress: state.progress + 1}
  | FetchDataPending => {...state, loading: true}
  | FetchDataFulfilled(data) => {
      ...state,
      loading: false,
      data: data.item,
      progress: data.progress_ms / 1000,
    }
  | FetchAlbumDataPending => {...state, albumDataLoading: true}
  | FetchAlbumDataFulfilled(data) => {
      ...state,
      albumDataLoading: false,
      albumData: data.albums.items,
    }
  | PlayerLoading(accessToken) => {
      ...state,
      player: {
        ...state.player,
        accessToken,
        loading: true,
      },
    }
  | PlayerReady(deviceId) => {
      ...state,
      player: {
        ...state.player,
        loading: false,
        deviceId,
      },
    }
  };

let initialState = {
  loading: false,
  playing: false,
  player: {
    accessToken: "",
    deviceId: "",
    loading: false,
  },
  data: {
    id: "",
    name: "",
    artists: [],
  },
  progress: 0,
  progressTimer: Js.Global.setInterval(() => (), 1000000), // fix so we can default to unit
  albumDataLoading: false,
  albumData: [],
};

let rec handleAsync = (dispatch, state, action: action) => {
  switch (action) {
  | PlaySong(uri) =>
    Js.Global.clearInterval(state.progressTimer);
    let timerId =
      Js.Global.setInterval(() => dispatch(IncrementProgress), 1000);
    Js.Promise.(
      playSong(uri, state.player.deviceId, state.player.accessToken)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
      |> then_(_ => dispatch(Play(timerId)) |> resolve)
    )  // wtf why the timeout
    |> ignore;
  | Prev =>
    Js.Promise.(
      request(Previous, state.player.accessToken)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | Next =>
    Js.Promise.(
      request(Next, state.player.accessToken)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | FetchPlayer =>
    Js.Promise.(
      request(Player, state.player.accessToken)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> Decode.response |> resolve)
      |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      |> resolve
    )
    |> ignore
  | Play =>
    Js.Global.clearInterval(state.progressTimer);
    let timerId =
      Js.Global.setInterval(() => dispatch(IncrementProgress), 1000);
    Js.Promise.(
      request(Play, state.player.accessToken) |> then_(_ => dispatch(Play(timerId)) |> resolve)
    )
    |> ignore;
  | Pause =>
    Js.Global.clearInterval(state.progressTimer);
    Js.Promise.(request(Pause, state.player.accessToken) |> then_(_ => dispatch(Pause) |> resolve))
    |> ignore;
  | FetchAlbumDataPending =>
    Js.Promise.(
      request(NewReleases, state.player.accessToken)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> AlbumData.Decode.response |> resolve)
      |> then_(data => FetchAlbumDataFulfilled(data) |> dispatch |> resolve)
    )
    |> ignore
  | LoadPlayer =>
    let hash = getLocationHash(window);
    //setLocationHash(window, "");
    let accessToken =
      switch (
        hash
        |> Webapi.Url.URLSearchParams.make
        |> Webapi.Url.URLSearchParams.get("#access_token", _)
      ) {
      | None => "none"
      | Some(accessToken) => accessToken
      };

    PlayerLoading(accessToken) |> dispatch;

    onSpotifyWebPlaybackSDKReady(
      window,
      () => {
        let settings =
          settings(~name="Reason client test", ~getOAuthToken=cb =>
            cb(accessToken)
          );
        let player = createSpotifyPlayer(settings);
        player
        |> addListener("initialization_error", state =>
             state |> messageGet |> Js.log
           )
        |> ignore;
        player
        |> addListener("authentication_error", state =>
             state |> messageGet |> Js.log
           )
        |> ignore;
        player
        |> addListener("account_error", state =>
             state |> messageGet |> Js.log
           )
        |> ignore;
        player
        |> addListener("playback_error", state =>
             state |> messageGet |> Js.log
           )
        |> ignore;
        player
        |> addListener("player_state_changed", state => state |> Js.log)
        |> ignore;
        player
        |> addListener("ready", state => {
             state |> device_idGet |> Js.log2("Ready with Device ID");
             PlayerReady(state |> device_idGet) |> dispatch;
           })
        |> ignore;
        // dispatch ready here
        player
        |> addListener("not_ready", state =>
             state |> device_idGet |> Js.log2("Device ID has gone offline")
           )
        |> ignore;
        player |> connectPlayer() |> ignore;
      },
    );
  };
};

let storeContext = React.createContext((initialState, _ => ()));
module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let make = React.Context.provider(storeContext);
};

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  <Provider value=(state, handleAsync(dispatch, state))>
    children
  </Provider>;
};