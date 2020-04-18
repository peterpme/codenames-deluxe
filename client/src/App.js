import React, { useEffect } from "react";
import { make as Game } from "./Game.bs";
import "./App.css";

import useSocket from "use-socket.io-client";

export default function App() {
  const [socket] = useSocket("http://localhost:8080");
  socket.connect();

  useEffect(() => {
    socket.on("connect", data => {
      socket.emit("join", "123");
    });

    socket.on("updatedTile", data => {
      console.log("new tile", data);
    });
  }, socket);

  return <Game />;
}
