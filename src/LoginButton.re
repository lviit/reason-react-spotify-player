open Utils;

module Styles = {
  open Css;

  let button =
    style([
      padding2(px(5), px(20)),
      fontSize(16 |> px),
      backgroundColor(hex("B03D32")),
      letterSpacing(px(1)),
      color(white),
      textDecoration(none),
      textAlign(center),
      display(`flex),
      alignItems(`center),
      fontWeight(`num(600)),
      hover([backgroundColor(hex("fff")), color(hex("DB4D3F"))]),
      //backgroundColor(hex("DB4D3F")),
    ]);
};

[@react.component]
let make = () => {
  let authEndpoint = "https://accounts.spotify.com/authorize";
  let clientId = "64d03692241b478cb763ec2a7eed99e0";
  let redirectUri = "http://localhost:8000";
  let scopes = [
    "user-read-currently-playing",
    "user-read-playback-state",
    "streaming",
    "user-read-birthdate",
    "user-read-email",
    "user-read-private",
  ];

  <a
    className=Styles.button
    href={
      authEndpoint
      ++ "?client_id="
      ++ clientId
      ++ "&redirect_uri="
      ++ redirectUri
      ++ "&scope="
      ++ join("%20", scopes)
      ++ "&response_type=token&show_dialog=true"
    }>
    {ReasonReact.string("Login to Spotify")}
  </a>;
};