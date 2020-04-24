type t('a) =
  | UpdateCard: t(Board.card)
  | ToggleRevealed: t(string)
  | UpdateBoard: t(Board.t);

let stringify = (type a, t: t(a)) =>
  switch (t) {
  | UpdateCard => "UpdateCard"
  | ToggleRevealed => "ToggleRevealed"
  | UpdateBoard => "UpdateBoard"
  };
