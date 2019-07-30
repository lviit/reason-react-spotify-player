open StoreProvider;
open Utils;

module Styles = {
  open Css;

  let track =
    style([
      padding2(~v=px(15), ~h=px(0)),
      borderTop(px(2), `solid, hex("eee")),
      hover([cursor(`pointer)]),
      display(`flex),
      justifyContent(`spaceBetween),
      position(`relative),
    ]);

  let trackNumber = style([width(px(35)), flexShrink(0.0)]);
  let trackDuration = style([marginLeft(px(10))]);
  let trackName = style([flexGrow(1.0)]);

  let playButton = (isPlaying: bool, hover: bool) =>
    style([
      backgroundColor(white),
      position(`absolute),
      top(px(isPlaying ? 14 : 10)),
      transform(translateX(px(isPlaying ? (-12) : hover ? (-17) : (-70)))),
      transition(~duration=200, ~delay=0, ~timingFunction=`easeInOut, "transform"),
    ]);
};

[@react.component]
let make = (~track as {name, uri, id, track_number, duration_ms}: StoreData.track, ~albumUri) => {
  let (state, dispatch) = React.useContext(storeContext);
  let (hover, setHover) = React.useState(() => false);
  let isPlaying =
    switch (state.currentTrack) {
    | None => false
    | Some(currentTrack) => currentTrack.id === id
    };

  <span
    className=Styles.track
    onMouseEnter={_ => setHover(_ => true)}
    onMouseLeave={_ => setHover(_ => false)}
    onClick={_ => PlaySong(uri, albumUri)->dispatch}>
    <span className={Styles.playButton(isPlaying, hover)}>
      {isPlaying
         ? <Button icon=Button.Volume color=Button.Dark action={_ => ()} size="24" />
         : <Button icon=Button.Play color=Button.Dark action={_ => ()} size="32" />}
    </span>
    <span className=Styles.trackNumber>
      {ReasonReact.string(track_number->string_of_int ++ ".")}
    </span>
    <span className=Styles.trackName> {ReasonReact.string(name)} </span>
    <span className=Styles.trackDuration>
      {ReasonReact.string(duration_ms->formatDuration)}
    </span>
  </span>;
};