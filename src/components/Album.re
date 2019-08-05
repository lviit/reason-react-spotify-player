open StoreProvider;

module Styles = {
  open Css;
  open Styles;

  let container =
    style([
      backgrounds([]),
      hover([
        selector("img", [transform(`scale((1.10, 1.10))), filter([`saturate(100.0)])]),
      ]),
      backgroundPosition(`percent(50.0), `percent(50.0)),
      color(colorFontLight |> hex),
      position(`relative),
      cursor(`pointer),
      overflow(`hidden),
    ]);

  let image(ready) =
    style([
      width(`percent(100.0)),
      filter([`saturate(50.0)]),
      opacity(ready ? 1.0 : 0.0),
      verticalAlign(`middle),
      transition(
        ~duration=500,
        ~delay=0,
        ~timingFunction=cubicBesier(0.15, 1.0, 0.33, 1.0),
        "all",
      ),
    ]);

  let title =
    style([
      marginTop(0 |> px),
      marginBottom(5 |> px),
      fontSize(22 |> px),
      fontWeight(`num(600)),
      whiteSpace(`nowrap),
      overflow(`hidden),
      textOverflow(`ellipsis),
    ]);

  let info =
    style([
      position(`absolute),
      bottom(`zero),
      left(`zero),
      right(`zero),
      top(`zero),
      display(`flex),
      flexDirection(`column),
      justifyContent(`flexEnd),
      padding(20 |> px),
      backgrounds([
        radialGradient([
          (`percent(50.0), rgba(0, 0, 0, 0.0)),
          (`percent(100.0), rgba(0, 0, 0, 0.4)),
        ]),
        linearGradient(
          deg(0.0),
          [(`percent(0.0), rgba(0, 0, 0, 0.8)), (`percent(50.0), rgba(0, 0, 0, 0.0))],
        ),
      ]),
    ]);
};

[@react.component]
let make = (~album as {name, id, images, artists}: StoreData.album) => {
  let (_, dispatch) = React.useContext(storeContext);
  let (imgReady, setImgReady) = React.useState(_ => false);
  let image = List.hd(images);
  let artist = List.hd(artists);

  <div className=Styles.container onClick={_ => FetchAlbumDetails(id)->dispatch}>
    <img className=Styles.image(imgReady) onLoad={_ => setImgReady(_ => true)} src={image.url} alt=name />
    <div className=Styles.info>
      <h2 className=Styles.title> {React.string(name)} </h2>
      <span> {React.string(artist.name)} </span>
    </div>
  </div>;
};