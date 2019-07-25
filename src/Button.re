module Styles = {
  open Css;

  let icon = style([padding2(~v=px(0), ~h=px(5)), hover([cursor(`pointer)])]);
};

type icon =
  | Play
  | Pause
  | Next
  | Prev
  | Volume;

type color =
  | Dark
  | Light;

[@react.component]
let make = (~icon, ~action, ~color=?, ~size="40") => {
  let fill =
    switch (color) {
    | None => "#fff"
    | Some(Light) => "#fff"
    | Some(Dark) => "#000"
    };

  switch (icon) {
  | Play =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path d="M0 0h24v24H0z" fill="none" />
      <path
        fill
        d="M10 16.5l6-4.5-6-4.5v9zM12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"
      />
    </svg>
  | Pause =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path d="M0 0h24v24H0z" fill="none" />
      <path
        fill
        d="M9 16h2V8H9v8zm3-14C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm1-4h2V8h-2v8z"
      />
    </svg>
  | Next =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path d="M6 18l8.5-6L6 6v12zM16 6v12h2V6h-2z" fill />
      <path d="M0 0h24v24H0z" fill="none" />
    </svg>
  | Prev =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path d="M6 6h2v12H6zm3.5 6l8.5 6V6z" fill />
      <path d="M0 0h24v24H0z" fill="none" />
    </svg>
  | Volume =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path d="M3 9v6h4l5 5V4L7 9H3zm13.5 3c0-1.77-1.02-3.29-2.5-4.03v8.05c1.48-.73 2.5-2.25 2.5-4.02zM14 3.23v2.06c2.89.86 5 3.54 5 6.71s-2.11 5.85-5 6.71v2.06c4.01-.91 7-4.49 7-8.77s-2.99-7.86-7-8.77z"/>
      <path d="M0 0h24v24H0z" fill="none"/>
    </svg>
  };
};