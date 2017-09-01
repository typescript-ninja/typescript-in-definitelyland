// 足し算
interface Add {
	type: "add";
	left: Tree;
	right: Tree;
}
// 末端の値
interface Leaf {
	type: "leaf";
	value: number;
}

type Tree = Add | Leaf;

// (10 + 3) + 5 を表現する
let node: Tree = {
	type: "add",
	left: {
		type: "add",
		left: { type: "leaf", value: 10 },
		right: { type: "leaf", value: 3 },
	},
	right: {
		type: "leaf",
		value: 5,
	},
};

// 18 と表示される
console.log(calc(node));

function calc(root: Tree): number {
	// プロパティの値で型の絞込ができる！
	switch (root.type) {
		case "leaf":
			// 型は Leaf で決定！
			return root.value;
		case "add":
			// 型は Add で決定！
			return calc(root.left) + calc(root.right);
		default:
			throw new Error("unknown node");
	}
}

export { }
