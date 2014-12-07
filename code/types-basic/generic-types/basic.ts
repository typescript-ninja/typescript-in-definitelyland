// string[] は実は Array<string> と同じ意味なのだ！(Arrayだけ特別に！
var strArray: Array<string> = ["a", "b", "c"];
var numArray: Array<number> = [1, 2, 3];

// ArrayのメソッドとしてforEachがある
// forEachで渡される値の型はそれぞれ違う(forEachは汎用的だ！
strArray.forEach(v => v.charAt(0));
numArray.forEach(v => v.toFixed(2));

// 頑張ってこうやって書いてもいいけど、めんどいよね
strArray.forEach((v: string) => v.charAt(0));
numArray.forEach((v: number) => v.toFixed(2));

// あと、間違った型を指定した時にエラーにならないとこわい…
// error TS2345: Argument of type '(v: RegExp) => boolean' is not assignable to
// parameter of type '(value: string, index: number, array: string[]) => void'.
// strArray.forEach((v: RegExp) => v.test("str"));
