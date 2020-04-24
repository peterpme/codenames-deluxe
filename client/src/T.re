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

let tiles = [|
  {word: "LEMONS", status: Hidden, color: Black},
  {word: "WATER", status: Hidden, color: Neutral},
  {word: "ORANGE", status: Hidden, color: Blue},
  {word: "COMPUTER", status: Hidden, color: Red},
  {word: "DOLPHIN", status: Hidden, color: Blue},
  {word: "ALASKAN", status: Hidden, color: Red},
  {word: "ANTARTICA", status: Hidden, color: Blue},
  {word: "TELEVISION", status: Hidden, color: Red},
  {word: "PIEKARCZYK", status: Hidden, color: Blue},
  {word: "MONKEY", status: Hidden, color: Neutral},
  {word: "KITCHEN", status: Hidden, color: Neutral},
  {word: "REFRIGERATOR", status: Hidden, color: Neutral},
  {word: "BLUEBERRIES", status: Hidden, color: Blue},
  {word: "TILES", status: Hidden, color: Red},
  {word: "FLOOR", status: Hidden, color: Blue},
  {word: "CONSOLE", status: Hidden, color: Red},
  {word: "LAPTOP", status: Hidden, color: Blue},
  {word: "MONITOR", status: Hidden, color: Red},
  {word: "MATTHEWS", status: Hidden, color: Blue},
  {word: "ZEBRA", status: Hidden, color: Neutral},
  {word: "DESK", status: Hidden, color: Neutral},
  {word: "FRESH", status: Hidden, color: Red},
  {word: "PEPSI", status: Hidden, color: Neutral},
  {word: "MOUSE", status: Hidden, color: Blue},
  {word: "KEYBOARD", status: Hidden, color: Red},
|];
