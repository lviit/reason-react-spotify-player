open StoreProvider;

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

  let nowPlaying =
    style([
      fontSize(px(18)),
      letterSpacing(px(1)),
    ]);

  let controls = style([display(`flex), alignItems(`center)]);

  let song = style([fontWeight(`num(700))]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let artist =
    switch (state.data.artists) {
    | [] => ""
    | [a, ..._] => a.name
    };

  <div className=Styles.container>
    <div className=Styles.nowPlaying>
      <span> {ReasonReact.string(artist ++ " - ")} </span>
      <span className=Styles.song>
        {ReasonReact.string(state.data.name)}
      </span>
      <Progress />
    </div>
    <div className=Styles.controls>
      <Button icon=Button.Prev action={_ => dispatch(TogglePlay)} />
      <Button
        icon={state.playing ? Button.Pause : Button.Play}
        action={_ => dispatch(TogglePlay)}
      />
      <Button icon=Button.Next action={_ => dispatch(TogglePlay)} />
    </div>
  </div>;
};