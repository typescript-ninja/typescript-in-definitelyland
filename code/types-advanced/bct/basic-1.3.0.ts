// {}[] でございます。
var array = [1, true];

// No best common type exists among return expressions. と怒られます。
// つまり 返り値が {} に推論されたので怒られた。
function test() {
	if (Math.random() < 0.5) {
		return 1;
	} else {
		return true;
	}
}
