open T;

type word = {
  color: string,
  word: string,
};

type words = array(word);

type action =
  | StartGame(words)
  | Spymaster
  | SelectCard(string);

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
    let socket = SocketV2.Client.createWithUrl("http://localhost:8080");

    socket->SocketV2.Socket.onConnect(() => {
      Js.log("connected");
      socket->SocketV2.Socket.emit("joinGame", id);
    });

    socket->SocketV2.Socket.onEvent("joinGame", words => {
      Js.log2("joinGame", words)
    });

    // TODO unsubscribe here
    None;
  });

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | StartGame(words) => {...state, tiles: words}
        | Spymaster => {...state, showAll: !state.showAll}
        | SelectCard(word) =>
          let tiles =
            Belt.Array.map(state.tiles, t => {
              t.word === word ? {...t, selected: true} : t
            });

          {...state, tiles};
        },
      defaultState,
    );

  let onClickTile = word => {/* dispatch(SelectCard(word)); */};

  let onToggleSpymaster = _ => {/* dispatch(Spymaster); */};

  <div className="flex flex-col h-screen mx-auto max-w-screen-xl">
    <div className="mb-2">
      <Header
        blueScore={state.blueScore}
        redScore={state.redScore}
        onClickNewGame={_ => {Js.log("hi")}}
        onToggleSpymaster
      />
    </div>
    <Board showAll={state.showAll} tiles={state.tiles} onClick=onClickTile />
  </div>;
};
