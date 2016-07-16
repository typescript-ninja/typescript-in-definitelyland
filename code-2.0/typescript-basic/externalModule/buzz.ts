function bye(word = "TypeScript") {
	return `Good bye, ${word}`;
}
// foo.ts でECMAScript 2015形式でimportする時に次のエラーが出るのを抑制するためのハック
// error TS2497: Module '"略/buzz"' resolves to a non-module entity and cannot be imported using this construct.
namespace bye{}

// CommonJS向け ECMAScript 2015では×
export = bye;
