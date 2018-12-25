interface Cat {
	kind: string;
	name: string;
	age: number;
}

// Catのkindプロパティの型を指す！
// つまり string である
type KindType = Cat["kind"];

// kindとageのどちらかの型なので string | number になる
type KindOrAge = Cat["kind" | "age"];
