async function main() {
	// 動的にモジュールをimportできる Promiseが返ってくる
	// 即値（文字列リテラル）でモジュール名を指定するとちゃんと型がついてる！
	const sub = await import("./sub");
	console.log(sub.hello());
}

function mainAnother() {
	// こういうのも普通にイケる
	import("./sub").then(sub => {
		console.log(sub.hello());
	});
}

main();
