open StoreProvider;

module Styles = {
  open Css;

  let container = albumDetailsOpen =>
    style([
      position(`fixed),
      right(`zero),
      bottom(`zero),
      top(px(50)),
      transform(translateX(`percent(albumDetailsOpen ? 0.0 : 100.0))),
      transition(
        ~duration=500,
        ~delay=0,
        ~timingFunction=cubicBesier(0.15, 1.0, 0.33, 1.0),
        "transform",
      ),
      boxShadow(~x=px(0), ~y=px(0), ~blur=px(10), rgba(12, 38, 69, 0.3)),
      zIndex(1),
      overflow(`auto),
      width(px(400)),
      backgroundColor(hex("fff")),
    ]);

  let infoContainer =
    style([display(`flex), flexDirection(`column), padding2(~v=px(15), ~h=px(25))]);
  let albumTitle = style([fontSize(px(32)), margin(`zero), lineHeight(px(34))]);
  let artistName =
    style([
      fontSize(px(20)),
      margin3(~top=px(5), ~h=`zero, ~bottom=px(30)),
      fontWeight(`num(300)),
    ]);
  let image = style([width(`percent(100.0))]);
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(storeContext);
  switch (state.albumDetails) {
  | None => <div className={Styles.container(false)} />
  | Some(albumDetails) =>
    let {images, artists, name, uri, tracks, release_date}: StoreData.albumDetails = albumDetails;
    let image = List.hd(images).url;
    let artist = List.hd(artists).name;
    let year = String.sub(release_date, 0, 4);

    <div className={Styles.container(state.albumDetailsOpen)}>
      <img className=Styles.image src=image alt=name />
      <div className=Styles.infoContainer>
        <h2 className=Styles.albumTitle> {React.string(name)} </h2>
        <span className=Styles.artistName> {React.string(artist ++ " (" ++ year ++ ")")} </span>
        {List.map(track => <Track track albumUri=uri key={track.uri} />, tracks.items)
         ->Array.of_list
         ->ReasonReact.array}
      </div>
    </div>;
  };
};