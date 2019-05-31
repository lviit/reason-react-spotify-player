type state = {
  loading: bool,
  deviceId: string,
};

type action =
  | PlayerLoading
  | PlayerReady(string);


module Styles = {
  open Css;

  let main =
    style([
      padding3(~top=px(80), ~h=px(20), ~bottom=px(20))
    ]);

  global(
    "body",
    [
      fontFamily("IBM Plex Sans"),
      margin(px(0)),
      backgroundColor(hex("F6F4F4")),
    ],
  );
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | PlayerLoading => {...state, loading: true}
        | PlayerReady(id) => {loading: false, deviceId: id}
        },
      {loading: false, deviceId: ""},
    );

  React.useEffect1(
    () => {
      Js.log("effect!");
      dispatch(PlayerLoading);
      //initSpotifyPlayer();

      // TODO: handle with proper bindings
      %bs.raw
      {|
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
       |};

      Some(() => ());
    },
    [||],
  );

  <SettingsProvider>
    <Header />
    <div className=Styles.main> <Player /> <NewReleases /> </div>
  </SettingsProvider>;
};