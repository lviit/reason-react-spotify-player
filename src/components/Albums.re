open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      display(`grid),
      gridTemplateColumns([`repeat((`autoFill, `minmax((px(250), fr(1.0)))))]),
      gridGap(px(3)),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {albumData: {items}, albumDataLoading, player: {deviceId}}: StoreData.state = state;
  let tiles = [<SearchInput />, ...List.map(album => <Album album key={album.id} />, items)];

  React.useEffect1(
    () => {
      switch (deviceId->String.length) {
      | 0 => ()
      | _ => dispatch(FetchNewReleases)
      };
      None;
    },
    [|deviceId|],
  );

  <div>
    {albumDataLoading && items->List.length > 0 ? <LoadingSpinner /> : ReasonReact.null}
    <div className=Styles.container> {tiles |> Array.of_list |> ReasonReact.array} </div>
  </div>;
};