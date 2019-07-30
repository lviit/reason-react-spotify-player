open StoreProvider;

module Styles = {
  open Css;

  let container = style([marginBottom(px(20)), display(`flex), justifyContent(`spaceBetween)]);

  let input =
    style([
      padding2(~v=px(10), ~h=px(15)),
      fontSize(px(22)),
      color(hex("3d3d3d")),
      backgroundColor(hex("ddd")),
      backgroundImage(
        `url(
          "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTUuNSAxNGgtLjc5bC0uMjgtLjI3QzE1LjQxIDEyLjU5IDE2IDExLjExIDE2IDkuNSAxNiA1LjkxIDEzLjA5IDMgOS41IDNTMyA1LjkxIDMgOS41IDUuOTEgMTYgOS41IDE2YzEuNjEgMCAzLjA5LS41OSA0LjIzLTEuNTdsLjI3LjI4di43OWw1IDQuOTlMMjAuNDkgMTlsLTQuOTktNXptLTYgMEM3LjAxIDE0IDUgMTEuOTkgNSA5LjVTNy4wMSA1IDkuNSA1IDE0IDcuMDEgMTQgOS41IDExLjk5IDE0IDkuNSAxNHoiIGZpbGw9IiM4NDg0ODQiIC8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjwvc3ZnPg==",
        ),
      ),
      backgroundRepeat(`noRepeat),
      backgroundPosition(`percent(95.0), `percent(50.0)),
      backgroundSize(`size(px(30), px(30))),
      borderStyle(`none),
      placeholder([fontStyle(`italic), color(hex("848484"))]),
    ]);

  let resultCount = style([
    fontSize(px(28)),
    fontWeight(`num(700)),
    display(`flex),
    alignItems(`center)
  ]);
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
    <input name="keywords" placeholder="search albums" className=Styles.input />
    <span className=Styles.resultCount>{ReasonReact.string(state.albumData.total->string_of_int ++ " results")}</span>
  </form>;
};