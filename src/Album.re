module Styles = {
  open Css;
  open Styles;

  let container = style([padding(px(30)), backgroundColor(hex(bgColor))]);

  let title = style([fontSize(px(20)), textTransform(`uppercase)]);
};

[@react.component]
let make = (~album as {name, id, release_date}: AlbumData.album) => {
  <div className=Styles.container key=id>
    <h2 className=Styles.title> {React.string(name)} </h2>
    <p>{React.string(release_date)}</p>
  </div>;
};