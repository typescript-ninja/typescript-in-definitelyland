interface String {
	trimStart(): string;
}

let str = "  TypeScript  ";

// 文字列先頭の空白文字を削る
console.log(str.trimStart());
