open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      display(`grid),
      gridTemplateColumns([`repeat((`autoFill, `minmax((px(250), fr(1.0)))))]),
      gridGap(px(3)),
      unsafe("grid-auto-rows", "1fr"),
      selector("& > *:first-child", [gridRow(1, 1), gridColumn(1, 1)]),
      before([
        unsafe("content", ""),
        width(`zero),
        paddingBottom(`percent(100.0)),
        gridRow(1, 1),
        gridColumn(1, 1),
      ]),
    ]);

  let placeHolderTile =
    style([
      backgrounds([
        radialGradient([
          (`percent(0.0), rgba(0, 0, 0, 0.0)),
          (`percent(100.0), rgba(0, 0, 0, 0.5)),
        ]),
        hex("6b6b6b"),
      ]),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {albumData: {items}, player: {deviceId}}: StoreData.state = state;
  let tiles =
    List.length(items) > 0
      ? [<SearchInput />, ...List.map(album => <Album album key={album.id} />, items)]
      : <div className=Styles.placeHolderTile /> |> Belt.List.make(50);

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

  <div className=Styles.container> {tiles |> Array.of_list |> ReasonReact.array} </div>;
};