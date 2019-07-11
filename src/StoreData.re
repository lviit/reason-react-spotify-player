type artist = {name: string};

type item = {
  id: string,
  name: string,
  artists: list(artist),
};

type player = {
  accessToken: string,
  deviceId: string,
  loading: bool,
};

type state = {
  data: item,
  loading: bool,
  playing: bool,
  progress: int,
  progressTimer: Js.Global.intervalId,
  albumDataLoading: bool,
  albumData: list(AlbumData.album),
  player,
};

type response = {
  item,
  progress_ms: int,
};

type reducer =
  | Play(Js.Global.intervalId)
  | Pause
  | FetchDataPending
  | FetchDataFulfilled(response)
  | FetchAlbumsPending
  | FetchAlbumsFulfilled(AlbumData.response)
  | IncrementProgress
  | PlayerLoading(string)
  | PlayerReady(string);

type actionType =
  | Prev
  | Next
  | PlaySong(string)
  | Play
  | Pause
  | FetchPlayer
  | FetchNewReleases
  | LoadPlayer
  | Search(string);