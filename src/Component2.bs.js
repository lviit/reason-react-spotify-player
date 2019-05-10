// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Utils$ReactHooksTemplate = require("./Utils.bs.js");

function item(json) {
  return /* record */[
          /* id */Json_decode.field("id", Json_decode.string, json),
          /* name */Json_decode.field("name", Json_decode.string, json)
        ];
}

function response(json) {
  return /* record */[/* item */Json_decode.field("item", item, json)];
}

var Decode = /* module */[
  /* item */item,
  /* response */response
];

function Component2(Props) {
  var authHeader = Props.authHeader;
  var match = React.useReducer((function (state, action) {
          if (typeof action === "number") {
            switch (action) {
              case 0 : 
                  return /* record */[
                          /* count */state[/* count */0] + 1 | 0,
                          /* show */state[/* show */1],
                          /* data */state[/* data */2],
                          /* loading */state[/* loading */3]
                        ];
              case 1 : 
                  return /* record */[
                          /* count */state[/* count */0],
                          /* show */!state[/* show */1],
                          /* data */state[/* data */2],
                          /* loading */state[/* loading */3]
                        ];
              case 2 : 
                  return /* record */[
                          /* count */state[/* count */0],
                          /* show */state[/* show */1],
                          /* data */state[/* data */2],
                          /* loading */true
                        ];
              
            }
          } else {
            return /* record */[
                    /* count */state[/* count */0],
                    /* show */state[/* show */1],
                    /* data */action[0][/* item */0],
                    /* loading */false
                  ];
          }
        }), /* record */[
        /* count */0,
        /* show */true,
        /* data : record */[
          /* id */"",
          /* name */""
        ],
        /* loading */false
      ]);
  var dispatch = match[1];
  var state = match[0];
  React.useEffect((function () {
          Curry._1(dispatch, /* FetchDataPending */2);
          Utils$ReactHooksTemplate.fetchWithAuth("https://api.spotify.com/v1/me/player", authHeader).then((function (prim) {
                      return prim.json();
                    })).then((function (json) {
                    return Promise.resolve(response(json));
                  })).then((function (data) {
                  return Promise.resolve(Curry._1(dispatch, /* FetchDataFulfilled */[data]));
                }));
          return (function (param) {
                    return /* () */0;
                  });
        }), /* array */[]);
  var message = "You've clicked this " + (String(state[/* count */0]) + " times(s)");
  var text = "now playing" + state[/* data */2][/* name */1];
  var match$1 = state[/* loading */3];
  return React.createElement("div", undefined, React.createElement("button", {
                  onClick: (function (_event) {
                      return Curry._1(dispatch, /* Click */0);
                    })
                }, message), React.createElement("button", {
                  onClick: (function (_event) {
                      return Curry._1(dispatch, /* Toggle */1);
                    })
                }, "Toggle greeting"), React.createElement("div", undefined, match$1 ? "...loading" : React.createElement("div", undefined)), React.createElement("h3", undefined, text), React.createElement("a", {
                  className: "btn btn--loginApp-link",
                  href: "https://accounts.spotify.com/authorize?client_id=64d03692241b478cb763ec2a7eed99e0&redirect_uri=http://localhost:8000&scope=" + (Utils$ReactHooksTemplate.join("%20", /* :: */[
                          "user-read-currently-playing",
                          /* :: */[
                            "user-read-playback-state",
                            /* [] */0
                          ]
                        ]) + "&response_type=token&show_dialog=true")
                }, "Login to Spotify"));
}

var make = Component2;

exports.Decode = Decode;
exports.make = make;
/* react Not a pure module */
