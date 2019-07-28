open Css;

let colorBg = "F6F4F4";
let colorFontLight = "FFFFFF";

let scrollBar = [
  selector("scrollbar-face-color", [backgroundColor(hex("#aaa"))]),
  selector("scrollbar-track-color", [backgroundColor(hex("#eee"))]),
  selector("::-webkit-scrollbar", [width(px(10))]),
  selector("::-webkit-scrollbar-thumb", [background(hex("848484"))]),
  selector("::-webkit-scrollbar-track", [background(hex("aaa"))]),
];