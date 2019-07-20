open Json.Decode;

let artist = (json): AlbumData.artist => {name: json |> field("name", string)};

let image = (json): AlbumData.image => {
  height: json |> field("height", int),
  width: json |> field("width", int),
  url: json |> field("url", string),
};

let track = (json): AlbumData.track => {
  name: json |> field("name", string),
  uri: json |> field("uri", string),
  duration_ms: json |> field("duration_ms", int),
  track_number: json |> field("track_number", int),
};

let album = (json): AlbumData.album => {
  name: json |> field("name", string),
  release_date: json |> field("release_date", string),
  id: json |> field("id", string),
  images: json |> field("images", list(image)),
  artists: json |> field("artists", list(artist)),
  uri: json |> field("uri", string),
};

let albumDetails = (json): AlbumData.albumDetails => {
  name: json |> field("name", string),
  release_date: json |> field("release_date", string),
  id: json |> field("id", string),
  images: json |> field("images", list(image)),
  artists: json |> field("artists", list(artist)),
  uri: json |> field("uri", string),
  tracks:
    json
    |> field("tracks", (json) =>
         ({items: json |> field("items", list(track))}: AlbumData.tracksItems)
       ),
};

let albums = (json): AlbumData.albums => {
  albums:
    json
    |> field("albums", (json) =>
         ({items: json |> field("items", list(album))}: AlbumData.albumsItems)
       ),
};

let currentTrack = (json): StoreData.currentTrack => {
  id: json |> field("id", string),
  name: json |> field("name", string),
  artists: json |> field("artists", list(artist)),
  duration_ms: json |> field("duration_ms", int),
};

let track_window = (json): StoreData.trackWindow => {
  current_track: json |> field("current_track", currentTrack),
};

let playerState = (json): StoreData.playerState => {
  position: json |> field("position", int),
  paused: json |> field("paused", bool),
  track_window: json |> field("track_window", track_window),
};