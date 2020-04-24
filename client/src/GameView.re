open T;

module MyClient = BsSocket.Client.Make(CodeNamesGame);

module Tile = {
  [@react.component]
  let make = (~word, ~color, ~status, ~onClick) => {
    let cs =
      switch (status) {
      | Turned => "selected " ++ getColor(color)
      | Hidden => "bg-white"
      };

    let disabled = status === Turned;

    <button
      disabled
      className={
        "text-black rounded text-center cursor-pointer font-bold " ++ cs
      }
      onClick={_ => onClick(word, color)}>
      {React.string(word)}
    </button>;
  };
};

module Board = {
  [@react.component]
  let make = (~showAll, ~tiles, ~onClick) => {
    let showAllClass = showAll === true ? " spymaster" : "";
    let className =
      "grid grid-cols-5 grid-rows-5 gap-2 h-full" ++ showAllClass;
    <main className>
      {Belt.Array.map(tiles, t =>
         <Tile word={t.word} onClick color={t.color} status={t.status} />
       )
       ->React.array}
    </main>;
  };
};

module Button = {
  [@react.component]
  let make = (~label, ~onClick) => {
    <button
      className="bg-blue-500 hover:bg-blue-600 text-white font-medium text-sm py-1 px-4
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
      <div className="flex items-center">
        <h1 className="font-bold text-md">
          {React.string("Codenames Deluxe")}
          <span className="ml-2 font-medium text-sm">
            {React.string("| turtles")}
          </span>
        </h1>
      </div>
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

/* type action = ToggleSpymaster | UpdateGame | UpdateBoard */

type game = {
  blueScore: int,
  redScore: int,
  showAll: bool,
  channel: string,
  firstTurn: string,
  tiles: array(tile),
};

let defaultState = {
  redScore: 9,
  blueScore: 8,
  channel: "fun-fridays",
  firstTurn: "blue",
  tiles,
  showAll: false,
};

[@react.component]
let make = (~id) => {
  React.useEffect0(() => {
    let socket = SocketClient.create();

    SocketClient.on(socket, Codenames.updateboard, data => {
      dispatch(UpdateBoard(data))
    });

    Js.log(id);
    Js.log("get worrds here");
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
      defaultState,
    );

  let onClickTile = (word, color) => {
    Js.log(word);
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
        onClickSpymaster={_ => dispatch(ShowAll)}
      />
    </div>
    <Board showAll={state.showAll} tiles={state.tiles} onClick=onClickTile />
  </div>;
};
