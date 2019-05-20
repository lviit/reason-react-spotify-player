open Utils;

type state = {
  count: int,
  show: bool,
  data: list(AlbumData.album),
  loading: bool,
};

type action =
  | FetchDataPending
  | FetchDataFulfilled(AlbumData.response);

module Styles = {
  open Css;

  let title = style([fontSize(px(60))]);

  let container =
    style([
      display(`grid),
      gridAutoRows(px(300)),
      gridTemplateColumns([px(300), px(300), px(300)]),
      gridGap(px(30)),
    ]);

  global(
    "body",
    [
      fontFamily("IBM Plex Sans"),
    ],
  );
};

[@react.component]
let make = (~authHeader, ~deviceId) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | FetchDataPending => {...state, loading: true}
        | FetchDataFulfilled(data) => {
            ...state,
            loading: false,
            data: data.albums.items,
          }
        },
      {count: 0, show: true, loading: false, data: []},
    );

  React.useEffect1(
    () => {
      dispatch(FetchDataPending);

      Js.Promise.(
        fetchWithAuth(
          "https://api.spotify.com/v1/browse/new-releases",
          authHeader,
        )
        |> then_(Fetch.Response.json)
        |> then_(json => json |> AlbumData.Decode.response |> resolve)
        |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [||],
  );

  <div>
    <h1 className=Styles.title> {ReasonReact.string("new releases")} </h1>
    <div className=Styles.container>
      {List.length(state.data) > 0
         ? List.map(album => <Album album authHeader deviceId/>, state.data)
           |> Array.of_list
           |> ReasonReact.array
         : ReasonReact.string("No data available.")}
    </div>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
  </div>;
};