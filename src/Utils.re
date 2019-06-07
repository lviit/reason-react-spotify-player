let rec join = (char: string, list: list(string)): string =>
  switch (list) {
  | [] => Failure("Passed an empty list") |> raise
  | [tail] => tail
  | [head, ...tail] => head ++ char ++ join(char, tail)
  };