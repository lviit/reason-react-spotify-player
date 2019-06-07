open Request;

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
  | TogglePlay
  | Play(string)
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
  | Play(uri) => {...state, currentSong: uri}
  | TogglePlay => {...state, playing: !state.playing}
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

let storeContext = React.createContext((initialState, action => ()));
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

  // play song
   React.useEffect1(
     () => {
       if (state.currentSong !== "") {
         let payload = Js.Dict.empty();
         Js.Dict.set(
           payload,
           "context_uri",
           Js.Json.string(state.currentSong),
         );

         Js.Promise.(
           playSong(payload)
           |> then_(resolve)
         )
         |> ignore;
       };
       Some(() => ());
     },
     [|state.currentSong|],
   );

  // fetch player data
  React.useEffect1(
    () => {
      dispatch(FetchDataPending);

      Js.Promise.(
        request(Player)
          |> then_(Fetch.Response.json)
          |> then_(json => json |> Decode.response |> resolve)
          |> then_(data => FetchDataFulfilled(data) |> dispatch |> resolve)
        )
      |> ignore;
      Some(() => ());
    },
    [|state.currentSong|],
  );

  // toggle play/pause
  React.useEffect1(
    () => {      
      Js.Promise.(
        request(state.playing? Pause : Play)
        |> then_(resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [|state.playing|],
  );

  // fetch album data
  React.useEffect1(
    () => {
      dispatch(FetchAlbumDataPending);

      Js.Promise.(
        request(NewReleases)
        |> then_(Fetch.Response.json)
        |> then_(json => json |> AlbumData.Decode.response |> resolve)
        |> then_(data => FetchAlbumDataFulfilled(data) |> dispatch |> resolve)
      )
      |> ignore;
      Some(() => ());
    },
    [||],
  );

  <Provider value=(state, dispatch)> children </Provider>;
};