// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

let c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
namespace alternative {
	class Point {
		constructor(public x: number, public y: number) {
		}
	}
	class Circle {
		constructor(public p: Point, public r: number) {
		}
	}
	let c2: Circle = new Circle(new Point(1, 2), 3);
	console.log(c2.p, c2.r);
}

export { Point, Circle, c, alternative }
