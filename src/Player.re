open SettingsProvider;

module Styles = {
  open Css;

  let container =
    style([
      padding(px(10)),
      display(`flex),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(settingsContext);
  let text = "now playing" ++ state.data.name;

  <div className=Styles.container>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string("previous")}
    </button>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string(state.playing ? "pause" : "play")}
    </button>
    <button onClick={_event => dispatch(TogglePlay)}>
      {ReasonReact.string("next")}
    </button>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
    <h3> {ReasonReact.string(text)} </h3>
  </div>;
};
