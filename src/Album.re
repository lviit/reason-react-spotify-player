open Utils;

module Styles = {
  open Css;
  open Styles;

  let container = (image: AlbumData.image) =>
    style([
      backgroundImage(`url(image.url)),
      backgroundColor(colorBg |> hex),
      backgroundSize(`cover),
      color(colorFontLight |> hex),
      position(`relative),
      display(`flex),
      flexDirection(`column),
      justifyContent(`flexEnd),
      cursor(`pointer),
    ]);

  let title =
    style([
      marginTop(0 |> px),
      marginBottom(10 |> px),
      fontSize(24 |> px),
      //textTransform(`uppercase),
    ]);

  let info =
    style([
      padding(20 |> px),
      background(
        linearGradient(
          deg(0),
          [(0, rgba(0, 0, 0, 1.0)), (100, rgba(0, 0, 0, 0.0))],
        ),
      ),
    ]);
};

[@react.component]
let make =
    (
      ~album as {name, id, images, artists, uri}: AlbumData.album,
      ~authHeader,
      ~deviceId,
    ) => {
  let (play, setPlay) = React.useState(() => "");

  React.useEffect1(
    () => {

    if (play !== "") {
        let payload = Js.Dict.empty();
        Js.Dict.set(payload, "context_uri", Js.Json.string(play));

        Js.Promise.(
          putWithAuth(
            "https://api.spotify.com/v1/me/player/play?device_id=" ++ deviceId,
            authHeader,
            payload,
          )
          |> then_(resolve)
        )
        |> ignore;
    }
      Some(() => ());
    },
    [|play|],
  );

  let image = List.hd(images);
  let artist = List.hd(artists);

  <div
    className={Styles.container(image)}
    key=id
    onClick={_event => setPlay(_ => uri)}>
    <div className=Styles.info>
      <h2 className=Styles.title> {React.string(name)} </h2>
      <span> {React.string(artist.name)} </span>
    </div>
  </div>;
};