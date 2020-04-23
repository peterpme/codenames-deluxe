[@bs.module "slug"] external slug: string => string = "default";

[@react.component]
let make = () => {
  let (roomName, setRoomName) = React.useState(_ => "turtles");
  let onSubmit = event => {
    event->ReactEvent.Form.preventDefault;
    let s = slug(roomName);
    ReasonReactRouter.push("/" ++ s);
  };

  <main className="mt-12 container mx-auto max-w-md text-center">
    <h1 className="font-sans text-4xl mb-2">
      {React.string("Codenames Online")}
    </h1>
    <p className="font-sans text-md">
      {React.string(
         "Play Codenames online across multiple devices on a shared board. To create a new game or join an existing
         game, enter a game identifier and click Join Game.",
       )}
    </p>
    <div className="h-8" />
    <form onSubmit className="w-3/4 mx-auto flex items-center">
      <input
        className="block w-full h-12 px-4 py-3 leading-tight text-gray-700 bg-gray-200 appearance-none focus:outline-none focus:bg-white"
        type_="text"
        autoFocus=true
        placeholder="Game Code"
        value=roomName
        onChange={event => {
          let value = event->ReactEvent.Form.target##value;
          setRoomName(value);
        }}
      />
      <button
        type_="submit"
        className="flex-none h-12 py-2 px-4 bg-blue-400 block appearance-none text-white">
        {React.string("Join Game")}
      </button>
    </form>
  </main>;
};
