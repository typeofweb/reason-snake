// In order to implement our Snake game we need to implement a bunch of functions.
// Your task will be to implement the missing parts following the instructions below.

type square = (int, int);
type segments = list(square);

type snake = {
  segments: list(MyUtils.position),
  direction: MyUtils.direction,
};

type apple = option(MyUtils.position);

type gameState = {
  clock: float,
  speed: float,
  step: bool,
  points: int,
  snake,
  apple,
  isPlaying: bool,
};

let initialState: gameState = {
  clock: 0.0,
  speed: 0.2,
  step: false,
  points: 0,
  snake: {
    segments: [(0, 0)],
    direction: `Down,
  },
  apple: None,
  isPlaying: true,
};

// # 1
// Let's start with some drawing ;) Draw a square. This function will be used for drawing the snake and apples.
// Hint 1: Use `Reprocessing.Draw.fill`, `Reprocessing.Draw.rect` and `Board.gridSize`
let drawSquare = (~pos, ~color, env) => {
  let (x, y) = pos;
  Reprocessing.Draw.fill(color, env);
  Reprocessing.Draw.rect(
    ~pos=(x * Board.gridSize, y * Board.gridSize),
    ~width=Board.gridSize,
    ~height=Board.gridSize,
    env,
  );
};

// # 2
// More drawing! Draw snake. For each of its segments, draw a square.
// Hint 1: Use `List.iter`, `drawSquare` and `Color.snake`.

let drawSnake = (snake: snake, env) => {
  snake.segments
  |> List.iter(pos => {
       drawSquare(~pos, ~color=Color.snake, env);
       ();
     });
  ();
};

// # 3
// We need to be able to extend snake in to the direction it's moving.
// Your task is to implement a function which takes a direction and a list of segments
// and extends segments in that direction.
// Note: Just extend the segments. We'll chop of the "tail" in the next exercise.
// Hint 1: You need to pattern match on `direction`.
// Hint 2: It could be one of the following values: [ | `Up | `Right | `Down | `Left ];
// Hint 3: Use `MyUtils.getLastElement`

let extendSegments = (direction: MyUtils.direction, segments) => {
  let (x, y) = MyUtils.getLastElement(segments);
  let (dx, dy) =
    switch (direction) {
    | `Up => (0, (-1))
    | `Right => (1, 0)
    | `Down => (0, 1)
    | `Left => ((-1), 0)
    };

  segments @ [(x + dx, y + dy)];
};

let getTail = segments => {
  switch (segments) {
  | [_] => segments
  | _ => List.tl(segments)
  };
};

// # 4
// Moving the Snake can be represented as extending in one direction and then removing the tail.
// That's exactly what we need to implement here.
// Hint 1: Use extendSegments and then chop off the tail.

let moveSegments = (direction: MyUtils.direction, segments) =>
  getTail(extendSegments(direction, segments));

// # 5
// Now we need to really move our snake ;) Using functions such as `extendSegments` and `moveSegments`,
// implement movement of the snake.
// Hint 1: When apple was collected just extend the snake

let moveSnake = (collectedApple, snake: snake): snake => {
  let fn = collectedApple ? extendSegments : moveSegments;
  {...snake, segments: fn(snake.direction, snake.segments)};
};

// # 6
// When snake hits the wall, it dies.
// Your task is to implement a function which checks whether snake is still in the box (a predicate).
// Long story short, you'll need to check if a given coordinate is within the boundaries of the board.
// Hint 1: Use `Board.size`
// Hint 2: Destructure `square`

let isSquareInBoard = ((x, y)) => {
  x >= 0 && x < Board.size && y >= 0 && y < Board.size;
};

// # 7
// However, snake won't be a single square. It'll be a list of such squares (we'll call it segments).
// Implement a function which checks if segments are inside the board. Use `MyUtils.every`
// Hint 1: Reuse `isSquareInBoard`

let isInBoard = segments => {
  MyUtils.every(isSquareInBoard, segments);
};

// # 8
// When you hit the arrow keys, your snake should turn. Implement just that!
let turnSnake = (direction, snake: snake): snake => {
  {...snake, direction};
};

// # 9
// You need to handle user events — when they press up, right, down or left arrow keys.
// Hint 1: You'll have to pattern match on `Reprocessing.Env.keyCode(env)`
// Hint 2: Values are `Reprocessing.Events.Up` etc…
// Hint 3: Take into account that snake can't turn left when it's going right etc.
//
let handleKeyTyped = (state: gameState, env): gameState => {
  switch (Reprocessing.Env.keyCode(env)) {
  | Reprocessing.Events.Up => {...state, snake: turnSnake(`Up, state.snake)}
  | Reprocessing.Events.Right => {
      ...state,
      snake: turnSnake(`Right, state.snake),
    }
  | Reprocessing.Events.Down => {
      ...state,
      snake: turnSnake(`Down, state.snake),
    }
  | Reprocessing.Events.Left => {
      ...state,
      snake: turnSnake(`Left, state.snake),
    }
  | Reprocessing.Events.Q => initialState
  | _ => state
  };
};

// # 10
// On the board, apples should appear at random positions. Implement a function which makes some apples!
// Note: We expect this function to return `Some` because existence of an apple is optional
// Hint 1: Use `Reprocessing.Utils.random`
// Hint 2: Use `Board.size`

let makeApple = () => {
  let (x, y) = (
    Reprocessing.Utils.random(~min=0, ~max=Board.size),
    Reprocessing.Utils.random(~min=0, ~max=Board.size),
  );
  Some((x, y));
};

// # 11
// Even more drawing ;) The apple we just created — you need to draw it too!
// Hint 1: Use `Color.apple`.
// Hint 2: You need to pattern match the apple because it could be None

let drawApple = (apple, env) => {
  switch (apple) {
  | Some(a) => drawSquare(~pos=a, ~color=Color.apple, env)
  | _ => ()
  };
};

// # 12
// It's time to implement collisions! We need to know when our snake hits an apple (which is good
// and you get a point) or hits its own tail (which is bad and you lose).
// Your task is to implement 3 functions.
// `isColliding` — check if two points are colliding
// `collidesWithSelf` — check if any of the segments is colliding with it's head
// `didCollectApple` — check if head is colliding with an apple
// Hint 1: Compare coordinates
// Hint 2: Use `MyUtils.any`
// Hint 3: Make sure you're not comparing a square with itself
// Hint 4: You'll need to patternmatch on Apple because it can be Some(a) or None

let isColliding = (a: square, b: square) => false;

let collidesWithSelf = snake => false;

let didCollectApple = (apple, snake) => false;

// # 13
// Display the amount of point on the screen.
// Hint 1: Use `Reprocessing.Draw.text` and `string_of_int`
let drawPoints = (state: gameState, env) => ();

// # 14
// We need to display a message when snake dies.
// Create a function which displays a message, similar to the one with points.
let drawDeadMessage = (_state: gameState, env) => ();

// # 15
// We're done! Open Game.re and Myutils.re to see how the "engine" of the game works.
// It's just a few more lines of code!

// Try modifying the game and see what effects you can add.
// Some ideas:
// - snake speeds up as you progreess
// - snake changes color as you progress
// - apples have random color
// - apples are somehow animated (colors)
// - apples rotate
// - add walls inside the board
