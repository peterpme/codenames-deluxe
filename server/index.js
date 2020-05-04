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
  return words.map((word, index) => {
    if (index < 9) return { word, color: "red" };
    if (9 <= index < 8) return { word, color: "blue" };
    if (index < 8) return { word, color: "gray" };
    return "black";
  });
}

async function getWords() {
  const words = [];
  const rl = readline.createInterface({
    input: fs.createReadStream("./words.txt"),
    crlfDelay: Infinity,
  });

  return new Promise((resolve, reject) => {
    rl.on("line", (line) => {
      words.push(line);
    }).on("close", () => {
      const shuffled = shuffle(words);
      const first25 = shuffled.slice(0, 25);
      const colored = color(first25);
      resolve(shuffle(colored));
    });
  });
}

class Game {
  constructor(id) {
    this.id = id;
    this.createdAt = new Date();
    this.updatedAt = new Date();
    this.startingTeam = this.getStartingTeam();
    this.words = this.getWords();
    this.round = 0;
  }

  getStartingTeam() {
    const random = Math.random() >= 0.5;
    return random ? "red" : "blue";
  }

  async getWords() {
    this.words = await getWords();
  }

  updateWord(word) {
    this.words = this.words.map((word) => {
      if (word.word === word) {
        return {
          ...word,
          selected: true,
        };
      }

      return word;
    });
  }
}

app.get("game/:id", async (req, res) => {
  const gameId = req.params.id;
  const game = Game.get(id);
  if (game) {
    return game;
  } else {
    const game = new Game(gameId);
    return game;
  }
});

function createRoom(data) {
  console.log(data);
}

function joinRoom(data) {
  console.log(data);
}

// user connects to socket
io.on("connection", async (socket) => {
  console.log("connected", socket.id);

  // emit game: score & words
  const words = await getWords();
  console.log(words.length);
  socket.emit("joinGame", words);

  // server reacts to client creating room
  socket.on("createRoom", (data) => createRoom(data));

  socket.on("joinRoom", (data) => joinRoom(data));

  socket.on("disconnect", function () {
    socket.emit("user disconnected");
  });
});

server.listen(process.env.PORT || 8080, () => {
  console.log(`Server started on port:${server.address().port}`);
});
