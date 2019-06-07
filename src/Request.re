open Webapi;

module QueryString = {
  open Url.URLSearchParams;

  [@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

  let authHeader =
    switch (hash |> make |> get("#access_token", _)) {
    | None => "none"
    | Some(accessToken) => "Bearer " ++ accessToken
    };

  let deviceId =
    switch (hash |> make |> get("device_id", _)) {
    | None => "none"
    | Some(deviceId) => deviceId
    };
};

let baseUrl = "https://api.spotify.com/v1";

let reqBody = payload =>
  payload |> Js.Json.object_ |> Js.Json.stringify |> Fetch.BodyInit.make;

type request =
  | NewReleases
  | Player
  | Play
  | Pause
  | Next
  | Previous;

let requestBase = (endpoint: string, method: Fetch.requestMethod) =>
  Fetch.fetchWithInit(
    baseUrl ++ endpoint,
    Fetch.RequestInit.make(
      ~headers=
        Fetch.HeadersInit.make({"Authorization": QueryString.authHeader}),
      ~method_=method,
      (),
    ),
  );

let playSong = (payload) =>
  Fetch.fetchWithInit(
    baseUrl ++ "/me/player/play?device_id=" ++ QueryString.deviceId,
    Fetch.RequestInit.make(
      ~headers=
        Fetch.HeadersInit.make({"Authorization": QueryString.authHeader}),
      ~method_=Put,
      ~body=payload |> Js.Json.object_ |> Js.Json.stringify |> Fetch.BodyInit.make,
      (),
    ),
  );

let request = request =>
  switch (request) {
  | NewReleases => requestBase("/browse/new-releases", Get)
  | Player => requestBase("/me/player", Get)
  | Play => requestBase("/me/player", Put)
  | Pause => requestBase("/me/player/pause", Put)
  | Next => requestBase("/me/player/next", Get)
  | Previous => requestBase("/me/player/previous", Get)
  };
  