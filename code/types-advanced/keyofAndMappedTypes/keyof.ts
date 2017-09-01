interface Cat {
	kind: string;
	name: string;
	age: number;
}

// 全プロパティのキー名
// つまり "kind" | "name" | "age" になる
type CatPropertyNames = keyof Cat;
