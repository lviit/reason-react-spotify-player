open StoreProvider;
open Utils;

module Styles = {
  open Css;

  let track =
    style([
      padding2(~v=px(15), ~h=px(0)),
      letterSpacing(px(1)),
      borderTop(px(2), `solid, hex("eee")),
      hover([cursor(`pointer)]),
      display(`flex),
      justifyContent(`spaceBetween),
      position(`relative),
    ]);

  let trackNumber = style([width(px(35)), flexShrink(0.0)]);
  let trackDuration = style([marginLeft(px(10))]);
  let trackName = style([flexGrow(1.0)]);

  let playButton = (hover: bool) =>
    style([
      backgroundColor(white),
      position(`absolute),
      top(px(7)),
      transform(translateX(px(hover ? (-20) : (-70)))),
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
    <span className={Styles.playButton(isPlaying ? true : hover)}>
      <Button icon={isPlaying ? Button.Volume : Button.Play} color=Button.Dark action={_ => ()} />
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