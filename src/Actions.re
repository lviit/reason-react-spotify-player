open Request;
open Externals;
open StoreData;
open Js.Promise;
open Js.Global;

let stopTimer = progressTimer =>
  switch (progressTimer) {
  | None => ()
  | Some(progressTimer) => clearInterval(progressTimer)
  };

let startTimer = dispatch => setInterval(_ => IncrementProgress->dispatch, 1000);

let action = (dispatch, state, actionType: actionType) => {
  let {deviceId, accessToken, progressTimer} = state.player;

  switch (actionType) {
  | PlaySong(trackUri, contextUri) =>
    progressTimer->stopTimer;
    let timerId = startTimer(dispatch);
    playSong(trackUri, contextUri, deviceId, accessToken)
    |> then_(_ => timerId->Play->dispatch->resolve)
    |> ignore;
  | Prev => request(Previous, accessToken) |> ignore
  | Next => request(Next, accessToken) |> ignore
  | Play =>
    let timerId = startTimer(dispatch);
    request(Play, accessToken) |> then_(_ => timerId->Play->dispatch->resolve) |> ignore;
  | Pause =>
    progressTimer->stopTimer;
    request(Pause, accessToken) |> then_(_ => Pause->dispatch->resolve) |> ignore;
  | FetchNewReleases =>
    FetchAlbumsPending->dispatch;
    request(NewReleases, accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->Decode.albums->FetchAlbumsFulfilled->dispatch->resolve)
    |> ignore;
  | Search(keywords) =>
    FetchAlbumsPending->dispatch;
    request(keywords->Search, accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->Decode.albums->FetchAlbumsFulfilled->dispatch->resolve)
    |> ignore;
  | FetchAlbumDetails(id) =>
    OpenAlbumDetails->dispatch;
    FetchAlbumDetailsPending->dispatch;
    request(AlbumDetails(id), accessToken)
    |> then_(Fetch.Response.json)
    |> then_(json => json->Decode.albumDetails->FetchAlbumDetailsFulfilled->dispatch->resolve)
    |> ignore;
  | CloseAlbumDetails =>
    CloseAlbumDetails->dispatch;
  | Seek(progress) =>
    request(progress->Seek, accessToken)
    |> then_(_ => progress->Seek->dispatch->resolve)
    |> ignore
  | LoadPlayer =>
    ServiceWorker.register(navigator, "service-worker.js");
    let params = window->Location.getHash->URLSearchParams.make;
    let accessToken =
      switch (URLSearchParams.get("#access_token", params)) {
      | None => Dom.Storage.getItem("accessToken", Dom.Storage.localStorage)
      | Some(accessToken) => Some(accessToken)
      };

    let expireDate =
      switch (URLSearchParams.get("expires_in", params)) {
      | None =>
        switch (Dom.Storage.getItem("expireDate", Dom.Storage.localStorage)) {
        | None => 0.0
        | Some(expireDate) => expireDate->float_of_string
        }
      | Some(expireDate) => Js.Date.now() +. expireDate->float_of_string *. 1000.0
      };

    switch (accessToken) {
    | None => Js.log("No access token available")
    | Some(accessToken) =>
      Js.Date.now() > expireDate
        ? Js.log("access token expired")
        : {
          Dom.Storage.setItem("accessToken", accessToken, Dom.Storage.localStorage);
          Dom.Storage.setItem(
            "expireDate",
            expireDate->Js.Float.toString,
            Dom.Storage.localStorage,
          );
          Location.setHash(window, "");
          PlayerLoading(accessToken) |> dispatch;
          Spotify.(
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
                     state->Decode.playerState->PlayerStateChange->dispatch
                   );
                player
                |> addListener("ready", state => PlayerReady(state->device_idGet)->dispatch);
                player
                |> addListener("not_ready", state => state->device_idGet->Js.log2("not ready"));
                player |> connect() |> ignore;
              },
            )
          );

          let scriptTag = Element.createElement(Element.document, "script");
          Element.setSrc(scriptTag, "https://sdk.scdn.co/spotify-player.js");
          Element.appendToHead(Element.document, scriptTag);
        }
    };
  };
};