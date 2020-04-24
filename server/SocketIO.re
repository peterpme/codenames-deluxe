module Json = {
  /* @Performance
       Special serialization logic because we cannot serialize variants using Json.stringify.
       The reason for that is that Variants are represented as arrays under the hood with a property .tag
       representing the tag kind. That property isn't one that's normally on array so Json.stringify won't serialize it.
       We have to resort to using our own encoding.
             Ben - July 24 2017
     */
  let toValidJson = [%bs.raw
    {|
function toJson(o) {
  switch (typeof o){
    case "boolean":
    case "number":
    case "string":
      return o;
    case "function":
      throw new Error("Cannot serialize functions");
    case "object":
      if (Array.isArray(o)){
        return [o.hasOwnProperty("tag") ? o.tag : -1, o.map(toJson)];
      }
      throw new Error("Cannot serialize unidentified object [" + o + "].")
  }
}
|}
  ];
  /* let toValidJson = obj : 'a =>
     switch (obj |> classify) {
     | JSFalse
     | JSTrue
     | JSNull
     | JSUndefined
     | JSNumber(_)
     | JSString(_)
     | JSSymbol(_) => obj
     | JSFunction(_) =>
       raise(Js.Exn.raiseTypeError("Cannot serialize functions"))
     | JSObject(_) => Js.Array.isArray(obj) ?
     }; */
  let fromValidJson = [%bs.raw
    {|
function fromJson(o) {
  switch (typeof o){
    case "boolean":
    case "number":
    case "string":
      return o;
    case "function":
      throw new Error("Cannot deserialize functions");
    case "object":
      if (Array.isArray(o)){
        var first = o[0]
        if (first == -1){
          return o[1].map(fromJson);
        } else {
          var a = o[1].map(fromJson);
          a.tag = first;
          return a
        }
      }
      throw new Error("Cannot deserialize unidentified object [" + o + "].")
  }
}
|}
  ];
};

module Common = {
  /* Module to implement to get an instance of the server module.

       Example:

        module InnerServer = Server.Make({
          type t('a) = string;
          let stringify(t) => t;
        });

        let io = InnerServer.create();

        `stringify` is used to turn the event type to a string which is used to
        identify the packet.
     */
  module type M_t = {type t('a); let stringify: t('a) => string;};
};

