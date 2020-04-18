let make = () => {
  let url = ReasonReactRouter.useUrl();
  switch (url.path) {
  | [id] => <Game id />
  | [] => <JoinGame />
  | _ => <JoinGame />
  };
};
