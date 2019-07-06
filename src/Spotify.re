open Request;

[@bs.deriving abstract]
type listener = {
  message: string,
  device_id: string,
};

type player;
[@bs.send.pipe: player] external connectPlayer: unit => player = "connect";
[@bs.send.pipe: player]
external addListener: (string, listener => unit) => unit = "addListener";

[@bs.deriving abstract]
type settings = {
  name: string,
  getOAuthToken: (string => unit) => unit,
};

[@bs.scope "Spotify"] [@bs.new]
external createSpotifyPlayer: settings => player = "Player";

type win;

[@bs.val]
external window: win = "";

[@bs.set]
external onSpotifyWebPlaybackSDKReady: (win, unit => unit) => unit = "";

[@bs.set] [@bs.scope "location"]
external setLocationHash: (win, string) => unit = "hash";

[@bs.get] [@bs.scope "location"]
external getLocationHash: win => string = "hash";
