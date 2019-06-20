type t = option(MyUtils.position);

let draw = (apple, env) => {
  switch (apple) {
  | Some(pos) =>
    let (x, y) = pos;
    MyUtils.drawSquare(
      ~pos=(Board.gridSize * x, Board.gridSize * y),
      ~color=Color.apple,
      env,
    );
  | _ => ()
  };
};

let make = () => {
  Some((
    Reprocessing.Utils.random(~min=0, ~max=Board.size),
    Reprocessing.Utils.random(~min=0, ~max=Board.size),
  ));
};
