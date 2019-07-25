open StoreProvider;

module Styles = {
  open Css;

  let main = albumDetailsOpen =>
    style([
      padding3(~top=px(115), ~h=px(20), ~bottom=px(20)),
      filter([`blur(px(albumDetailsOpen ? 10 : 0))]),
      maxWidth(px(1440)),
      margin2(~v=`zero, ~h=`auto),
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
      margin(px(0)),
      backgroundColor(hex("F6F4F4")),
      color(hex("24292e")),
    ],
  );
};

module Main = {
  [@react.component]
  let make = () => {
    let (state, dispatch) = React.useContext(storeContext);
    let {albumDetailsOpen, player: {deviceId, loading}}: StoreData.state = state;

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
      <AlbumDetails />
      <div className={Styles.main(albumDetailsOpen)}>
        {switch (deviceId->String.length) {
         | 0 => loading ? <LoadingSpinner /> : <LoginButton />
         | _ => loading ? <LoadingSpinner /> : <div> <SearchInput /> <Albums /> </div>
         }}
      </div>
    </React.Fragment>;
  };
};

[@react.component]
let make = () => {
  <StoreProvider> <Main /> </StoreProvider>;
};