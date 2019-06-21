type direction = [ | `Up | `Right | `Down | `Left];

type position = (int, int);

let getLastElement = list => {
  List.nth(list, List.length(list) - 1);
};

let isJust = a =>
  switch (a) {
  | Some(_) => true
  | None => false
  };

let isNone = a => !isJust(a);

let any = (fn, list): bool => {
  list |> List.fold_left((acc, el) => acc || fn(el), false);
};

let every = (fn, list): bool => {
  list |> List.fold_left((acc, el) => acc && fn(el), true);
};

let directionToVector = direction =>
  switch (direction) {
  | `Up => (0, (-1))
  | `Right => (1, 0)
  | `Down => (0, 1)
  | `Left => ((-1), 0)
  };
