var obj: number;
if (typeof obj === "string") {
	// number を string に"狭める"ことはできない…
	// コンパイル通っちゃう
	obj.toFixed(0);
}
