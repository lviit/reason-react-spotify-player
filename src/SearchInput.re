open StoreProvider;

module Styles = {
  open Css;

  let container =
    style([
      marginTop(px(40)),
      display(`flex),
      justifyContent(`center),
    ]);

  let input =
    style([
      padding2(~v=px(10), ~h=px(15)),
      fontSize(px(20)),
      backgroundColor(hex("ddd")),
      borderStyle(`none)
    ]);
};

[@react.component]
let make = () => {
  let (_, dispatch) = React.useContext(storeContext);

  let handleSubmit = e => {
    ReactEvent.Form.preventDefault(e);
    let keywords = e->ReactEvent.Form.target##keywords##value;
    Search(keywords)->dispatch;
  };

  <form className=Styles.container onSubmit=handleSubmit>
    <input name="keywords" placeholder="search albums" className=Styles.input />
  </form>;
};