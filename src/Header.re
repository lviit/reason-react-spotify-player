module Styles = {
  open Css;

  let titleBar =
    style([
      padding(px(10)),
      backgroundColor(hex("DB4D3F")),
      display(`flex),
      justifyContent(`spaceBetween),
      boxSizing(`borderBox),
    ]);

    let controls =
    style([
      backgroundColor(hex("B03D32")),
      display(`flex),
      justifyContent(`flexEnd),
      boxSizing(`borderBox),
    ]);

  let container =
    style([
      position(`fixed),
      width(`percent(100.0)),
      zIndex(10),
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
    <div className=Styles.titleBar>
      <h1 className=Styles.title>
        {ReasonReact.string("Reason Spotify client")}
      </h1>
      <LoginButton />
    </div>
    <div className=Styles.controls> <Player /> <Visualizer /> </div>
  </div>;
};