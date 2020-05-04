# Codenames Deluxe 🎮

A showcase & experiment using some :fire: tools :smile:

- [ReasonReact](https://github.com/reasonml/reason-react/)
- [Create React App](https://create-react-app.dev)
- [Tailwind CSS](https://tailwindcss.com/)
- [GenType](https://github.com/cristiano/gen-type)
- [Socket.io](https://socket.io/)
- [Fly](https://fly.io)
- [Redis](https://redis.io/)

Using these tools we're able to create a infinitely scalable, globally available, playable on all platform game!

## What is Codenames?

Codenames is a 2015 card game for 4–8 players designed by Vlaada Chvátil and published by Czech Games Edition. Two teams compete by each having a "spymaster" give one-word clues that can point to multiple words on the board. The other players on the team attempt to guess their team's words while avoiding the words of the other team. In a variant with 2–3 players, one spymaster gives clues to the other player or players. Read more [here](https://en.wikipedia.org/wiki/Codenames_(board_game))


## Project Structure

### Client

Runs both create-react-app (`yarn start`) and the ReasonML compiler (`yarn re:watch`) which means you'll need two terminal windows
The client has a flat file structure so the file names should be reasonably easy to reason about (puns!!)

The file that might throw you off is `T.re` which is treated like your types / data structure file. It's a pattern that's been copied from the OCaml world. The filename doesn't mean anything, it just shorthand for `Type`.

### Server

Same idea. Two terminal windows:

Inside the `client` folder, install the dependencies `yarn` and then `yarn start` to start the server.
In another terminal window, run: `yarn re:watch` to start the reason compiler

### Contributing

Help make this project better :fire: All contributions are welcome!
