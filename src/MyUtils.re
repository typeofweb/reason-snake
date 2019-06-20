type direction = [ | `Up | `Right | `Down | `Left];

type position = (int, int);

let listTail = l => {
  switch (l) {
  | [] => None
  | xs => Some(List.nth(xs, List.length(xs) - 1))
  };
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

let drawSquare = (~pos: (int, int), ~color, env) => {
  Reprocessing.Draw.fill(color, env);
  Reprocessing.Draw.rect(
    ~pos,
    ~width=Board.gridSize,
    ~height=Board.gridSize,
    env,
  );
};

let directionToVector = direction =>
  switch (direction) {
  | `Up => (0, (-1))
  | `Right => (1, 0)
  | `Down => (0, 1)
  | `Left => ((-1), 0)
  };

let isColliding = ((ax, ay): (int, int), (bx, by)) => {
  ax == bx && ay == by;
};

let isInBoard = (segments): bool => {
  segments
  |> every(((x, y)) => x >= 0 && y >= 0 && x < Board.size && y < Board.size);
};
