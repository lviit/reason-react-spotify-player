open StoreProvider;
open Utils;

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
      boxSizing(`borderBox),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(12, 38, 69, 0.3)),
    ]);

  let title = currentTrack =>
    style([
      position(`absolute),
      fontSize(20 |> px),
      textTransform(`uppercase),
      color(white),
      margin(px(0)),
      marginLeft(px(5)),
      fontWeight(`num(700)),
      letterSpacing(px(2)),
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
      letterSpacing(px(1)),
      paddingLeft(px(10)),
      cursor(`pointer),
      position(`relative),
      height(px(50)),
      overflow(`hidden),
      minWidth(px(450)),
      lineHeight(px(50)),
      flexGrow(1.0),
    ]);

  let progress = style([margin2(~v=`zero, ~h=px(10))]);
  let progressBar =
    style([
      width(px(200)),
      margin2(~v=`zero, ~h=px(10)),
      unsafe("-webkit-appearance", "none"),
      backgroundColor(hex("fff")),
      height(px(4)),
      borderRadius(px(2)),
      focus([outlineStyle(`none)]),
      selector(
        "::-webkit-slider-thumb",
        [
          unsafe("-webkit-appearance", "none"),
          backgroundColor(hex("fff")),
          height(px(16)),
          width(px(16)),
          borderRadius(px(8)),
        ],
      ),
    ]);

  let controls = style([display(`flex), alignItems(`center), margin2(~v=`zero, ~h=px(10))]);
  let song = style([fontWeight(`num(700))]);
  let right = style([display(`flex)]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {progress, playing}: StoreData.player = state.player;

  let handleProgressChange = e => {
    let value = e->ReactEvent.Synthetic.target##value->int_of_string;
    Seek(value)->dispatch;
  };

  let {name, duration_ms, artists, album: {uri}}: StoreData.currentTrack =
    switch (state.currentTrack) {
    | None => {
        name: "",
        id: "",
        duration_ms: 0,
        artists: [{name: ""}],
        album: {
          uri: "",
          name: "",
        },
      }
    | Some(currentTrack) => currentTrack
    };

  let artist = List.hd(artists).name;
  let id = String.length(uri) > 0 ? String.sub(uri, 14, 22) : "";

  <div className=Styles.container>
    <div className=Styles.nowPlaying onClick={_ => FetchAlbumDetails(id)->dispatch}>
      <h1 className={Styles.title(state.currentTrack)}>
        {ReasonReact.string("Reason Spotify client")}
      </h1>
      <span className={Styles.trackInfo(state.currentTrack)}>
        <span> {ReasonReact.string(artist ++ " - ")} </span>
        <span className=Styles.song> {ReasonReact.string(name)} </span>
      </span>
    </div>
    <div className=Styles.right>
      <div className=Styles.controls>
        <span className=Styles.progress> {progress->formatDuration->ReasonReact.string} </span>
        <input
          className=Styles.progressBar
          type_="range"
          name="progress"
          min=0
          max={duration_ms->string_of_int}
          value={progress->string_of_int}
          onChange=handleProgressChange
        />
        <Button icon=Button.Prev action={_ => dispatch(Prev)} size="32" />
        <Button
          icon={playing ? Button.Pause : Button.Play}
          action={_ => dispatch(playing ? Pause : Play)}
        />
        <Button icon=Button.Next action={_ => dispatch(Next)} size="32" />
      </div>
      <Visualizer />
    </div>
  </div>;
};