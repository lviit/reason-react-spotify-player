let rec join = (char: string, list: list(string)): string =>
  switch (list) {
  | [] => Failure("Passed an empty list") |> raise
  | [tail] => tail
  | [head, ...tail] => head ++ char ++ join(char, tail)
  };

let formatDuration = (progress: int): string => {
  let minutes = string_of_int(progress / 60);
  let seconds =
    switch (progress) {
    | progress when progress mod 60 < 10 => "0" ++ string_of_int(progress mod 60)
    | progress when progress >= 60 => string_of_int(progress mod 60)
    | _ => string_of_int(progress)
    };

  minutes ++ ":" ++ seconds;
};