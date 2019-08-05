open StoreProvider;

module Styles = {
  open Css;

  let main = albumDetailsOpen =>
    style([
      paddingTop(px(50)),
      position(`relative),
      filter([`blur(px(albumDetailsOpen ? 10 : 0))]),
      margin2(~v=`zero, ~h=`auto),
      overflow(albumDetailsOpen ? `hidden : `auto),
      height(`calc((`sub, vh(100.0), px(50)))),
      unsafe("-webkit-overflow-scrolling", "touch"),
      ...Styles.scrollBar,
    ]);

  let overlay =
    style([
      display(`block),
      zIndex(1),
      position(`fixed),
      top(`zero),
      left(`zero),
      right(`zero),
      bottom(`zero),
      backgroundColor(rgba(12, 38, 69, 0.38)),
    ]);

  global(
    "body",
    [
      fontFamily("IBM Plex Sans"),
      letterSpacing(pxFloat(0.5)),
      margin(px(0)),
      backgroundColor(hex("000")),
      color(hex("24292e")),
    ],
  );

  global("input", [letterSpacing(pxFloat(0.5))]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);
  let {albumDetailsOpen, albumDataLoading, player: {deviceId, loading}}: StoreData.state = state;

  React.useEffect1(
    () => {
      LoadPlayer->dispatch;
      None;
    },
    [||],
  );

  <React.Fragment>
    <Header />
    {albumDetailsOpen
       ? <div className=Styles.overlay onClick={_ => CloseAlbumDetails->dispatch} />
       : ReasonReact.null}
    {loading || albumDataLoading
       ? <div className=Styles.overlay> <LoadingSpinner /> </div> : ReasonReact.null}
    <AlbumDetails />
    <div className={Styles.main(albumDetailsOpen)}>
      {switch (deviceId->String.length) {
       | 0 => <LoginButton />
       | _ => <Albums />
       }}
    </div>
  </React.Fragment>;
};