type image = {
  height: int,
  width: int,
  url: string,
};

type artist = {name: string};

type album = {
  name: string,
  release_date: string,
  id: string,
  images: list(image),
  artists: list(artist)
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

  let album = json =>
    Json.Decode.{
      name: json |> field("name", string),
      release_date: json |> field("release_date", string),
      id: json |> field("id", string),
      images: json |> field("images", list(image)),
      artists: json |> field("artists", list(artist))
    };

  let albums = json =>
    Json.Decode.{items: json |> field("items", list(album))};

  let response = json =>
    Json.Decode.{albums: json |> field("albums", albums)};
};