open Webapi;

[@bs.val] [@bs.scope ("window", "location")] external hash: string = "hash";

[@react.component]
let make = () => {
  let url = Url.URLSearchParams.make(hash);
  let accessToken = Url.URLSearchParams.get("#access_token", url);
  let authHeader =
    switch (accessToken) {
    | None => "none"
    | Some(accessToken) => "Bearer " ++ accessToken
    };

  <div> <Component2 authHeader /> <NewReleases authHeader/> </div>;
};