module Styles = {
  open Css;

  let progress =
    style([
      fontSize(px(18)),
      letterSpacing(px(1)),
    ]);
};

type action =
  | Increment;

type state = {progress: int};

let reducer = (state, action) =>
  switch (action) {
  | Increment => {progress: state.progress + 1}
  };

let initialState = {progress: 0};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  React.useEffect1(
    () => {
      Js.Global.setInterval(() => dispatch(Increment), 1000) |> ignore;
      Some(() => ());
    },
    [||],
  );

  let minutes = string_of_int(state.progress / 60);
  let seconds = progress =>
    switch (progress) {
    | progress when progress mod 60 < 10 => "0" ++ string_of_int(progress)
    | progress when progress >= 60 => string_of_int(progress mod 60)
    | _ => string_of_int(progress)
    };

  <span className=Styles.progress>
    {ReasonReact.string(" - " ++ minutes ++ ":" ++ seconds(state.progress))}
  </span>;
};