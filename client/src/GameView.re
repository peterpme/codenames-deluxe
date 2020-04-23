open T;

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

module Tile = {
  [@react.component]
  let make = (~showAll, ~word, ~color, ~status, ~onClick) => {
    let cs =
      /* consider placing class on body and show all with css */
      switch (showAll, status) {
      | (false, Turned) => getColor(color)
      | (false, Hidden) => ""
      | (true, Hidden) => getColor(color)
      | (true, Turned) => "selected " ++ getColor(color)
      };

    let disabled = status === Turned;

    <button
      disabled
      className={
        "text-black rounded text-center cursor-pointer
        font-bold bg-white "
        ++ cs
      }
      onClick={_ => onClick(word, color)}>
      {React.string(word)}
    </button>;
  };
};

module Board = {
  [@react.component]
  let make = (~showAll, ~tiles, ~onClick) => {
    <main className="grid grid-cols-5 grid-rows-5 gap-2 h-full">
      {Belt.Array.map(tiles, t =>
         <Tile
           showAll
           word={t.word}
           onClick
           color={t.color}
           status={t.status}
         />
       )
       ->React.array}
    </main>;
  };
};

module Button = {
  [@react.component]
  let make = (~label, ~onClick) => {
    <button
      className="bg-blue-500 hover:bg-blue-600 text-white font-medium py-1 px-3
      rounded transition duration-75"
      onClick={_ => onClick()}>
      {React.string(label)}
    </button>;
  };
};

module Score = {
  [@react.component]
  let make = (~red, ~blue) => {
    <div className="flex">
      <div
        className="rounded-full w-6 h-6 bg-red-500 text-white text-center
      mx-1">
        {red |> string_of_int |> React.string}
      </div>
      {React.string("vs")}
      <div
        className="rounded-full w-6 h-6 bg-blue-500 text-white text-center
        mx-1">
        {blue |> string_of_int |> React.string}
      </div>
    </div>;
  };
};

module Header = {
  [@react.component]
  let make = (~blueScore, ~redScore, ~onClickNewGame, ~onClickSpymaster) => {
    <header className="bg-white flex justify-between items-center rounded p-1">
      <h1 className="font-bold text-lg">
        {React.string("Virtual Codenames")}
      </h1>
      <Score red=redScore blue=blueScore />
      <div className="flex">
        <Button onClick=onClickSpymaster label="Spymaster" />
        <div className="w-1" />
        <Button onClick=onClickNewGame label="New Game" />
      </div>
    </header>;
  };
};

type action =
  | ShowAll
  | Toggle(string, color);

type game = {
  blueScore: int,
  redScore: int,
  showAll: bool,
  channel: string,
  firstTurn: string,
  tiles: array(tile),
};

[@react.component]
let make = (~id) => {
  React.useEffect0(() => {
    Js.log(id);
    None;
  });

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ShowAll => {...state, showAll: !state.showAll}
        | Toggle(word, color) =>
          let tiles =
            Belt.Array.map(state.tiles, t => {
              t.word === word ? {...t, status: Turned} : t
            });

          let (blueScore, redScore) =
            switch (color) {
            | Red => (state.redScore - 1, state.blueScore)
            | Blue => (state.blueScore - 1, state.redScore)
            | _ => (state.blueScore, state.redScore)
            };

          {...state, tiles, blueScore, redScore};
        },
      {
        redScore: 9,
        blueScore: 8,
        channel: "fun-fridays",
        firstTurn: "blue",
        tiles,
        showAll: false,
      },
    );

  let onClickTile = (word, color) => {
    dispatch(Toggle(word, color));
  };

  /* let onClickShowAll = () => { */
  /*   dispatch(ShowAll); */
  /* }; */

  <div className="max-w-screen-xl mx-auto flex flex-col h-screen">
    <div className="mb-2">
      <Header
        blueScore={state.blueScore}
        redScore={state.redScore}
        onClickNewGame={_ => {Js.log("hi")}}
        onClickSpymaster={_ => {Js.log("hey")}}
      />
    </div>
    <Board showAll={state.showAll} tiles={state.tiles} onClick=onClickTile />
  </div>;
};
