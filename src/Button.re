module Styles = {
  open Css;

  let icon = style([padding2(~v=px(0), ~h=px(5)), hover([cursor(`pointer)])]);
};

type icon =
  | Play
  | Pause
  | Next
  | Prev;

[@react.component]
let make = (~icon, ~action) =>
  switch (icon) {
  | Play =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width="40"
      height="40"
      viewBox="0 0 24 24">
      <path d="M0 0h24v24H0z" fill="none" />
      <path
        fill="#fff"
        d="M10 16.5l6-4.5-6-4.5v9zM12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"
      />
    </svg>
  | Pause =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width="40"
      height="40"
      viewBox="0 0 24 24">
      <path d="M0 0h24v24H0z" fill="none" />
      <path
        fill="#fff"
        d="M9 16h2V8H9v8zm3-14C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm1-4h2V8h-2v8z"
      />
    </svg>
  | Next =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width="32"
      height="32"
      viewBox="0 0 24 24">
      <path d="M6 18l8.5-6L6 6v12zM16 6v12h2V6h-2z" fill="#fff" />
      <path d="M0 0h24v24H0z" fill="none" />
    </svg>
  | Prev =>
    <svg
      className=Styles.icon
      onClick=action
      xmlns="http://www.w3.org/2000/svg"
      width="32"
      height="32"
      viewBox="0 0 24 24">
      <path d="M6 6h2v12H6zm3.5 6l8.5 6V6z" fill="#fff" />
      <path d="M0 0h24v24H0z" fill="none" />
    </svg>
  };