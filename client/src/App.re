[%bs.raw {|require('./App.css')|}];
type state = Router.route;

module SocketClient = SocketIO.Client.Make(CodenamesSocket);

module UseSocketMake = {
    type t;
    [@bs.send]
}

module UseSocket = {
  type t;

  let emit = unit => unit;
  let on = unit => unit;
  let connect = unit => unit;
};

[@react.component]
let make = () => {
  let (route, onChangeRoute) = React.useState(_ => Router.currentRoute());

  React.useEffect0(() => {
    let watchId =
      Router.watchUrl(url => {
        let route = url |> Router.fromUrl;
        onChangeRoute(_ => route);
      });
    Some(() => Router.unwatchUrl(watchId));
  });

  switch (route) {
  | Lobby => <LobbyView />
  | Game(id) => <GameView id />
  };
};
