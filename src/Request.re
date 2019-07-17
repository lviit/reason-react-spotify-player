let baseUrl = "https://api.spotify.com/v1";

let reqBody = payload => payload |> Js.Json.object_ |> Js.Json.stringify |> Fetch.BodyInit.make;

type request =
  | NewReleases
  | Player
  | Play
  | Pause
  | Next
  | Previous
  | Search(string)
  | AlbumDetails(string);

let requestBase = (endpoint: string, method: Fetch.requestMethod, accessToken) =>
  Fetch.fetchWithInit(
    baseUrl ++ endpoint,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": "Bearer " ++ accessToken}),
      ~method_=method,
      (),
    ),
  );

let playSong = (trackUri, contextUri, deviceId, accessToken) => {
  let offset = Js.Dict.empty();
  let payload = Js.Dict.empty();
  Js.Dict.set(offset, "uri", Js.Json.string(trackUri));
  Js.Dict.set(payload, "offset", Js.Json.object_(offset));
  Js.Dict.set(payload, "context_uri", Js.Json.string(contextUri));
  Fetch.fetchWithInit(
    baseUrl ++ "/me/player/play?device_id=" ++ deviceId,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": "Bearer " ++ accessToken}),
      ~method_=Put,
      ~body=payload |> Js.Json.object_ |> Js.Json.stringify |> Fetch.BodyInit.make,
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
  | Search(keywords) =>
    requestBase("/search?type=album&limit=50&q=" ++ keywords, Get, accessToken)
  | AlbumDetails(id) => requestBase("/albums/" ++ id, Get, accessToken)
  };