module Client = {
  module Make = (M: Common.M_t) => {
    type t;
    [@bs.new] external create : unit => t = "io";
    [@bs.send] external _emit : (t, string, 'a) => unit = "emit";
    let emit = (socket, message: M.t('a), obj: 'a) =>
      _emit(socket, M.stringify(message), Json.toValidJson(obj));
    [@bs.send] external _on : (t, string, 'a => unit) => unit = "on";
    let on = (socket, message: M.t('a), func: 'a => unit) =>
      _on(socket, M.stringify(message), obj =>
        func(Json.fromValidJson(obj))
      );
  };
};

module Server = {
  type serverT;
  type socketT;
  module Make = (M: Common.M_t) => {
    [@bs.module] external create : unit => serverT = "socket.io";
    [@bs.module] external createWithHttp : 'a => serverT = "socket.io";

    /***
     * makeOptions is a simple way to get around the fact that ocaml won't allow you to declare
     * partially defined records (and row polymorphism + ad hoc polymorphism is tricky in ocaml)
     * This allows you to create a JS object with any of the defined properties, allowing to omit
     * any number of them.
     */
    type createOptionsT;
    [@bs.obj]
    external makeOptions :
      (
        ~pingTimeout: int=?,
        ~pingInterval: int=?,
        ~maxHttpBufferSize: int=?,
        ~transports: list(string)=?,
        ~allowUpgrades: Js.boolean=?,
        ~perMessageDeflate: int=?,
        ~httpCompression: int=?,
        ~cookie: string=?,
        ~cookiePath: string=?,
        ~wsEngine: string=?,
        unit
      ) =>
      createOptionsT =
      "";
    [@bs.module]
    external createWithOptions : createOptionsT => serverT = "socket.io";
    [@bs.module]
    external createWithHttpAndOption : ('a, createOptionsT) => serverT =
      "socket.io";
    [@bs.module]
    external createWithPort : (int, createOptionsT) => serverT = "socket.io";

    /*** */
    [@bs.send]
    external serveClient : (serverT, Js.boolean) => serverT = "serveClient";

    /*** */
    [@bs.send] external path : (serverT, string) => serverT = "path";

    /***
     * This kind of function is annoying because it relies on the type of another module which you might not
     * care about (here https://github.com/socketio/socket.io-adapter), and which is defined by someone else.
     * The only idea I have to typecheck this is to make sure this type is a module type representing an
     * interface, and have the external package also use that module signature. The problem is to keep them in
     * sync. No clue how to do that.
     */
    [@bs.send] external adapter : (serverT, 'a) => serverT = "adapter";

    /*** */
    [@bs.send] external origins : (serverT, string) => serverT = "origins";
    [@bs.send]
    external originsWithFunc : (serverT, ('a, bool) => unit) => serverT =
      "origins";

    /*** */
    [@bs.send] external close : serverT => unit = "close";

    /*** This is the same as "server.listen". */
    [@bs.send]
    external attach : (serverT, 'a, createOptionsT) => serverT = "attach";
    [@bs.send]
    external attachWithPort : (serverT, int, createOptionsT) => serverT =
      "attach";

    /*** Emit is reused below. That's why it has those abstract types. */
    [@bs.send] external _emit : ('a, string, 'b) => unit = "emit";
    let emit = (server: serverT, t: M.t('a), obj: 'a) : unit =>
      _emit(server, M.stringify(t), Json.toValidJson(obj));

    /***
     * socketT is the representation of a connection received by the server.
     * It's a pipeline through which one can emit and listen to events.
     */
    module Socket = {
      [@bs.get] external getId : socketT => string = "id";
      [@bs.get] external getRooms : socketT => Js.t('a) = "rooms";
      [@bs.get] external getHandshake : socketT => Js.t('a) = "handshake";
      /* Here 'a means that you can send anything you want, and it'll depend on
         Bucklescript */
      [@bs.send]
      external _on :
        (socketT, [@bs.ignore] M.t('a), string, 'a => unit) => unit =
        "on";
      /* let on = (type a, socket, t: M.t(a), func: M.t(a) => unit) =>
         _on(socket, t, M.stringify(t), obj =>
           func(Json.fromValidJson(obj))
         ); */
      let on = (socket, t, func) =>
        _on(socket, t, M.stringify(t), obj =>
          func(Json.fromValidJson(obj))
        );

      /*** */
      let emit = (socket: socketT, t: M.t('a), obj: 'a) =>
        _emit(socket, M.stringify(t), Json.toValidJson(obj));

      /*** */
      type broadcastT;
      [@bs.get]
      external _unsafeGetBroadcast : socketT => broadcastT = "broadcast";
      let broadcast = (socket, str, data) =>
        _emit(
          _unsafeGetBroadcast(socket),
          M.stringify(str),
          Json.toValidJson(data),
        );

      /*** */
      [@bs.send]
      external join : (socketT, string, 'a => unit) => socketT = "join";
      [@bs.send]
      external leave : (socketT, string, 'a => unit) => socketT = "leave";
      [@bs.send] external to_ : (socketT, string) => socketT = "to";
      [@bs.send]
      external compress : (socketT, Js.boolean) => socketT = "compress";
      [@bs.send]
      external disconnect : (socketT, Js.boolean) => socketT = "disconnect";
      [@bs.send]
      external use : (socketT, ('a, unit => unit) => unit) => unit = "use";

      /*** */
      [@bs.send] external _once : (socketT, string, 'b) => unit = "once";
      let once = (socket, t, func) =>
        _once(socket, M.stringify(t), obj => func(Json.fromValidJson(obj)));

      /*** Volatile */
      type volatileT;
      [@bs.get] external getVolatile : socketT => volatileT = "volatile";
      [@bs.send]
      external _volatileEmit : (volatileT, string, 'a) => unit = "emit";
      let volatileEmit = (server: socketT, event: M.t('a), obj: 'a) : unit =>
        _volatileEmit(
          getVolatile(server),
          M.stringify(event),
          Json.toValidJson(obj),
        );
    };
    [@bs.send]
    external _unsafeOnConnect : (serverT, string, socketT => unit) => unit =
      "on";
    let onConnect = (io, cb) => _unsafeOnConnect(io, "connection", cb);
  };
};
