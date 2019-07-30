open StoreData;

let reducer = (state, reducer: StoreData.reducer) =>
  switch (reducer) {
  | Play(timerId) => {
      ...state,
      player: {
        ...state.player,
        playing: true,
        progressTimer: Some(timerId),
      },
    }
  | Pause => {
      ...state,
      player: {
        ...state.player,
        playing: false,
      },
    }
  | IncrementProgress => {
      ...state,
      player: {
        ...state.player,
        progress: state.player.progress + 1000,
      },
    }
  | PlayerStateChange(playerState) => {
      ...state,
      currentTrack: Some(playerState.track_window.current_track),
      player: {
        ...state.player,
        progress: playerState.position,
        playing: !playerState.paused,
      },
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
  | FetchAlbumsPending => {...state, albumDataLoading: true}
  | FetchAlbumsFulfilled(data) => {
      ...state,
      albumDataLoading: false,
      albumData: {
        items: data.albums.items,
        total: data.albums.total,
      },
    }
  | FetchAlbumDetailsPending => {...state, albumDetailsLoading: true}
  | FetchAlbumDetailsFulfilled(data) => {
      ...state,
      albumDetailsLoading: false,
      albumDetails: Some(data),
    }
  | OpenAlbumDetails => {...state, albumDetailsOpen: true}
  | CloseAlbumDetails => {...state, albumDetailsOpen: false}
  | Seek(progress) => {
      ...state,
      player: {
        ...state.player,
        progress,
      },
    }
  };
