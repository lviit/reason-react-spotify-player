open Utils;
open SettingsProvider;

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
  let (_, dispatch) = React.useContext(settingsContext);
  let image = List.hd(images);
  let artist = List.hd(artists);

  <div
    className={Styles.container(image)}
    key=id
    onClick={_event => dispatch(Play(uri))}>
    <div className=Styles.info>
      <h2 className=Styles.title> {React.string(name)} </h2>
      <span> {React.string(artist.name)} </span>
    </div>
  </div>;
};
