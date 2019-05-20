let fetchWithAuth = (url, authHeader) =>
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": authHeader}),
      (),
    ),
  );

let putWithAuth = (url, authHeader, payload) => {
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": authHeader}),
      ~method_=Put,
      ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
      (),
    ),
  );
}

let rec join = (char: string, list: list(string)): string => {
  switch (list) {
  | [] => Failure("Passed an empty list") |> raise
  | [tail] => tail
  | [head, ...tail] => head ++ char ++ join(char, tail)
  };
};