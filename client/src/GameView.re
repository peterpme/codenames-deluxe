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

type request;
type response;
[@bs.new] external makeXMLHttpRequest: unit => request = "XMLHttpRequest";
[@bs.send]
external addEventListener: (request, string, unit => unit) => unit =
  "addEventListener";
[@bs.get] external response: request => response = "response";
[@bs.send] external open_: (request, string, string) => unit = "open";
[@bs.send] external send: request => unit = "send";
[@bs.send] external abort: request => unit = "abort";

[@react.component]
let make = (~id) => {
  React.useEffect0(() => {
    let socket = SocketV2.Client.createWithUrl("http://localhost:8080");

    socket->SocketV2.Socket.onConnect(() => {
      socket->SocketV2.Socket.emit("joinGame", id)
    });

    socket->SocketV2.Socket.onEvent("joinGame", words => {
      Js.log2("joinGame", words)
    });

    let request = makeXMLHttpRequest();
    request->addEventListener("load", () => {
      Js.log(
        "hi",
        /* setState(_previousState => */
        /*   LoadedDogs(request->response->parseResponse##message) */
        /* ) */
      )
    });
    request->addEventListener("error", () => {
      Js.log(
        "error",
        /* setState(_previousState => ErrorFetchingDogs) */
      )
    });
    request->open_("GET", "https://dog.ceo/api/breeds/image/random/3");
    request->send;

    // TODO unsubscribe here
    // the return value is called by React's useEffect when the component unmounts
    Some(() => {request->abort});
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
