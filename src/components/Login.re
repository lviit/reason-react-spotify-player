open Utils;

module Styles = {
  open Css;

  let button =
    style([
      padding2(~v=px(10), ~h=px(10)),
      margin3(~top=px(40), ~h=`auto, ~bottom=`zero),
      fontSize(px(20)),
      backgroundColor(hex("B03D32")),
      color(white),
      textDecoration(none),
      display(`flex),
      justifyContent(`center),
      fontWeight(`num(600)),
      maxWidth(px(200)),
    ]);

  let container =
    style([
      position(`absolute),
      zIndex(1),
      top(`percent(50.0)),
      left(`percent(50.0)),
      transform(translate(`percent(-50.0), `percent(-50.0))),
      padding(px(40)),
      backgroundColor(hex("fff")),
      minWidth(px(400)),
      boxShadow(~blur=px(64), hex("000")),
    ]);
};

[@react.component]
let make = () => {
  let authEndpoint = "https://accounts.spotify.com/authorize";
  let clientId = "64d03692241b478cb763ec2a7eed99e0";
  let redirectUri = "https://lviit.github.io/reason-react-spotify-player";
  //let redirectUri = "http://localhost:8000";
  let scopes = ["user-read-currently-playing", "user-read-playback-state", "streaming"];

  <div className=Styles.container>
    <h2> {ReasonReact.string("Reason React Spotify Player")} </h2>
    <p>
      {ReasonReact.string("Spotify player built with ReasonML. Have a look at ")}
      <a target="_blank" href="https://github.com/lviit/reason-react-spotify-player">
        {ReasonReact.string("github.")}
      </a>
    </p>
    <p>
      {ReasonReact.string(
         "Uses the Spotify Web API, which requires the application to be authorized by the user to access playback information.",
       )}
    </p>
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
        ++ "&response_type=token"
      }>
      {ReasonReact.string("Authorize")}
    </a>
  </div>;
};