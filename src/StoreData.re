type currentTrack = {
  id: string,
  name: string,
  artists: list(AlbumData.artist),
  duration_ms: int,
};

type player = {
  accessToken: string,
  deviceId: string,
  loading: bool,
  playing: bool,
  progress: int,
  progressTimer: option(Js.Global.intervalId),
};

type state = {
  player,
  currentTrack: option(currentTrack),
  albumDataLoading: bool,
  albumData: list(AlbumData.album),
  albumDetailsLoading: bool,
  albumDetailsOpen: bool,
  albumDetails: option(AlbumData.albumDetails),
};

type trackWindow = {current_track: currentTrack};

type playerState = {
  position: int,
  paused: bool,
  track_window: trackWindow,
};

type reducer =
  | Play(Js.Global.intervalId)
  | Pause
  | FetchAlbumsPending
  | FetchAlbumsFulfilled(AlbumData.albums)
  | OpenAlbumDetails
  | CloseAlbumDetails
  | FetchAlbumDetailsPending
  | FetchAlbumDetailsFulfilled(AlbumData.albumDetails)
  | IncrementProgress
  | PlayerLoading(string)
  | PlayerReady(string)
  | PlayerStateChange(playerState)
  | Seek(int);

type actionType =
  | Prev
  | Next
  | PlaySong(string, string)
  | Play
  | Pause
  | FetchNewReleases
  | FetchAlbumDetails(string)
  | LoadPlayer
  | Search(string)
  | CloseAlbumDetails
  | Seek(int);
