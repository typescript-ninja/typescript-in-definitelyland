interface Point {
	x: number;
	y: number;
	color?: string; // なくてもいい
}
function printPoint(p: Point): void {
	var message = "x=" + p.x + ", y=" + p.y;
	if (p.color) {
		message = p.color + "(" + message + ")";
	}
	console.log(message);
}
// optinal なプロパティはなくても大丈夫！
// x=1, y=2 と表示される
printPoint({
	x: 1,
	y: 2
});
// red(x=1, y=2) と表示される
printPoint({
	x: 1,
	y: 2,
	color: "red"
});
// とはいえ、型が不一致だと怒られる。
// error TS2345: Argument of type '{ x: number; y: number; color: boolean; }'
//   is not assignable to parameter of type 'Point'.
//   Types of property 'color' are incompatible.
//     Type 'boolean' is not assignable to type 'string'.
// printPoint({
//   x: 1,
//   y: 2,
//   color: true
// });
