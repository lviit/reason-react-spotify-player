module Styles = {
  open Css;

  let load8 =
    keyframes([
      (0, [transform(`rotate(deg(0.0)))]),
      (100, [transform(`rotate(deg(360.0)))]),
    ]);

  let spinner =
    style([
      margin2(~v=px(50), ~h=`auto),
      fontSize(px(10)),
      position(`relative),
      textIndent(em(-9999.0)),
      borderTop(em(1.5), `solid, rgba(0, 0, 0, 0.2)),
      borderRight(em(1.5), `solid, rgba(0, 0, 0, 0.2)),
      borderBottom(em(1.5), `solid, rgba(0, 0, 0, 0.2)),
      borderLeft(em(1.5), `solid, rgba(0, 0, 0, 0.6)),
      transform(translateZ(`zero)),
      borderRadius(`percent(50.0)),
      width(rem(12.0)),
      height(rem(12.0)),
      animationName(load8),
      animationDuration(1100),
      animationIterationCount(`infinite),
      animationTimingFunction(`linear),
    ]);
};

[@react.component]
let make = () => <div className=Styles.spinner />;
