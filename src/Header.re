open Utils;

module Styles = {
  open Css;

  let container =
    style([
      padding(px(10)),
      backgroundColor(hex("DB4D3F")),
      position(`fixed),
      width(`percent(100.0)),
      zIndex(10),
      display(`flex),
      justifyContent(`spaceBetween),
      boxSizing(`borderBox)
    ]);

  let title =
    style([
      fontSize(25 |> px),
      textTransform(`uppercase),
      color(white),
      margin(px(0)),
      fontWeight(`num(700)),
      //textTransform(`uppercase),
    ]);
};

[@react.component]
let make = () => {
  <div className=Styles.container>
    <h1 className=Styles.title>
      {ReasonReact.string("Reason Spotify client")}
    </h1>
    <LoginButton />
  </div>;
};