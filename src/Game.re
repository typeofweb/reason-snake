type t = {
  clock: float,
  speed: float,
  step: bool,
  points: int,
  snake: Snake.t,
  apple: Apple.t,
  isPlaying: bool,
};

let getSpeedFromPoints = (points: int): float => {
  // 0 -> 0.2
  // 20 -> 0.1
  // 100 -> 0.05
  0.2 -. 0.0325 *. log(float_of_int(points + 1));
};

let initialState: t = {
  clock: 0.0,
  speed: getSpeedFromPoints(0),
  step: false,
  points: 0,
  snake: {
    segments: [(0, 0)],
    direction: `Down,
  },
  apple: None,
  isPlaying: true,
};

let setup = (env): t => {
  Reprocessing.Env.size(
    ~width=Board.size * Board.gridSize,
    ~height=Board.size * Board.gridSize,
    env,
  );
  initialState;
};

let updateClock = (env, state): t => {
  let delta = Reprocessing.Env.deltaTime(env);
  let clock = state.clock +. delta;

  if (clock > state.speed) {
    {...state, clock: clock -. state.speed, step: true};
  } else {
    {...state, clock, step: false};
  };
};

let makeStep = state => {
  let collectedApple = state.snake |> Snake.didCollectApple(state.apple);
  let movedSnake = state.snake |> Snake.move(collectedApple);
  let canMove =
    !Snake.collidesWithSelf(movedSnake)
    && MyUtils.isInBoard(movedSnake.segments);
  if (!canMove) {
    {...state, isPlaying: false};
  } else {
    let newApple =
      switch (state.apple) {
      | Some(_) when collectedApple == false => state.apple
      | _ => Apple.make()
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

let drawDeadMessage = (_state, env) => {
  let text = "You dead!";
  let w = Reprocessing.Draw.textWidth(~body=text, env);
  Reprocessing.Draw.text(
    ~body=text,
    ~pos=(
      Board.size * Board.gridSize / 2 - w / 2,
      Board.size * Board.gridSize / 2 - 30,
    ),
    env,
  );
};

let drawPoints = (state, env) => {
  Reprocessing.Draw.text(
    ~body=string_of_int(state.points),
    ~pos=(0, 0),
    env,
  );
};

let draw = (state, env) => {
  Reprocessing.Draw.background(Color.background, env);
  state.apple->Apple.draw(env);
  state.snake->Snake.draw(env);

  drawPoints(state, env);

  let newState =
    if (!state.isPlaying) {
      drawDeadMessage(state, env);
      state;
    } else if (state.step) {
      makeStep(state);
    } else {
      state;
    };

  newState |> updateClock(env);
};
