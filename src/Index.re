open Reprocessing;

type direction =
  | Top
  | Right
  | Bottom
  | Left;

type position = (int, int);

type apple = option(position);

type gameState = {
  clock: float,
  speed: float,
  step: bool,
  snakePosition: position,
  snakeDirection: direction,
  apple,
};

let setup = (env): gameState => {
  Env.size(~width=600, ~height=600, env);
  {
    clock: 0.0,
    speed: 0.5,
    step: false,
    snakePosition: (0, 0),
    snakeDirection: Bottom,
    apple: None,
  };
};

let updateClock = (state, env): gameState => {
  let delta = Env.deltaTime(env);
  let clock = state.clock +. delta;

  if (clock > state.speed) {
    {...state, clock: clock -. state.speed, step: true};
  } else {
    {...state, clock, step: false};
  };
};

let drawSquare = (~pos: (int, int), env) => {
  Draw.rect(~pos, ~width=10, ~height=10, env);
};

let isInBoard = ((x, y)): bool => {
  x >= 0 && y >= 0 && x <= 600 && y <= 600;
};

let move = (position, direction) => {
  let (x, y) = position;
  let (dx, dy) =
    switch (direction) {
    | Top => (0, (-1))
    | Right => (1, 0)
    | Bottom => (0, 1)
    | Left => ((-1), 0)
    };
  let newPosition = (x + 10 * dx, y + 10 * dy);

  if (isInBoard(newPosition)) {
    newPosition;
  } else {
    position;
  };
};

let makeApple = () => {
  Some((Utils.random(~min=0, ~max=600), Utils.random(~min=0, ~max=600)));
};

let makeStep = (state): gameState => {
  let newPosition = move(state.snakePosition, state.snakeDirection);
  let newApple =
    switch (state.apple) {
    | None => makeApple()
    | Some(_) => state.apple
    };
  {...state, snakePosition: newPosition, apple: newApple};
};

let draw = (state, env) => {
  Draw.background(Utils.color(~r=255, ~g=217, ~b=229, ~a=255), env);
  Draw.fill(Utils.color(~r=41, ~g=166, ~b=244, ~a=255), env);
  Draw.rect(~pos=(150, 150), ~width=300, ~height=300, env);
  drawSquare(~pos=state.snakePosition, env);

  switch (state.apple) {
  | Some(pos) => drawSquare(~pos, env)
  | None => ()
  };

  if (state.step) {
    makeStep(state)->updateClock(env);
  } else {
    updateClock(state, env);
  };
};

let keyTyped = (state, env) => {
  switch (Env.keyCode(env)) {
  | Events.Up => {...state, snakeDirection: Top}
  | Events.Right => {...state, snakeDirection: Right}
  | Events.Down => {...state, snakeDirection: Bottom}
  | Events.Left => {...state, snakeDirection: Left}
  | _ => state
  };
};

run(~setup, ~draw, ~keyTyped, ());
