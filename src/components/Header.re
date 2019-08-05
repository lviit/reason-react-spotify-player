open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      fontSize(px(20)),
      color(hex("fff")),
      position(`fixed),
      width(`percent(100.0)),
      zIndex(10),
      backgroundColor(hex("B03D32")),
      display(`flex),
      justifyContent(`spaceBetween),
      alignItems(`center),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(12, 38, 69, 0.3)),
    ]);

  let title = currentTrack =>
    style([
      fontSize(px(20)),
      position(`absolute),
      textTransform(`uppercase),
      margin(px(0)),
      marginLeft(px(5)),
      transform(translateY(`percent(Belt.Option.isSome(currentTrack) ? 100.0 : 0.0))),
      transition(
        ~duration=500,
        ~delay=0,
        ~timingFunction=cubicBesier(0.15, 1.0, 0.33, 1.0),
        "transform",
      ),
    ]);

  let trackInfo = currentTrack =>
    style([
      position(`absolute),
      transform(translateY(`percent(Belt.Option.isSome(currentTrack) ? 0.0 : (-100.0)))),
      transition(
        ~duration=500,
        ~delay=0,
        ~timingFunction=cubicBesier(0.15, 1.0, 0.33, 1.0),
        "transform",
      ),
    ]);

  let nowPlaying =
    style([
      paddingLeft(px(10)),
      cursor(`pointer),
      position(`relative),
      height(px(50)),
      overflow(`hidden),
      lineHeight(px(50)),
      flexGrow(1.0),
    ]);

  let song = style([fontWeight(`num(700))]);
  let right = style([display(`flex)]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);

  <div className=Styles.container>
    <div className=Styles.nowPlaying>
      <h1 className={Styles.title(state.currentTrack)}>
        {ReasonReact.string("Reason React Spotify Player")}
      </h1>
      <span className={Styles.trackInfo(state.currentTrack)}>
        {switch (state.currentTrack) {
         | None => ReasonReact.null
         | Some(({name, artists, album: {uri}}: StoreData.currentTrack)) =>
           let artist = List.hd(artists).name;
           let id = String.length(uri) > 0 ? String.sub(uri, 14, 22) : "";
           <span onClick={_ => FetchAlbumDetails(id)->dispatch}>
             {ReasonReact.string(artist ++ " - ")}
             <span className=Styles.song> {ReasonReact.string(name)} </span>
           </span>;
         }}
      </span>
    </div>
    <div className=Styles.right> <Controls /> /* <Visualizer /> */ </div>
  </div>;
};