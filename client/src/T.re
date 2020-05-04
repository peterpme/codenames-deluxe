type player =
  | Spymaster
  | Regular;

type color =
  | Black
  | Blue
  | Red
  | Neutral;

type tile = {
  word: string,
  selected: bool,
  color,
};

let toColorString = color =>
  switch (color) {
  | Blue => "blue"
  | Red => "red"
  | Black => "black"
  | Neutral => "gray"
  };

let tiles = [|
  {word: "LEMONS", selected: false, color: Black},
  {word: "WATER", selected: false, color: Neutral},
  {word: "ORANGE", selected: false, color: Blue},
  {word: "COMPUTER", selected: false, color: Red},
  {word: "DOLPHIN", selected: false, color: Blue},
  {word: "ALASKAN", selected: false, color: Red},
  {word: "ANTARTICA", selected: false, color: Blue},
  {word: "TELEVISION", selected: false, color: Red},
  {word: "PIEKARCZYK", selected: false, color: Blue},
  {word: "MONKEY", selected: false, color: Neutral},
  {word: "KITCHEN", selected: false, color: Neutral},
  {word: "REFRIGERATOR", selected: false, color: Neutral},
  {word: "BLUEBERRIES", selected: false, color: Blue},
  {word: "TILES", selected: false, color: Red},
  {word: "FLOOR", selected: false, color: Blue},
  {word: "CONSOLE", selected: false, color: Red},
  {word: "LAPTOP", selected: false, color: Blue},
  {word: "MONITOR", selected: false, color: Red},
  {word: "MATTHEWS", selected: false, color: Blue},
  {word: "ZEBRA", selected: false, color: Neutral},
  {word: "DESK", selected: false, color: Neutral},
  {word: "FRESH", selected: false, color: Red},
  {word: "PEPSI", selected: false, color: Neutral},
  {word: "MOUSE", selected: false, color: Blue},
  {word: "KEYBOARD", selected: false, color: Red},
|];
