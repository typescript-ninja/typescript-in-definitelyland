function upperAll(words: string | string[]) {
	if (typeof words === "string") {
		// string なら string[] に変換する
		words = [words];
	}

	// この時点ではwordsはstring[]に揃えられる
	return words.map(word => word.toUpperCase());
}

console.log(upperAll("TypeScript"));
console.log(upperAll(["TypeScript", "JavaScript"]));

export { }
