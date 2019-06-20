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
