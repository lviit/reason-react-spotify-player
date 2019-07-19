open StoreProvider;
open Utils;

module Styles = {
  open Css;

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
  let {data: {name, duration_ms, artists}, progress, playing}: StoreData.state = state;

  let artist =
    switch (artists) {
    | [] => ""
    | [a, ..._] => a.name
    };

  let handleProgressChange = e => {
    let value = e->ReactEvent.Synthetic.target##value->int_of_string;
    Seek(value)->dispatch;
  };

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
        type_="range"
        value={progress->string_of_int}
        name="progress"
        min=0
        max={duration_ms->string_of_int}
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