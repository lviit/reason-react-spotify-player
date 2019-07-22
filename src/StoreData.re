type image = {
  height: int,
  width: int,
  url: string,
};

type track = {
  name: string,
  uri: string,
  id: string,
  duration_ms: int,
  track_number: int,
};

type tracksItems = {items: list(track)};

type artist = {name: string};

type albumDetails = {
  name: string,
  release_date: string,
  id: string,
  images: list(image),
  artists: list(artist),
  tracks: tracksItems,
  uri: string,
};

type album = {
  name: string,
  id: string,
  images: list(image),
  artists: list(artist),
  uri: string,
};

type albumCondensed = {
  name: string,
  uri: string,
}

type albumsItems = {items: list(album)};
type albums = {albums: albumsItems};

type currentTrack = {
  id: string,
  name: string,
  artists: list(artist),
  duration_ms: int,
  album: albumCondensed,
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
  albumData: list(album),
  albumDetailsLoading: bool,
  albumDetailsOpen: bool,
  albumDetails: option(albumDetails),
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
  | FetchAlbumsFulfilled(albums)
  | OpenAlbumDetails
  | CloseAlbumDetails
  | FetchAlbumDetailsPending
  | FetchAlbumDetailsFulfilled(albumDetails)
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