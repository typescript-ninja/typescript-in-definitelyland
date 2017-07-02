/// <reference path="./declarationMerging.d.ts" />
// ↑ 昔はこのようにreference commentを使ってファイル間の依存関係を明示していましたが、
//   最近はtsconfig.jsonに依存関係を書くため見かけることはほぼなくなりました

let foo: Foo = null as any;

foo.hello();
foo.bye();

export { }
