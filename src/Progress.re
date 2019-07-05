open StoreProvider;

module Styles = {
  open Css;

  let progress =
    style([
      fontSize(px(18)),
      letterSpacing(px(1)),
    ]);
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(storeContext);
  let minutes = string_of_int(state.progress / 60);
  let seconds = progress =>
    switch (progress) {
    | progress when progress mod 60 < 10 => "0" ++ string_of_int(progress mod 60)
    | progress when progress >= 60 => string_of_int(progress mod 60)
    | _ => string_of_int(progress)
    };

  <span className=Styles.progress>
    {ReasonReact.string(" - " ++ minutes ++ ":" ++ seconds(state.progress))}
  </span>;
};