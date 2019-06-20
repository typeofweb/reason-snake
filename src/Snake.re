type t = {
  segments: list(MyUtils.position),
  direction: MyUtils.direction,
};

let move = (didCollectApple, {segments, direction}) => {
  // (0, 1), (0, 2)
  let [(tailX, tailY), ...xs] = segments;
  let (x, y) =
    switch (MyUtils.listTail(xs)) {
    | Some(p) => p
    | None => (tailX, tailY)
    };

  let (dx, dy) = MyUtils.directionToVector(direction);
  let newPosition = (x + dx, y + dy);

  didCollectApple
    ? {segments: [(tailX, tailY)] @ xs @ [newPosition], direction}
    : {segments: xs @ [newPosition], direction};
};

let didCollectApple = (apple, snake): bool => {
  switch (apple) {
  | None => false
  | Some(a) => snake.segments |> MyUtils.any(MyUtils.isColliding(a))
  };
};

let collidesWithSelf = (snake: t): bool => {
  let maybeHead = MyUtils.listTail(snake.segments);
  switch (maybeHead) {
  | None => false
  | Some(snakeHead) =>
    snake.segments
    |> MyUtils.any(p => p != snakeHead && MyUtils.isColliding(p, snakeHead))
  };
};

let draw = (snake, env) => {
  snake.segments
  |> List.iter(((x, y)) =>
       MyUtils.drawSquare(
         ~pos=(Board.gridSize * x, Board.gridSize * y),
         ~color=Color.snake,
         env,
       )
     );
};
