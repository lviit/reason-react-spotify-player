open StoreProvider;
open Utils;

module Styles = {
  open Css;

  let container = albumDetailsOpen =>
    style([
      position(`fixed),
      right(`zero),
      bottom(`zero),
      top(px(90)),
      transform(translateX(`percent(albumDetailsOpen ? 0.0 : 100.0))),
      transition(~duration=200, ~delay=0, ~timingFunction=`easeInOut, "transform"),
      boxShadow(~x=px(0), ~y=px(0), ~blur=px(10), rgba(12, 38, 69, 0.3)),
      zIndex(1),
      overflow(`auto),
      width(px(400)),
      backgroundColor(hex("fff")),
    ]);

  let infoContainer = style([display(`flex), flexDirection(`column), padding(px(25))]);

  let track =
    style([
      padding2(~v=px(15), ~h=px(0)),
      letterSpacing(px(1)),
      borderTop(px(2), `solid, hex("eee")),
      hover([cursor(`pointer)]),
    ]);

  let albumTitle = style([fontSize(px(24)), margin(`zero)]);
  let artistName = style([fontSize(px(18)), marginBottom(px(20))]);
  let image = style([width(`percent(100.0))]);
};

module Track = {
  [@react.component]
  let make = (~track as {name, uri, track_number, duration_ms}: AlbumData.track, ~albumUri) => {
    let (_, dispatch) = React.useContext(storeContext);

    <span className=Styles.track onClick={_ => PlaySong(uri, albumUri)->dispatch}>
      {ReasonReact.string(
         track_number->string_of_int ++ ". " ++ name ++ " - " ++ duration_ms->formatDuration,
       )}
    </span>;
  };
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(storeContext);
  switch (state.albumDetails) {
  | None => <div className={Styles.container(false)} />
  | Some(albumDetails) =>
    let {images, artists, name, uri, tracks}: AlbumData.albumDetails = albumDetails;
    let image = List.hd(images).url;
    let artist = List.hd(artists).name;

    <div className={Styles.container(state.albumDetailsOpen)}>
      <img className=Styles.image src=image alt=name />
      <div className=Styles.infoContainer>
        <h2 className=Styles.albumTitle> {React.string(name)} </h2>
        <span className=Styles.artistName> {React.string(artist)} </span>
        {List.map(track => <Track track albumUri=uri key={track.uri} />, tracks.items)
         ->Array.of_list
         ->ReasonReact.array}
      </div>
    </div>;
  };
};