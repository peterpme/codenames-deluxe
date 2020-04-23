type player =
  | Spymaster
  | Regular;

type status =
  | Hidden
  | Turned;

type color =
  | Black
  | Blue
  | Red
  | Neutral;

type tile = {
  word: string,
  status,
  color,
};

let getColor = color =>
  switch (color) {
  | Blue => "blue"
  | Red => "red"
  | Black => "black"
  | Neutral => "gray"
  };
