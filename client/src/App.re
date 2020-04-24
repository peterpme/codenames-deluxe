[%bs.raw {|require('./App.css')|}];
type state = Router.route;

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
