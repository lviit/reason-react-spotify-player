open SettingsProvider;
module Styles = {
  open Css;

  let container =
    style([
      padding(px(10)),
      display(`flex),
      color(hex("fff")),
      alignItems(`center),
    ]);

  let nowPlaying =
    style([
      margin2(~v=px(0), ~h=px(20)),
      fontSize(px(18)),
      letterSpacing(px(1)),
    ]);

  let song = style([fontWeight(`num(700))]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(settingsContext);
  let artist =
    switch (state.data.artists) {
    | [] => ""
    | [a, ..._] => a.name
    };

  <div className=Styles.container>
    <Button icon=Button.Prev action={_ => dispatch(TogglePlay)} />
    <Button icon={state.playing ? Button.Pause : Button.Play} action={_ => dispatch(TogglePlay)} />
    <Button icon=Button.Next action={_ => dispatch(TogglePlay)} />
    <div className=Styles.nowPlaying>
      <span> {ReasonReact.string(artist ++ " - ")} </span>
      <span className=Styles.song>
        {ReasonReact.string(state.data.name)}
      </span>
      <span> {state.data.progress_ms |> string_of_int |> ReasonReact.string} </span>
    </div>
  </div>;
};