open Js.Json;

let baseUrl = "https://api.spotify.com/v1";

type request =
  | NewReleases
  | Player
  | Play
  | Pause
  | Next
  | Previous
  | Seek(int)
  | Search(string)
  | AlbumDetails(string);

let requestBase = (endpoint: string, method: Fetch.requestMethod, accessToken) =>
  Fetch.RequestInit.make(
    ~headers=Fetch.HeadersInit.make({"Authorization": "Bearer " ++ accessToken}),
    ~method_=method,
    (),
  )
  |> Fetch.fetchWithInit(baseUrl ++ endpoint);

let playSong = (trackUri, contextUri, deviceId, accessToken) => {
  let offset = Js.Dict.empty();
  let payload = Js.Dict.empty();
  Js.Dict.set(offset, "uri", string(trackUri));
  Js.Dict.set(payload, "offset", object_(offset));
  Js.Dict.set(payload, "context_uri", string(contextUri));
  Fetch.fetchWithInit(
    baseUrl ++ "/me/player/play?device_id=" ++ deviceId,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": "Bearer " ++ accessToken}),
      ~method_=Put,
      ~body=payload->object_->stringify->Fetch.BodyInit.make,
      (),
    ),
  );
};

let request = (request, accessToken) =>
  switch (request) {
  | NewReleases => requestBase("/browse/new-releases", Get, accessToken)
  | Player => requestBase("/me/player", Get, accessToken)
  | Play => requestBase("/me/player/play", Put, accessToken)
  | Pause => requestBase("/me/player/pause", Put, accessToken)
  | Next => requestBase("/me/player/next", Post, accessToken)
  | Previous => requestBase("/me/player/previous", Post, accessToken)
  | Seek(position) =>
    requestBase("/me/player/seek?position_ms=" ++ string_of_int(position), Put, accessToken)
  | Search(keywords) =>
    requestBase("/search?type=album&limit=50&q=" ++ keywords, Get, accessToken)
  | AlbumDetails(id) => requestBase("/albums/" ++ id, Get, accessToken)
  };