open T;
module Tile = {
  [@react.component]
  let make = (~word, ~color, ~selected, ~onClick) => {
    let className =
      Cn.make([
        "text-black rounded text-center cursor-pointer font-bold bg-white",
        "selected"->Cn.ifTrue(selected),
        T.toColorString(color),
      ]);

    <button className disabled=selected onClick={_ => onClick(word)}>
      {React.string(word)}
    </button>;
  };
};

[@react.component]
let make = (~showAll, ~tiles, ~onClick) => {
  let className =
    Cn.make([
      "h-full grid grid-cols-5 grid-rows-5 gap-2",
      "spymaster"->Cn.ifTrue(showAll),
    ]);

  <main className>
    {Belt.Array.map(tiles, t =>
       <Tile word={t.word} onClick color={t.color} selected={t.selected} />
     )
     ->React.array}
  </main>;
};
