declare class Sample {
	// 返り値の型を指定し忘れている！
	// error TS7010: 'method', which lacks return-type annotation,
	//               implicitly has an 'any' return type.
	method();
}

// 仮引数wordに型注釈がない
// error TS7006: Parameter 'word' implicitly has an 'any' type.
function hi(word) {
	word = word || "TypeScript";
	console.log(`Hello, ${word}`);
}

export { }
