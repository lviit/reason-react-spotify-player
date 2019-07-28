open StoreProvider;

module Styles = {
  open Css;

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
  let {albumData: {items}, albumDataLoading, player: {deviceId}}: StoreData.state = state;

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
    {albumDataLoading && items->List.length > 0 ? <LoadingSpinner /> : ReasonReact.null}
    <div className=Styles.container>
      {List.map(album => <Album album key={album.id} />, items)
       |> Array.of_list
       |> ReasonReact.array}
    </div>
  </div>;
};