type album = {
  name: string,
  release_date: string,
  id: string,
};

type albums = {items: list(album)};
type response = {albums};

module Decode = {
  let album = json =>
    Json.Decode.{
      name: json |> field("name", string),
      release_date: json |> field("release_date", string),
      id: json |> field("id", string),
    };

  let albums = json =>
    Json.Decode.{items: json |> field("items", list(album))};

  let response = json =>
    Json.Decode.{albums: json |> field("albums", albums)};
};