open SettingsProvider;

module Styles = {
  open Css;

  let title = style([fontSize(px(60))]);

  let container =
    style([
      display(`grid),
      gridAutoRows(px(300)),
      gridTemplateColumns([px(300), px(300), px(300)]),
      gridGap(px(30)),
    ]);

  global("body", [fontFamily("IBM Plex Sans")]);
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(settingsContext);

  <div>
    <h1 className=Styles.title> {ReasonReact.string("new releases")} </h1>
    <div className=Styles.container>
      {List.length(state.albumData) > 0
         ? List.map(album => <Album album authHeader deviceId />, state.albumData)
           |> Array.of_list
           |> ReasonReact.array
         : ReasonReact.string("No data available.")}
    </div>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
  </div>;
};