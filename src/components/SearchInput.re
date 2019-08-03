open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      position(`relative),
      padding(px(20)),
      display(`flex),
      flexDirection(`column),
      backgroundColor(hex("8c3028")),
      color(hex("fff")),
      justifyContent(`center),
      overflow(`hidden),
      backgrounds([
        radialGradient([
          (`percent(0.0), rgba(0, 0, 0, 0.0)),
          (`percent(100.0), rgba(0, 0, 0, 0.5)),
        ]),
        hex("8c3028"),
      ]),
    ]);

  let infoContainer = style([zIndex(1), display(`flex), flexDirection(`column)]);

  let title = style([fontWeight(`num(600)), fontSize(px(32))]);

  let image =
    style([
      position(`absolute),
      left(`percent(-10.0)),
      width(`percent(120.0)),
      filter([`blur(px(10)), `brightness(50.0)]),
      transition(
        ~duration=500,
        ~delay=0,
        ~timingFunction=cubicBesier(0.15, 1.0, 0.33, 1.0),
        "transform",
      ),
    ]);

  let input =
    style([
      borderWidth(`zero),
      borderBottom(px(2), `solid, hex("fff")),
      padding2(~v=px(7), ~h=`zero),
      marginBottom(px(10)),
      fontSize(px(22)),
      color(hex("fff")),
      backgroundColor(`transparent),
      backgroundImage(
        `url(
          "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTUuNSAxNGgtLjc5bC0uMjgtLjI3QzE1LjQxIDEyLjU5IDE2IDExLjExIDE2IDkuNSAxNiA1LjkxIDEzLjA5IDMgOS41IDNTMyA1LjkxIDMgOS41IDUuOTEgMTYgOS41IDE2YzEuNjEgMCAzLjA5LS41OSA0LjIzLTEuNTdsLjI3LjI4di43OWw1IDQuOTlMMjAuNDkgMTlsLTQuOTktNXptLTYgMEM3LjAxIDE0IDUgMTEuOTkgNSA5LjVTNy4wMSA1IDkuNSA1IDE0IDcuMDEgMTQgOS41IDExLjk5IDE0IDkuNSAxNHoiIGZpbGw9IiNmZmYiIC8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjwvc3ZnPg==",
        ),
      ),
      backgroundRepeat(`noRepeat),
      backgroundPosition(`percent(100.0), `percent(75.0)),
      backgroundSize(`size((px(30), px(30)))),
      placeholder([fontStyle(`italic), color(hex("fff"))]),
      focus([outlineWidth(px(0))]),
    ]);

  let resultCount = style([fontSize(px(16)), fontWeight(`num(700))]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(storeContext);

  let handleSubmit = e => {
    ReactEvent.Form.preventDefault(e);
    let keywords = e->ReactEvent.Form.target##keywords##value;
    Search(keywords)->dispatch;
  };

  let bgItem =
    switch (List.length(state.albumData.items)) {
    | 0 => None
    | length => Some(List.nth(state.albumData.items, Random.int(length)).images->List.hd)
    };

  <form className=Styles.container onSubmit=handleSubmit>
    {switch (bgItem) {
     | Some(item) => <img className=Styles.image src={item.url} alt="" />
     | None => ReasonReact.null
     }}
    <div className=Styles.infoContainer>
      <span className=Styles.title> {ReasonReact.string("Search")} </span>
      <input name="keywords" placeholder="album title" className=Styles.input />
      <span className=Styles.resultCount>
        {ReasonReact.string(state.albumData.total->string_of_int ++ " results")}
      </span>
    </div>
  </form>;
};