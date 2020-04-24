open Express;

open CodenamesSocket;

module Http = {
  type http;
  [@bs.module "http"] external make : App.t => http = "Server";
  [@bs.send] external listen : (http, int, string, unit => unit) => unit = "";
};

let port = 3333;

let host = "0.0.0.0";

let app = express();

let http = Http.make(app);

App.get(app, ~path="/hello") @@
Middleware.from((_, _) => Response.sendString("hello!!!"));

let dirname: option(string) = [%bs.node __dirname];

let publicDir =
  switch (dirname) {
  | Some(dirname) => dirname ++ "/../build"
  | None => "./build"
  };

App.use(app) @@
(Static.make(publicDir, Static.defaultOptions()) |> Static.asMiddleware);

/* Setup Sockets */
module SocketServer = SocketIO.Server.Make(CodenamesSocket);

let io = SocketServer.createWithHttp(http);

let words = [|
  "Date",
  "France",
  "Net",
  "Diamond",
  "Pass",
  "Knight",
  "Cross",
  "Tie",
  "Court",
  "Cotton",
  "Point",
  "Card",
  "Duck",
  "Star",
  "Slip",
  "Fighter",
  "Bridge",
  "Band",
  "Hood",
  "Olive",
  "Shop",
  "Match",
  "Ball",
  "Bow",
  "Mercury",
|];

module State = {
  type t = {board: Board.t};
  let make = (board_: Board.t) => {board: board_};
};

let state = ref(words |> Board.make_with_words |> State.make);

let reduce =
    (type a, state: State.t, action: CodenamesSocket.t(a), data: a)
    : State.t =>
  switch (action) {
  | UpdateCard => {board: Board.updateCard(state.board, data)}
  | ToggleRevealed => {board: Board.toggleRevealed(state.board, data)}
  | _ => state
  };

SocketServer.onConnect(
  io,
  socket => {
    open SocketServer;
    Socket.emit(socket, UpdateBoard, state^.board);
    let update = (action, data) => {
      state := reduce(state^, action, data);
      SocketServer.emit(io, UpdateBoard, state^.board);
    };
    Socket.on(socket, UpdateCard, update(UpdateCard));
    Socket.on(socket, ToggleRevealed, update(ToggleRevealed));
  },
);

let onListen = () =>
  Js.log @@ "Listening at " ++ host ++ ":" ++ string_of_int(3333);

Http.listen(http, port, host, onListen);
