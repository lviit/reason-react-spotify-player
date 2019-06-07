open StoreProvider;

module Styles = {
  open Css;
  open Styles;

  let container = (image: AlbumData.image) =>
    style([
      backgrounds([
        radialGradient(
          [(50, rgba(0, 0, 0, 0.0)), (100, rgba(0, 0, 0, 0.3))],
        ),
        `url(image.url)
      ]),
      backgroundSize(`cover),
      backgroundColor(colorBg |> hex),
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
    ) => {
  let (_, dispatch) = React.useContext(storeContext);
  let image = List.hd(images);
  let artist = List.hd(artists);

  <div
    className={Styles.container(image)}
    onClick={_event => dispatch(Play(uri))}>
    <div className=Styles.info>
      <h2 className=Styles.title> {React.string(name)} </h2>
      <span> {React.string(artist.name)} </span>
    </div>
  </div>;
};
