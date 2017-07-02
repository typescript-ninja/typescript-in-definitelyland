interface Point {
	x: number;
	y: number;
	color?: string; // なくてもいい
}
function printPoint(p: Point): void {
	let message = `x=${p.x}, y=${p.y}`;
	if (p.color) {
		message = `${p.color}(${message})`;
	}
	console.log(message);
}

// optinal なプロパティはなくても大丈夫！
printPoint({
	x: 1,
	y: 2,
});

printPoint({
	x: 1,
	y: 2,
	color: "red", // color があるパターン 型が一致していないともちろん怒られる
});
