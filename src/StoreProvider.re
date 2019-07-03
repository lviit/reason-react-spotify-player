open Request;

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";

type artist = {name: string};

type item = {
  id: string,
  name: string,
  artists: list(artist),
};

type state = {
  data: item,
  loading: bool,
  playing: bool,
  progress: int,
  currentSong: string,
  albumDataLoading: bool,
  albumData: list(AlbumData.album),
};

type response = {
  item,
  progress_ms: int,
};

/* Action declaration */
type action =
  | Prev
  | Next
  | PlaySong(string)
  | Play
  | Pause
  | FetchPlayer
  | FetchDataPending
  | FetchDataFulfilled(response)
  | FetchAlbumDataPending
  | FetchAlbumDataFulfilled(AlbumData.response);

module Decode = {
  let artist = json => Json.Decode.{name: json |> field("name", string)};

  let item = json =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      artists: json |> field("artists", list(artist)),
    };

  let response = json =>
    Json.Decode.{
      item: json |> field("item", item),
      progress_ms: json |> field("progress_ms", int),
    };
};

let reducer = (state, action) =>
  switch (action) {
  | Play => {...state, playing: true}
  | Pause => {...state, playing: false}
  | FetchDataPending => {...state, loading: true}
  | FetchDataFulfilled(data) => {
      ...state,
      loading: false,
      data: data.item,
      progress: data.progress_ms,
    }
  | FetchAlbumDataPending => {...state, albumDataLoading: true}
  | FetchAlbumDataFulfilled(data) => {
      ...state,
      albumDataLoading: false,
      albumData: data.albums.items,
    }
  | _ => state
  };

let initialState = {
  loading: false,
  playing: false,
  data: {
    id: "",
    name: "",
    artists: [],
  },
  progress: 0,
  albumDataLoading: false,
  albumData: [],
  currentSong: "",
};

let rec handleAsync = (dispatch, action) => {
  switch (action) {
  | PlaySong(uri) =>
    Js.Promise.(
      playSong(uri)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, FetchPlayer), 300)
           |> resolve
         )
      |> then_(_ => dispatch(Play) |> resolve)
    )  // wtf why the timeout
    |> ignore
  | Prev =>
    Js.Promise.(
      request(Previous)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | Next =>
    Js.Promise.(
      request(Next)
      |> then_(_ =>
           setTimeout(_ => handleAsync(dispatch, FetchPlayer), 300)
           |> resolve
         )
    )  // wtf why the timeout
    |> ignore
  | FetchPlayer =>
    Js.Promise.(
      request(Player)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> Decode.response |> resolve)
      |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
      |> resolve
    )
    |> ignore
  | Play =>
    Js.Promise.(request(Play) |> then_(_ => dispatch(Play) |> resolve))
    |> ignore
  | Pause =>
    Js.Promise.(request(Pause) |> then_(_ => dispatch(Pause) |> resolve))
    |> ignore
  | FetchAlbumDataPending =>
    Js.Promise.(
      request(NewReleases)
      |> then_(Fetch.Response.json)
      |> then_(json => json |> AlbumData.Decode.response |> resolve)
      |> then_(data => FetchAlbumDataFulfilled(data) |> dispatch |> resolve)
    )
    |> ignore
  | _ => action |> dispatch
  };
};

let storeContext = React.createContext((initialState, _ => ()));
module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let make = React.Context.provider(storeContext);
};

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  <Provider value=(state, handleAsync(dispatch))> children </Provider>;
};