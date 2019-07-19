type artist = {name: string};

type item = {
  id: string,
  name: string,
  artists: list(artist),
  duration_ms: int,
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
  albumDetailsLoading: bool,
  albumDetailsOpen: bool,
  albumDetails: AlbumData.albumDetails,
};

type track_window = {current_track: item};

type playerState = {
  position: int,
  paused: bool,
  track_window,
};

type reducer =
  | Play(Js.Global.intervalId)
  | Pause
  | FetchAlbumsPending
  | FetchAlbumsFulfilled(AlbumData.response)
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

module Decode = {
  let artist = json => Json.Decode.{name: json |> field("name", string)};

  let item = json =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      artists: json |> field("artists", list(artist)),
      duration_ms: json |> field("duration_ms", int),
    };

  let track_window = json => Json.Decode.{current_track: json |> field("current_track", item)};

  let playerState = json =>
    Json.Decode.{
      position: json |> field("position", int),
      paused: json |> field("paused", bool),
      track_window: json |> field("track_window", track_window),
    };
};
