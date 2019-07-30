open StoreData;
open Actions;
open Reducer;

let initialState = {
  player: {
    accessToken: "",
    deviceId: "",
    loading: false,
    playing: false,
    progress: 0,
    progressTimer: None,
  },
  currentTrack: None,
  albumDataLoading: false,
  albumData: {
    items: [],
    total: 0,
  },
  albumDetailsLoading: false,
  albumDetailsOpen: false,
  albumDetails: None,
};

let storeContext = React.createContext((initialState, _ => ()));
module Provider = {
  let makeProps = (~value, ~children, ()) => {"value": value, "children": children};

  let make = React.Context.provider(storeContext);
};

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  <Provider value=(state, action(dispatch, state))> children </Provider>;
};