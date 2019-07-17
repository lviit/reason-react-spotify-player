open Actions;
open StoreData;

let reducer = (state, reducer: StoreData.reducer) =>
  switch (reducer) {
  | Play(timerId) => {...state, playing: true, progressTimer: timerId}
  | Pause => {...state, playing: false}
  | IncrementProgress => {...state, progress: state.progress + 1}
  | FetchDataPending => {...state, loading: true}
  | FetchDataFulfilled(data) => {
      ...state,
      loading: false,
      data: data.item,
      progress: data.progress_ms / 1000,
    }
  | FetchAlbumsPending => {...state, albumDataLoading: true}
  | FetchAlbumsFulfilled(data) => {
      ...state,
      albumDataLoading: false,
      albumData: data.albums.items,
    }
  | OpenAlbumDetails => {...state, albumDetailsOpen: true}
  | CloseAlbumDetails => {...state, albumDetailsOpen: false}
  | FetchAlbumDetailsPending => {...state, albumDetailsLoading: true}
  | FetchAlbumDetailsFulfilled(data) => {
      ...state,
      albumDetailsLoading: false,
      albumDetails: data,
    }
  | PlayerLoading(accessToken) => {
      ...state,
      player: {
        ...state.player,
        accessToken,
        loading: true,
      },
    }
  | PlayerReady(deviceId) => {
      ...state,
      player: {
        ...state.player,
        loading: false,
        deviceId,
      },
    }
  };

let initialState = {
  loading: false,
  playing: false,
  player: {
    accessToken: "",
    deviceId: "",
    loading: false,
  },
  data: {
    id: "",
    name: "",
    artists: [],
  },
  progress: 0,
  progressTimer: Js.Global.setInterval(() => (), 1000000), // fix so we can default to unit
  albumDataLoading: false,
  albumData: [],
  albumDetailsLoading: false,
  albumDetailsOpen: false,
  albumDetails: {
    name: "",
    release_date: "",
    id: "",
    images: [],
    artists: [],
    tracks: {
      itemss: [],
    },
    uri: "",
  },
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

  <Provider value=(state, action(dispatch, state))> children </Provider>;
};