// 指定した型の全ブロパティを省略可能にする
type Partial<T> = {
	[P in keyof T]?: T[P];
};

// 指定した型の全プロパティをreadonly扱いにする
type Readonly<T> = {
	readonly [P in keyof T]: T[P];
};

// 指定した型の、指定したプロパティだけを集めたサブセット型を作る
type Pick<T, K extends keyof T> = {
	[P in K]: T[P];
};

// 指定した型のプロパティの値の型を変換した型を作る
type Record<K extends string, T> = {
	[P in K]: T;
};

export { Partial, Readonly, Pick, Record }
