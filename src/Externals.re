type win;
[@bs.val] external window: win = "";

module URLSearchParams = {
  type t;
  [@bs.new] external make: string => t = "URLSearchParams";
  [@bs.return nullable] [@bs.send.pipe: t] external get: string => option(string) = "";
};

module Location = {
  [@bs.set] [@bs.scope "location"] external setHash: (win, string) => unit = "hash";
  [@bs.get] [@bs.scope "location"] external getHash: win => string = "hash";
};

module Element = {
  type document;
  [@bs.send] external createElement: (document, string) => Dom.element = "";
  [@bs.val] external document: document = "document";
  [@bs.set] external setSrc: (Dom.element, string) => unit = "src";
  [@bs.send] [@bs.scope "head"]
  external appendToHead: (document, Dom.element) => unit = "appendChild";
  [@bs.set] [@bs.scope "body"] external bodyStyle: (document, string) => unit = "style";
};

module Spotify = {
  [@bs.deriving abstract]
  type listener = {
    message: string,
    device_id: string,
  };

  type player;
  [@bs.send.pipe: player] external connect: unit => player = "connect";
  [@bs.send.pipe: player] external addListener: (string, listener => unit) => unit = "addListener";
  [@bs.send.pipe: player]
  external addStateListener: (string, Js.Json.t => unit) => unit = "addListener";

  [@bs.deriving abstract]
  type settings = {
    name: string,
    getOAuthToken: (string => unit) => unit,
  };

  [@bs.scope "Spotify"] [@bs.new] external createSpotifyPlayer: settings => player = "Player";

  [@bs.set] external onSpotifyWebPlaybackSDKReady: (win, unit => unit) => unit = "";
};