let setup = (env): Exercises.gameState => {
  Reprocessing.Env.size(
    ~width=Board.size * Board.gridSize,
    ~height=Board.size * Board.gridSize,
    env,
  );
  Exercises.initialState;
};

let makeStep = (state: Exercises.gameState) => {
  let collectedApple = state.snake |> Exercises.didCollectApple(state.apple);
  let movedSnake = state.snake |> Exercises.moveSnake(collectedApple);
  let canMove =
    !Exercises.collidesWithSelf(movedSnake)
    && Exercises.isInBoard(movedSnake.segments);
  if (!canMove) {
    {...state, isPlaying: false};
  } else {
    let newApple =
      switch (state.apple) {
      | Some(_) when collectedApple == false => state.apple
      | _ => Exercises.makeApple()
      };
    let points = collectedApple ? state.points + 1 : state.points;
    {...state, points, snake: movedSnake, apple: newApple};
  };
};

let updateClock = (env, state: Exercises.gameState): Exercises.gameState => {
  let delta = Reprocessing.Env.deltaTime(env);
  let clock = state.clock +. delta;

  if (clock > state.speed) {
    {...state, clock: clock -. state.speed, step: true};
  } else {
    {...state, clock, step: false};
  };
};

let draw = (state: Exercises.gameState, env) => {
  Reprocessing.Draw.background(Color.background, env);
  state.apple->Exercises.drawApple(env);
  state.snake->Exercises.drawSnake(env);

  Exercises.drawPoints(state, env);

  let newState =
    if (!state.isPlaying) {
      Exercises.drawDeadMessage(state, env);
      state;
    } else if (state.step) {
      makeStep(state);
    } else {
      state;
    };

  newState |> updateClock(env);
};
