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

app.get("/words", (req, res) => {
  const words = [];
  const rl = readline.createInterface({
    input: fs.createReadStream("./words.txt"),
    crlfDelay: Infinity
  });

  rl.on("line", line => {
    words.push(line);
  });

  if (words.length === 25) {
    res.json(words);
  }
});

io.on("connection", client => {
  client.on("join", game => {
    console.log("joined", game);
    client.join(game);
    io.emit("joined", game);
  });

  client.on("selectTile", data => {
    io.emit("updatedTile", data.index);
  });

  client.on("disconnect", function() {
    io.emit("user disconnected");
  });
});

server.listen(process.env.PORT || 3001, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});
