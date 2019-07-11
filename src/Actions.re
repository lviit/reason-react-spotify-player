open Request;
open Spotify;
open StoreData;

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";

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

let rec action = (dispatch, state, actionType: actionType) => {
  let {player: {deviceId, accessToken}, progressTimer} = state;

  switch (actionType) {
  | PlaySong(uri) =>
    Js.Global.clearInterval(progressTimer);
    let timerId =
      Js.Global.setInterval(() => dispatch(IncrementProgress), 1000);
    Js.Promise.(
      playSong(uri, deviceId, accessToken)
      |> then_(_ =>
           setTimeout(_ => action(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
      |> then_(_ => dispatch(Play(timerId)) |> resolve)
    )  // wtf why the timeout
    |> ignore;
  | Prev =>
    Js.Promise.(
      request(Previous, accessToken)
      |> then_(_ =>
           setTimeout(_ => action(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | Next =>
    Js.Promise.(
      request(Next, accessToken)
      |> then_(_ =>
           setTimeout(_ => action(dispatch, state, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | FetchPlayer =>
    Js.Promise.(
      request(Player, accessToken)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> Decode.response |> resolve)
      |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      |> resolve
    )
    |> ignore
  | Play =>
    Js.Global.clearInterval(progressTimer);
    let timerId =
      Js.Global.setInterval(() => dispatch(IncrementProgress), 1000);
    Js.Promise.(
      request(Play, accessToken)
      |> then_(_ => dispatch(Play(timerId)) |> resolve)
    )
    |> ignore;
  | Pause =>
    Js.Global.clearInterval(progressTimer);
    Js.Promise.(
      request(Pause, accessToken) |> then_(_ => dispatch(Pause) |> resolve)
    )
    |> ignore;
  | FetchNewReleases =>
    FetchAlbumsPending->dispatch;
    Js.Promise.(
      request(NewReleases, accessToken)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> AlbumData.Decode.response |> resolve)
      |> then_(data => FetchAlbumsFulfilled(data)->dispatch->resolve)
    )
    |> ignore;
  | Search(keywords) =>
    FetchAlbumsPending->dispatch;
    Js.Promise.(
      request(Search(keywords), accessToken)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> AlbumData.Decode.response |> resolve)
      |> then_(data => FetchAlbumsFulfilled(data)->dispatch->resolve)
    )
    |> ignore;
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
             state->messageGet->Js.log
           );
        player
        |> addListener("authentication_error", state =>
             state->messageGet->Js.log
           );
        player
        |> addListener("account_error", state => state->messageGet->Js.log);
        player
        |> addListener("playback_error", state => state->messageGet->Js.log);
        player |> addListener("player_state_changed", state => state->Js.log);
        player
        |> addListener("ready", state => {
             state->device_idGet->Js.log2("Ready with Device ID");
             PlayerReady(state->device_idGet)->dispatch;
           });
        // dispatch ready here
        player
        |> addListener("not_ready", state =>
             state |> device_idGet |> Js.log2("Device ID has gone offline")
           );
        player |> connectPlayer() |> ignore;
      },
    );
  };
};