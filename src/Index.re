open Reprocessing;

let boardSize = 30;
let gridSize = 20;

type direction =
  | Top
  | Right
  | Bottom
  | Left;

type position = (int, int);

type apple = option(position);

type snake = {
  segments: list(position),
  direction,
};

type gameState = {
  clock: float,
  speed: float,
  step: bool,
  points: int,
  snake,
  apple,
  isPlaying: bool,
};

let getSpeedFromPoints = (points: int): float => {
  // 0 -> 0.2
  // 20 -> 0.1
  // 100 -> 0.05
  0.2 -. 0.0325 *. log(float_of_int(points + 1));
};

let initialState = {
  clock: 0.0,
  speed: getSpeedFromPoints(0),
  step: false,
  points: 0,
  snake: {
    segments: [(0, 0), (0, 1), (0, 2)],
    direction: Bottom,
  },
  apple: None,
  isPlaying: true,
};

let setup = (env): gameState => {
  Env.size(~width=boardSize * gridSize, ~height=boardSize * gridSize, env);
  initialState;
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

let drawSquare = (~pos: (int, int), ~color, env) => {
  Draw.fill(color, env);
  Draw.rect(~pos, ~width=gridSize, ~height=gridSize, env);
};

let isInBoard = (segments): bool => {
  segments
  |> MyUtils.every(((x, y)) =>
       x >= 0 && y >= 0 && x < boardSize && y < boardSize
     );
};

let directionToVector = direction =>
  switch (direction) {
  | Top => (0, (-1))
  | Right => (1, 0)
  | Bottom => (0, 1)
  | Left => ((-1), 0)
  };

let isColliding = ((ax, ay), (bx, by)) => {
  ax == bx && ay == by;
};

let collidesWithSelf = (segments): bool => {
  List.tl(segments) |> MyUtils.any(isColliding(List.hd(segments)));
};

let moveSnake = ({segments, direction}, didCollectApple) => {
  // (0, 1), (0, 2)
  let [(tailX, tailY), ...xs] = segments;
  let (x, y) = List.nth(xs, List.length(xs) - 1);

  let (dx, dy) = directionToVector(direction);
  let newPosition = (x + dx, y + dy);

  switch (didCollectApple) {
  | false => {segments: xs @ [newPosition], direction}
  | true => {segments: [(tailX, tailY)] @ xs @ [newPosition], direction}
  };
};

let makeApple = () => {
  Some((
    Utils.random(~min=0, ~max=boardSize),
    Utils.random(~min=0, ~max=boardSize),
  ));
};

let didCollectApple = (state): bool => {
  let {snake, apple} = state;
  switch (apple) {
  | None => false
  | Some(a) => snake.segments |> MyUtils.any(isColliding(a))
  };
};

let makeStep = (state): gameState => {
  let collectedApple = didCollectApple(state);
  let movedSnake = moveSnake(state.snake, collectedApple);
  let canMove =
    !collidesWithSelf(movedSnake.segments) && isInBoard(movedSnake.segments);
  if (!canMove) {
    {...state, isPlaying: false};
  } else {
    let newApple =
      switch (state.apple) {
      | Some(_) when collectedApple == false => state.apple
      | _ => makeApple()
      };
    let points = collectedApple ? state.points + 1 : state.points;
    {
      ...state,
      points,
      speed: getSpeedFromPoints(points),
      snake: movedSnake,
      apple: newApple,
    };
  };
};

let drawSnake = (state, env) => {
  state.snake.segments
  |> List.iter(((x, y)) =>
       drawSquare(~pos=(gridSize * x, gridSize * y), ~color=Color.snake, env)
     );
};

let drawApple = (state, env) => {
  switch (state.apple) {
  | Some(pos) =>
    let (x, y) = pos;
    drawSquare(~pos=(gridSize * x, gridSize * y), ~color=Color.apple, env);
  | _ => ()
  };
};

let draw = (state, env) => {
  Draw.background(Color.background, env);
  drawApple(state, env);
  drawSnake(state, env);

  Draw.text(~body=string_of_int(state.points), ~pos=(0, 0), env);

  let newState =
    if (!state.isPlaying) {
      let text = "You dead!";
      let w = Draw.textWidth(~body=text, env);
      Draw.text(
        ~body=text,
        ~pos=(
          boardSize * gridSize / 2 - w / 2,
          boardSize * gridSize / 2 - 30,
        ),
        env,
      );
      state;
    } else if (state.step) {
      makeStep(state);
    } else {
      state;
    };

  newState->updateClock(env);
};

let keyTyped = (state: gameState, env): gameState => {
  let direction = state.snake.direction;

  switch (Env.keyCode(env)) {
  | Events.Up when direction == Left || direction == Right => {
      ...state,
      snake: {
        ...state.snake,
        direction: Top,
      },
    }
  | Events.Right when direction == Top || direction == Bottom => {
      ...state,
      snake: {
        ...state.snake,
        direction: Right,
      },
    }
  | Events.Down when direction == Left || direction == Right => {
      ...state,
      snake: {
        ...state.snake,
        direction: Bottom,
      },
    }
  | Events.Left when direction == Top || direction == Bottom => {
      ...state,
      snake: {
        ...state.snake,
        direction: Left,
      },
    }
  | Events.Q => initialState
  | _ => state
  };
};

run(~setup, ~draw, ~keyTyped, ());
