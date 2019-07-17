open StoreProvider;

module Styles = {
  open Css;

  let main = albumDetailsOpen =>
    style([
      padding3(~top=px(115), ~h=px(20), ~bottom=px(20)),
      filter([`blur(px(albumDetailsOpen ? 10 : 0))]),
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

    React.useEffect1(
      () => {
        dispatch(LoadPlayer);
        Some(() => ());
      },
      [||],
    );

    <React.Fragment>
      <Header />
      {state.albumDetailsOpen
         ? <div
             className=Styles.overlay
             onClick={_ => CloseAlbumDetails->dispatch}
           />
         : ReasonReact.null}
      <AlbumDetails />
      <div className={Styles.main(state.albumDetailsOpen)}>
        {state.player.loading
           ? <LoadingSpinner /> : <div> <SearchInput /> <Albums /> </div>}
      </div>
    </React.Fragment>;
  };
};

[@react.component]
let make = () => {
  <StoreProvider> <Main /> </StoreProvider>;
};