interface Cat {
	kind: string;
	name: string;
	age?: number;
}

let c1: Cat = {
	kind: "NorwegianForestCat",
	name: "アルファ",
};

let c2: Partial<Cat> = {
	kind: "NorwegianForestCat",
	// name, age が欠けていてもエラーにならない
};

let c3: Readonly<Cat> = {
	kind: "NorwegianForestCat",
	name: "アルファ",
	age: 3,
};
// readonly なので代入しようとするとエラーになる
// error TS2540: Cannot assign to 'name' 
//   because it is a constant or a read-only property.
// c3.kind = "TypeScript";

let c4: Pick<Cat, "name" | "age"> = {
	// kind は K に含まれていないので不要
	name: "アルファ",
	// age はもともとOptional
};

let c5: Record<keyof Cat, boolean> = {
	// 全てのプロパティの型はbooleanを要求される
	kind: true,
	name: true,
	age: true, // 必須になる
};

export { c1, c2, c3, c4, c5 }
