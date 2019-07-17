open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      position(`absolute),
      right(px(0)),
      top(px(90)),
      zIndex(1),
      width(px(300)),
      backgroundColor(hex("fff")),
    ]);

  let infoContainer =
    style([display(`flex), flexDirection(`column), padding(px(30))]);

  let track =
    style([
      padding2(~v=px(15), ~h=px(0)),
      letterSpacing(px(1)),
      borderBottom(px(2), `solid, hex("eee")),
      hover([cursor(`pointer)]),
    ]);

  let title = style([fontSize(px(24))]);

  let image = style([width(`percent(100.0))]);
};

module Track = {
  [@react.component]
  let make =
      (~track as {name, uri, track_number, duration_ms}: AlbumData.track, ~albumUri) => {
    let (_, dispatch) = React.useContext(storeContext);

    <span className=Styles.track onClick={_ => PlaySong(uri, albumUri)->dispatch}>
      {ReasonReact.string(
         string_of_int(track_number)
         ++ ". "
         ++ name
         ++ string_of_int(duration_ms),
       )}
    </span>;
  };
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(storeContext);
  let image =
    switch (state.albumDetails.images) {
    | [] => ""
    | [head, ..._] => head.url
    };
  let artist =
    switch (state.albumDetails.artists) {
    | [] => ""
    | [head, ..._] => head.name
    };

  <div className=Styles.container>
    <img className=Styles.image src=image alt={state.albumDetails.name} />
    <div className=Styles.infoContainer>
      <h2 className=Styles.title>
        {React.string(state.albumDetails.name)}
      </h2>
      <span> {React.string(artist)} </span>
      {List.map(track => <Track track albumUri={state.albumDetails.uri} />, state.albumDetails.tracks.itemss)
       ->Array.of_list
       ->ReasonReact.array}
    </div>
  </div>;
};