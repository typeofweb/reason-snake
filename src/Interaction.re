let handleKeyTyped = (state: Game.t, env): Game.t => {
  let direction = state.snake.direction;

  switch (Reprocessing.Env.keyCode(env)) {
  | Reprocessing.Events.Up when direction == `Left || direction == `Right => {
      ...state,
      snake: state.snake |> Snake.turn(`Up),
    }
  | Reprocessing.Events.Right when direction == `Up || direction == `Down => {
      ...state,
      snake: state.snake |> Snake.turn(`Right),
    }
  | Reprocessing.Events.Down when direction == `Left || direction == `Right => {
      ...state,
      snake: state.snake |> Snake.turn(`Down),
    }
  | Reprocessing.Events.Left when direction == `Up || direction == `Down => {
      ...state,
      snake: state.snake |> Snake.turn(`Left),
    }
  | Reprocessing.Events.Q => Game.initialState
  | _ => state
  };
};
