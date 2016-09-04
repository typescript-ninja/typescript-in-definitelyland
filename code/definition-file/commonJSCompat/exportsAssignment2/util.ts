function hello(word = "TypeScript") {
	console.log(`Hello, ${word}`);
}
// 呼び出し元でエラーになるのを防ぐ 同名のnamespaceを被せてごまかす
// error TS2497: Module '"略/util"' resolves to a non-module entity
//   and cannot be imported using this construct.
namespace hello { }

export = hello;
