type route =
  | Lobby
  | Game(string);

let toUrl = r => {
  switch (r) {
  | Lobby => "/"
  | Game(id) => "/" ++ id
  };
};

let fromUrl = (url: ReasonReact.Router.url): route =>
  switch (url.path) {
  | [] => Lobby
  | [id] => Game(id)
  | _ => Lobby
  };

let watchUrl = ReasonReact.Router.watchUrl;
let unwatchUrl = ReasonReact.Router.unwatchUrl;
let push = (r: route) => ReasonReact.Router.push(r |> toUrl);
let currentRoute = () =>
  ReasonReact.Router.dangerouslyGetInitialUrl() |> fromUrl;
