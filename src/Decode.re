open Json.Decode;

let artist = (json): StoreData.artist => {name: json |> field("name", string)};

let image = (json): StoreData.image => {
  height: json |> field("height", int),
  width: json |> field("width", int),
  url: json |> field("url", string),
};

let track = (json): StoreData.track => {
  name: json |> field("name", string),
  uri: json |> field("uri", string),
    id: json |> field("id", string),
  duration_ms: json |> field("duration_ms", int),
  track_number: json |> field("track_number", int),
};

let album = (json): StoreData.album => {
  name: json |> field("name", string),
  id: json |> field("id", string),
  images: json |> field("images", list(image)),
  artists: json |> field("artists", list(artist)),
  uri: json |> field("uri", string),
};

let albumDetails = (json): StoreData.albumDetails => {
  name: json |> field("name", string),
  release_date: json |> field("release_date", string),
  id: json |> field("id", string),
  images: json |> field("images", list(image)),
  artists: json |> field("artists", list(artist)),
  uri: json |> field("uri", string),
  tracks:
    json
    |> field("tracks", (json) =>
         ({items: json |> field("items", list(track))}: StoreData.tracksItems)
       ),
};

let albumCondensed = (json): StoreData.albumCondensed => {
  name: json |> field("name", string),
  uri: json |> field("uri", string),
};

let albums = (json): StoreData.albums => {
  albums:
    json
    |> field("albums", (json) =>
         ({items: json |> field("items", list(album))}: StoreData.albumsItems)
       ),
};

let currentTrack = (json): StoreData.currentTrack => {
  id: json |> field("id", string),
  name: json |> field("name", string),
  artists: json |> field("artists", list(artist)),
  duration_ms: json |> field("duration_ms", int),
  album: json |> field("album", albumCondensed),
};

let track_window = (json): StoreData.trackWindow => {
  current_track: json |> field("current_track", currentTrack),
};

let playerState = (json): StoreData.playerState => {
  position: json |> field("position", int),
  paused: json |> field("paused", bool),
  track_window: json |> field("track_window", track_window),
};