open Webapi;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

type settings = {
  authHeader: string,
  deviceId: string,
};

let url = Url.URLSearchParams.make(hash);
let accessToken = Url.URLSearchParams.get("#access_token", url);
let authHeader =
  switch (accessToken) {
  | None => "none"
  | Some(accessToken) => "Bearer " ++ accessToken
  };

let deviceId =
  switch (Url.URLSearchParams.get("device_id", url)) {
  | None => "none"
  | Some(deviceId) => deviceId
  };

let settingsContext = React.createContext({authHeader, deviceId});
let provider = React.Context.provider(settingsContext);

[@react.component]
let make = (~children) => {
  <provider> children </provider>;
};