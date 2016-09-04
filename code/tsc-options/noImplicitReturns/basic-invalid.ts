// returnがない（暗黙的にundefinedが返る）パターンを検出してくれる
// error TS7030: Not all code paths return a value.
function a(v: number) {
	if (v < 0) {
		return "negative";
	} else if (0 < v) {
		return "positive";
	}

	// return がない！
}

function b() {
	// そもそも常にreturnがないならOK
}

export { }
