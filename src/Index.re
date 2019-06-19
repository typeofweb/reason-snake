open Reprocessing;

type gameState = {
  clock: float,
  speed: float,
  step: bool,
};

let setup = (env): gameState => {
  Env.size(~width=600, ~height=600, env);
  {clock: 0.0, speed: 1.0, step: false};
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

let makeStep = (state): gameState => {
  // @todo
  state;
};

let draw = (state, env) => {
  Draw.background(Utils.color(~r=255, ~g=217, ~b=229, ~a=255), env);
  Draw.fill(Utils.color(~r=41, ~g=166, ~b=244, ~a=255), env);
  Draw.rect(~pos=(150, 150), ~width=300, ~height=300, env);

  if (state.step) {
    makeStep(state)->updateClock(env);
  } else {
    updateClock(state, env);
  };
};

run(~setup, ~draw, ());
