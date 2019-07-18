open StoreProvider;

module Styles = {
  open Css;

  let title =
    style([
      fontSize(px(60)),
      margin(px(50)),
      fontWeight(`num(700)),
      letterSpacing(px(1)),
      textAlign(`center),
    ]);

  let container =
    style([
      display(`grid),
      gridAutoRows(px(300)),
      gridTemplateColumns([`percent(25.0), `percent(25.0), `percent(25.0), `percent(25.0)]),
      //gridTemplateColumns(repeat(12, minmax(px(100), fr(1)))),
      //grid-template-columns: repeat(auto-fit, minmax(100px, 1fr));
      gridGap(px(3)),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {albumData, albumDataLoading, player: {deviceId}}: StoreData.state = state;

  React.useEffect1(
    () => {
      switch (deviceId->String.length) {
      | 0 => ()
      | _ => dispatch(FetchNewReleases)
      };
      Some(() => ());
    },
    [|deviceId|],
  );

  <div>
    <h1 className=Styles.title> {ReasonReact.string("Results")} </h1>
    {albumDataLoading && albumData->List.length > 0 ? <LoadingSpinner /> : ReasonReact.null}
    <div className=Styles.container>
      {List.map(album => <Album album key={album.id} />, albumData)
       |> Array.of_list
       |> ReasonReact.array}
    </div>
  </div>;
};