type image = {
  height: int,
  width: int,
  url: string,
};

type track = {
  name: string,
  uri: string,
  duration_ms: int,
  track_number: int,
};

type tracks = {itemss: list(track)};

type artist = {name: string};

type albumDetails = {
  name: string,
  release_date: string,
  id: string,
  images: list(image),
  artists: list(artist),
  tracks: tracks,
  uri: string,
};

type album = {
  name: string,
  release_date: string,
  id: string,
  images: list(image),
  artists: list(artist),
  uri: string,
};

type albums = {items: list(album)};
type response = {albums};

module Decode = {
  let artist = json => Json.Decode.{name: json |> field("name", string)};

  let image = json =>
    Json.Decode.{
      height: json |> field("height", int),
      width: json |> field("width", int),
      url: json |> field("url", string),
    };

  let track = json =>
    Json.Decode.{
      name: json |> field("name", string),
      uri: json |> field("uri", string),
      duration_ms: json |> field("duration_ms", int),
      track_number: json |> field("track_number", int),
    };

  let tracks = json =>
    Json.Decode.{itemss: json |> field("items", list(track))};

  let album = json =>
    Json.Decode.{
      name: json |> field("name", string),
      release_date: json |> field("release_date", string),
      id: json |> field("id", string),
      images: json |> field("images", list(image)),
      artists: json |> field("artists", list(artist)),
      uri: json |> field("uri", string),
    };

  let albumDetails = json =>
    Json.Decode.{
      name: json |> field("name", string),
      release_date: json |> field("release_date", string),
      id: json |> field("id", string),
      images: json |> field("images", list(image)),
      artists: json |> field("artists", list(artist)),
      uri: json |> field("uri", string),
      tracks: json |> field("tracks", tracks)
    };

  let albums = json =>
    Json.Decode.{items: json |> field("items", list(album))};

  let response = json =>
    Json.Decode.{albums: json |> field("albums", albums)};
};