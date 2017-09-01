function returnByPromise(word: string) {
	return new Promise(resolve => {
		setTimeout(() => {
			resolve(word);
		}, 100);
	});
}

// async functionの返り値の型は必ずPromiseになる
async function helloAsync(): Promise<string> {
	// この関数は実行すると A, TypeScript, B が順番に表示される

	console.log("A");
	// Promiseな値をawaitすると中身が取り出せる（ように見える）
	const word = await returnByPromise("TypeScript");
	console.log(word);
	console.log("B");

	return `Hello, ${word}`;
}

// awaitが使えるのは必ずasync functionの中
(async () => {
	const hello = await helloAsync();
	console.log(hello);
})();

// 普通にPromiseとして扱うこともできる
helloAsync().then(hello => console.log(hello));

export { }
