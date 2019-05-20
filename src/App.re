open Webapi;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";
//[@bs.new] [@bs.scope ("Spotify")] external initSpotifyPlayer : unit => unit = "Player";

type state = {
  loading: bool,
  deviceId: string,
};

type action =
  | PlayerLoading
  | PlayerReady(string);

[@react.component]
let make = () => {
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

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | PlayerLoading => {...state, loading: true}
        | PlayerReady(id) => {...state, loading: false, deviceId: id}
        },
      {loading: false, deviceId: ""},
    );

  React.useEffect1(
    () => {
      Js.log("effect!");
      dispatch(PlayerLoading);
      //initSpotifyPlayer();

      // TODO: handle with proper bindings
      [%bs.raw {| 
        window.onSpotifyWebPlaybackSDKReady = () => {
          const url = new URLSearchParams(window.location.hash);
          const token = url.get("#access_token");
          const player = new Spotify.Player({
            name: 'Reason client test',
            getOAuthToken: cb => { cb(token); }
          });

          player.addListener('initialization_error', ({ message }) => { console.error(message); });
          player.addListener('authentication_error', ({ message }) => { console.error(message); });
          player.addListener('account_error', ({ message }) => { console.error(message); });
          player.addListener('playback_error', ({ message }) => { console.error(message); });
          player.addListener('player_state_changed', state => { console.log(state); });
          player.addListener('ready', ({ device_id }) => {
            console.log('Ready with Device ID', device_id);
            window.location.hash = window.location.hash + "&device_id=" + device_id;
          });
          player.addListener('not_ready', ({ device_id }) => {
            console.log('Device ID has gone offline', device_id);
          });
          player.connect().then(success => player.togglePlay());
        } 
       |}];

      Some(() => ());
    },
    [||],
  );
  <div> <Component2 authHeader /> <NewReleases authHeader deviceId /> </div>;
};