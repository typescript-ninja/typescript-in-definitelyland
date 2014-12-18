function move(
			value: { x: number; y: number; },
			delta: { dx?: number; dy?: number; }
		): { x: number; y: number } {
	if (delta.dx) {
		value.x += delta.dx;
	}
	if (delta.dy) {
		value.y += delta.dy;
	}
	return value;
}

var result = move({ x: 1, y: 2 }, { dx: -2 });
// 次のように表示される
// {
//   "x": -1,
//   "y": 2
// }
console.log(JSON.stringify(result, null, 2));
