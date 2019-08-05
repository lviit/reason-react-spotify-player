open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      display(`grid),
      gridTemplateColumns([`repeat((`autoFill, `minmax((px(250), fr(1.0)))))]),
      gridGap(px(3)),
      unsafe("grid-auto-rows", "1fr"),
      selector("& > *:first-child", [
        gridRow(1,1),
        gridColumn(1,1),
      ]),
      before([
        unsafe("content", ""),
        width(`zero),
        paddingBottom(`percent(100.0)),
        gridRow(1,1),
        gridColumn(1,1),
      ])
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {albumData: {items}, player: {deviceId}}: StoreData.state = state;
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
    <div className=Styles.container> {tiles |> Array.of_list |> ReasonReact.array} </div>
  </div>;
};