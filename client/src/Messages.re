/* Messages */

type labelName = string;
type buttonList = list(labelName);
type numClients = int;

type msg =
  | AddButton(labelName)
  | RemoveButton(labelName);

type clientToServer =
  | Msg(msg)
  | Howdy;

type serverToClient =
  | Msg(msg)
  | ClientDelta(int)
  | Success((numClients, buttonList));
