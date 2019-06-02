module Styles = {
  open Css;

  let titleBar =
    style([
      padding(px(5)),
      backgroundColor(hex("DB4D3F")),
      display(`flex),
      alignItems(`center),
      justifyContent(`spaceBetween),
      boxSizing(`borderBox),
      position(`relative),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(12, 38, 69, 0.15)),
    ]);

  let controls =
    style([
      backgroundColor(hex("B03D32")),
      display(`flex),
      justifyContent(`flexEnd),
      boxSizing(`borderBox),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(12, 38, 69, 0.3)),
    ]);

  let container =
    style([position(`fixed), width(`percent(100.0)), zIndex(10)]);

  let title =
    style([
      fontSize(20 |> px),
      textTransform(`uppercase),
      color(white),
      margin(px(0)),
      marginLeft(px(5)),
      fontWeight(`num(700)),
      letterSpacing(px(2))
      //textTransform(`uppercase),
    ]);
};

[@react.component]
let make = () => {
  <div className=Styles.container>
    <div className=Styles.titleBar>
      <h1 className=Styles.title>
        {ReasonReact.string("Reason Spotify client")}
      </h1>
      <LoginButton />
    </div>
    <div className=Styles.controls> <Player /> <Visualizer /> </div>
  </div>;
};