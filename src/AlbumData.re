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
  release_date: string,
  id: string,
  images: list(image),
  artists: list(artist),
  uri: string,
};

type albumsItems = {items: list(album)};
type albums = {albums: albumsItems};
