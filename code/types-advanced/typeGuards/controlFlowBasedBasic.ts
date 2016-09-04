function upperA(word?: string) {
	// wordは省略可能引数なので string | undefined
	// ここでwordをいきなり使おうとするとエラーになる
	// Object is possibly 'undefined'.
	// word.toUpperCase();

	if (word == null) { // word が null か undefined の時
		// undefinedの可能性をstringで上書き！
		word = "TypeScript";
	}

	// undefinedの可能性を潰したのでこの時点でwordはstring確定！
	console.log(word.toUpperCase());
}

function upperB(word?: string) {
	// 別解：JSで || 演算子は最初にtruthyになった値を返す
	// ので、undefined（falsy）な時は "TypeScript" で上書きされる
	word = word || "TypeScript";

	// undefinedの可能性を潰したのでこの時点でwordはstring確定！
	console.log(word.toUpperCase());
}

function upperC(word = "TypeScript") {
	// TypeScript的に一番素直なパターン
	console.log(word.toUpperCase());
}

export { upperA, upperB, upperC }
