open StoreProvider;

module Styles = {
  open Css;

  let main = style([padding3(~top=px(115), ~h=px(20), ~bottom=px(20))]);

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
      <AlbumDetails />
      <div className=Styles.main>
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