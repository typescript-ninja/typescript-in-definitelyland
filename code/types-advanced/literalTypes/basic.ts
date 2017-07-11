// "文字列" が 型 です。値ではない！
type Suit = "Heart" | "Diamond" | "Club" | "Spade";

// OK
let suit = "Heart";
// NG suitの型に含まれていない
// error TS2322: Type '"Joker"' is not
//   assignable to type '"Heart" | "Diamond" | "Club" | "Spade"'.
// suit = "Joker";

// number, boolean, enum
type Digit = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7;
type Bool = true | false;
type Falsy = false | 0 | "" | null | undefined;

enum SuitEnum {
	Heart,
	Diamond,
	Club,
	Spade,
}
type RedSuit = SuitEnum.Heart | SuitEnum.Diamond;

export { Suit, suit, Digit, Bool, Falsy, SuitEnum, RedSuit }
