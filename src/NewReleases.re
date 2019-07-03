open StoreProvider;

module Styles = {
  open Css;

  let title =
    style([
      fontSize(px(60)),
      margin(px(100)),
      fontWeight(`num(700)),
      letterSpacing(px(1)),
      textAlign(`center),
    ]);

  let container =
    style([
      display(`grid),
      gridAutoRows(px(300)),
      gridTemplateColumns([
        `percent(25.0),
        `percent(25.0),
        `percent(25.0),
        `percent(25.0),
      ]),
      //gridTemplateColumns(repeat(12, minmax(px(100), fr(1)))),
      //grid-template-columns: repeat(auto-fit, minmax(100px, 1fr));
      gridGap(px(3)),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);

  React.useEffect1(
    () => {
      dispatch(FetchAlbumDataPending);
      Some(() => ());
    },
    [||],
  );

  <div>
    <h1 className=Styles.title> {ReasonReact.string("New releases")} </h1>
    <div className=Styles.container>
      {List.length(state.albumData) > 0
         ? List.map(album => <Album album />, state.albumData)
           |> Array.of_list
           |> ReasonReact.array
         : ReasonReact.string("No data available.")}
    </div>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
  </div>;
};