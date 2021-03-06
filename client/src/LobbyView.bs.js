// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE

import * as Slug from "slug";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as ReasonReactRouter from "reason-react/src/ReasonReactRouter.js";

function LobbyView$Mo(Props) {
  return Curry._1(Props.render, /* () */0);
}

var Mo = {
  make: LobbyView$Mo
};

function LobbyView(Props) {
  var match = React.useState((function () {
          return "turtles";
        }));
  var setRoomName = match[1];
  var roomName = match[0];
  var onSubmit = function ($$event) {
    $$event.preventDefault();
    var s = Slug.default(roomName);
    return ReasonReactRouter.push("/" + s);
  };
  return React.createElement("main", {
              className: "container max-w-md mx-auto mt-12 text-center"
            }, React.createElement("h1", {
                  className: "mb-2 font-sans text-4xl"
                }, "Codenames Online"), React.createElement("p", {
                  className: "font-sans text-md"
                }, "Play Codenames online across multiple devices on a shared board. To create a new game or join an existing\n                     game, enter a game identifier and click Join Game."), React.createElement("div", {
                  className: "h-8"
                }), React.createElement("form", {
                  className: "flex items-center w-3/4 mx-auto",
                  onSubmit: onSubmit
                }, React.createElement("input", {
                      className: "block w-full h-12 px-4 py-3 leading-tight text-gray-700 bg-gray-200 appearance-none focus:outline-none focus:bg-white",
                      autoFocus: true,
                      placeholder: "Game Code",
                      type: "text",
                      value: roomName,
                      onChange: (function ($$event) {
                          return Curry._1(setRoomName, $$event.target.value);
                        })
                    }), React.createElement("button", {
                      className: "flex-none block h-12 px-4 py-2 text-white bg-blue-400 appearance-none",
                      type: "submit"
                    }, "Join Game"), React.createElement(LobbyView$Mo, {
                      render: (function (param) {
                          return React.createElement("div", undefined);
                        })
                    })));
}

var make = LobbyView;

export {
  Mo ,
  make ,
  
}
/* slug Not a pure module */
