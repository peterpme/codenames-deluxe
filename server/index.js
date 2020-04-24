var express = require("express");
var app = express();
const fs = require("fs");
var server = require("http").createServer(app);
const io = require("socket.io")(server);
const readline = require("readline");

function shuffle(a) {
  var j, x, i;
  for (i = a.length - 1; i > 0; i--) {
    j = Math.floor(Math.random() * (i + 1));
    x = a[i];
    a[i] = a[j];
    a[j] = x;
  }
  return a;
}

function color(words) {
  return words.map((word, index, array) => {
    if (index < 9) return { word, color: "red" };
    if (9 <= index < 8) return { word, color: "blue" };
    if (index < 8) return { word, color: "gray" };
    return "black";
  });
}

app.get("/words", (req, res) => {
  const words = [];
  const rl = readline.createInterface({
    input: fs.createReadStream("./words.txt"),
    crlfDelay: Infinity,
  });

  rl.on("line", (line) => {
    words.push(line);
  }).on("close", () => {
    const shuffled = shuffle(words);
    const first25 = shuffled.slice(0, 25);
    const colored = color(first25);
    res.json(shuffle(colored));
  });
});

io.on("connection", (client) => {
  client.on("join", (game) => {
    console.log("joined", game);
    client.join(game);
    io.emit("joined", game);
  });

  client.on("selectTile", (data) => {
    io.emit("updatedTile", data.index);
  });

  client.on("disconnect", function () {
    io.emit("user disconnected");
  });
});

server.listen(process.env.PORT || 3001, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});
