open Request;
open Spotify;
open StoreData;
open Js.Promise;
open Js.Global;

module URLSearchParams = {
  type t;
  [@bs.new] external make: string => t = "URLSearchParams";
  [@bs.return nullable] [@bs.send.pipe: t] external get: string => option(string) = "";
};

module Element = {
  type document;
  [@bs.send] external createElement: (document, string) => Dom.element = "";
  [@bs.val] external document: document = "document";
  [@bs.set] external setSrc: (Dom.element, string) => unit = "src";
  [@bs.send] [@bs.scope "head"]
  external appendToHead: (document, Dom.element) => unit = "appendChild";
};

let action = (dispatch, state, actionType: actionType) => {
  let {player: {deviceId, accessToken}, progressTimer} = state;

  switch (actionType) {
  | PlaySong(trackUri, contextUri) =>
    clearInterval(progressTimer);
    let timerId = setInterval(() => IncrementProgress->dispatch, 1000);
    playSong(trackUri, contextUri, deviceId, accessToken)
    |> then_(_ => Play(timerId)->dispatch->resolve)
    |> ignore;
  | Prev => request(Previous, accessToken) |> ignore
  | Next => request(Next, accessToken) |> ignore
  | Play =>
    clearInterval(progressTimer);
    let timerId = setInterval(() => IncrementProgress->dispatch, 1000);
    request(Play, accessToken) |> then_(_ => Play(timerId)->dispatch->resolve) |> ignore;
  | Pause =>
    clearInterval(progressTimer);
    request(Pause, accessToken) |> then_(_ => Pause->dispatch->resolve) |> ignore;
  | FetchNewReleases =>
    FetchAlbumsPending->dispatch;
    request(NewReleases, accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->AlbumData.Decode.response->resolve)
    |> then_(data => FetchAlbumsFulfilled(data)->dispatch->resolve)
    |> ignore;
  | Search(keywords) =>
    FetchAlbumsPending->dispatch;
    request(Search(keywords), accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->AlbumData.Decode.response->resolve)
    |> then_(data => FetchAlbumsFulfilled(data)->dispatch->resolve)
    |> ignore;
  | FetchAlbumDetails(id) =>
    OpenAlbumDetails->dispatch;
    FetchAlbumDetailsPending->dispatch;
    request(AlbumDetails(id), accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->AlbumData.Decode.albumDetails->resolve)
    |> then_(data => FetchAlbumDetailsFulfilled(data)->dispatch->resolve)
    |> ignore;
  | CloseAlbumDetails => CloseAlbumDetails->dispatch
  | Seek(progress) =>
    request(Seek(progress), accessToken)
    |> then_(_ => Seek(progress)->dispatch->resolve)
    |> ignore
  | LoadPlayer =>
    let accessToken =
      window->getLocationHash->URLSearchParams.make |> URLSearchParams.get("#access_token");
    switch (accessToken) {
    | None => Js.log("No access token available")
    | Some(accessToken) =>
      setLocationHash(window, "");
      PlayerLoading(accessToken) |> dispatch;
      onSpotifyWebPlaybackSDKReady(
        window,
        () => {
          let settings =
            settings(~name="Reason client test", ~getOAuthToken=cb => cb(accessToken));
          let player = createSpotifyPlayer(settings);
          player |> addListener("initialization_error", state => state->messageGet->Js.log);
          player |> addListener("authentication_error", state => state->messageGet->Js.log);
          player |> addListener("account_error", state => state->messageGet->Js.log);
          player |> addListener("playback_error", state => state->messageGet->Js.log);
          player
          |> addStateListener("player_state_changed", state =>
               state->StoreData.Decode.playerState->PlayerStateChange->dispatch
             );
          player |> addListener("ready", state => PlayerReady(state->device_idGet)->dispatch);
          player |> addListener("not_ready", state => state->device_idGet->Js.log2("not ready"));
          player |> connect() |> ignore;
        },
      );

      let scriptTag = Element.createElement(Element.document, "script");
      Element.setSrc(scriptTag, "https://sdk.scdn.co/spotify-player.js");
      Element.appendToHead(Element.document, scriptTag);
    };
  };
};