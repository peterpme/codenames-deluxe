module Button = {
  [@react.component]
  let make = (~label, ~onClick) => {
    <button
      className="px-4 py-1 text-sm font-medium text-white bg-blue-500 rounded hover:bg-blue-600 transition duration-75"
      onClick={_ => onClick()}>
      {React.string(label)}
    </button>;
  };
};

module Score = {
  [@react.component]
  let make = (~red, ~blue) => {
    <div className="flex">
      <div className="w-6 h-6 mx-1 text-center text-white bg-red-500 rounded-full">
        {red |> string_of_int |> React.string}
      </div>
      {React.string("vs")}
      <div
        className="w-6 h-6 mx-1 text-center text-white bg-blue-500 rounded-full">
        {blue |> string_of_int |> React.string}
      </div>
    </div>;
  };
};

[@react.component]
let make = (~blueScore, ~redScore, ~onClickNewGame, ~onToggleSpymaster) => {
  <header className="flex items-center justify-between p-1 bg-white rounded">
    <div className="flex items-center">
      <h1 className="font-bold text-md">
        {React.string("Codenames Deluxe")}
        <span className="ml-2 text-sm font-medium">
          {React.string("| turtles")}
        </span>
      </h1>
    </div>
    <Score red=redScore blue=blueScore />
    <div className="flex">
      <Button onClick=onToggleSpymaster label="Spymaster" />
      <div className="w-1" />
      <Button onClick=onClickNewGame label="New Game" />
    </div>
  </header>;
};

