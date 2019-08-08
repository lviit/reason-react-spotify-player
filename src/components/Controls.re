open StoreProvider;
open Utils;

module Styles = {
  open Css;

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
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {progress, playing}: StoreData.player = state.player;

  let handleProgressChange = e => {
    let value = e->ReactEvent.Synthetic.target##value->int_of_string;
    Seek(value)->dispatch;
  };
  
  let duration =
    switch (state.currentTrack) {
    | None => 0
    | Some(currentTrack) => currentTrack.duration_ms
    };

  <div className=Styles.controls>
    <input
      className=Styles.progressBar
      type_="range"
      name="progress"
      min=0
      max={duration->string_of_int}
      value={progress->string_of_int}
      onChange=handleProgressChange
      ariaLabel="progress"
    />
    <span className=Styles.progress> {progress->formatDuration->ReasonReact.string} </span>
    <Button icon=Button.Prev action={_ => dispatch(Prev)} size="32" />
    <Button
      icon={playing ? Button.Pause : Button.Play}
      action={_ => dispatch(playing ? Pause : Play)}
    />
    <Button icon=Button.Next action={_ => dispatch(Next)} size="32" />
  </div>;
};