function hello(word = "TypeScript") {
	console.log(`Hello, ${word}`);
}

// CommonJSの exports.module = hello; 相当
// 外からこのモジュールを参照した時のオブジェクト自体を差し替える
export = hello;
