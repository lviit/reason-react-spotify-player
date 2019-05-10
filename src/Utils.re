let fetchWithAuth = (url, authHeader) =>
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Authorization": authHeader}),
      (),
    ),
  );

  let rec join = (char: string, list: list(string)): string => {
    switch (list) {
    | [] => Failure("Passed an empty list") |> raise
    | [tail] => tail
    | [head, ...tail] => head ++ char ++ join(char, tail)
    };
  };