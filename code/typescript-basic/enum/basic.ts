enum Suit {
	Heart,
	Diamond,
	Club,
	Spade,
}
// 0, 'Heart' と表示される
console.log(Suit.Heart, Suit[Suit.Heart]);

const enum Permission {
	Execute = 1,
	Read = 2,
	Write = 4,
	All = Execute | Read | Write,
}
// 7 と表示される
console.log(Permission.All);

enum Tree {
	Node = "node",
	Leaf = "leaf",
}
// node と表示される
console.log(Tree.Node);

export { Suit, Permission, Tree }
