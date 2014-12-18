var obj: number;
if (typeof obj === "string") {
	// numberをstringに"狭める"ことはできない…
	// コンパイル通っちゃう
	obj.toFixed(0);
}
