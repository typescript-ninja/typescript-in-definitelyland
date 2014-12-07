// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

var c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
module alternative {
    class Point {
        constructor(public x: number, public y: number) {}
    }
    class Circle {
        constructor(public p: Point, public r: number) {}
    }
    var c: Circle = new Circle(new Point(1, 2), 3);
}
