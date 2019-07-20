open StoreProvider;
open Utils;

module Styles = {
  open Css;

  let progressBar =
    style([
      width(px(200)),
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

  let container =
    style([
      padding2(~v=px(0), ~h=px(10)),
      display(`flex),
      flexGrow(1.0),
      color(hex("fff")),
      alignItems(`center),
      justifyContent(`spaceBetween),
    ]);

  let nowPlaying = style([fontSize(px(18)), letterSpacing(px(1))]);
  let controls = style([display(`flex), alignItems(`center)]);
  let song = style([fontWeight(`num(700))]);
  let progress = style([fontSize(px(18)), letterSpacing(px(1))]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {progress, playing}: StoreData.player = state.player;

  let handleProgressChange = e => {
    let value = e->ReactEvent.Synthetic.target##value->int_of_string;
    Seek(value)->dispatch;
  };

  switch (state.currentTrack) {
  | None => ReasonReact.null
  | Some(currentTrack) =>
    let {name, duration_ms, artists}: StoreData.currentTrack = currentTrack;
    let artist = List.hd(artists).name;
    <div className=Styles.container>
      <div className=Styles.nowPlaying>
        <span> {ReasonReact.string(artist ++ " - ")} </span>
        <span className=Styles.song> {ReasonReact.string(name)} </span>
        <span className=Styles.progress>
          {ReasonReact.string(" - " ++ formatDuration(progress))}
        </span>
      </div>
      <div className=Styles.controls>
        <input
          className=Styles.progressBar
          type_="range"
          name="progress"
          min=0
          max={duration_ms->string_of_int}
          value={progress->string_of_int}
          onChange=handleProgressChange
        />
        <Button icon=Button.Prev action={_ => dispatch(Prev)} />
        <Button
          icon={playing ? Button.Pause : Button.Play}
          action={_ => dispatch(playing ? Pause : Play)}
        />
        <Button icon=Button.Next action={_ => dispatch(Next)} />
      </div>
    </div>;
  };
};