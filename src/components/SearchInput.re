open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      position(`relative),
      display(`flex),
      justifyContent(`center),
      flexDirection(`column),
      backgrounds([
        radialGradient([
          (`percent(0.0), rgba(0, 0, 0, 0.0)),
          (`percent(100.0), rgba(0, 0, 0, 0.5)),
        ]),
        hex("282828"),
      ]),
      padding(px(20)),
      color(hex("fff")),
      overflow(`hidden),
    ]);

  let title = style([fontWeight(`num(700)), fontSize(px(28))]);

  let input =
    style([
      borderWidth(`zero),
      borderBottom(px(1), `solid, hex("fff")),
      padding2(~v=px(7), ~h=`zero),
      marginBottom(px(10)),
      fontSize(px(20)),
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

  <form className=Styles.container onSubmit=handleSubmit>
    <label htmlFor="keywords" className=Styles.title> {ReasonReact.string("Search")} </label>
    <input id="keywords" name="keywords" placeholder="artist or album" className=Styles.input />
    <span className=Styles.resultCount>
      {ReasonReact.string(state.albumData.total->string_of_int ++ " results")}
    </span>
  </form>;
};