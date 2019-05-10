open Utils;

type album = {
  name: string,
  release_date: string,
  id: string,
};
type albums = {items: list(album)};
type response = {albums};

type state = {
  count: int,
  show: bool,
  data: list(album),
  loading: bool,
};

type action =
  | FetchDataPending
  | FetchDataFulfilled(response);

module Decode = {
  let album = json =>
    Json.Decode.{
      name: json |> field("name", string),
      release_date: json |> field("release_date", string),
      id: json |> field("id", string),
    };

  let albums = json => Json.Decode.{items: json |> field("items", list(album))};

  let response = json =>
    Json.Decode.{albums: json |> field("albums", albums)};
};

[@react.component]
let make = (~authHeader) => {
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
        |> then_(json => json |> Decode.response |> resolve)
        |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [||],
  );

  <div>
    <div> {state.loading ? ReasonReact.string("...loading") : <div />} </div>
  </div>;
};