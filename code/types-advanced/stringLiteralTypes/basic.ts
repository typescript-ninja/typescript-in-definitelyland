// "文字列" が 型 です。値ではない！
let suit: "Heart" | "Diamond" | "Club" | "Spade";

// OK
suit = "Heart";
// NG suitの型に含まれていない
// error TS2322: Type '"Joker"' is not assignable to type '"Heart" | "Diamond" | "Club" | "Spade"'.
// suit = "Joker";

export { }
