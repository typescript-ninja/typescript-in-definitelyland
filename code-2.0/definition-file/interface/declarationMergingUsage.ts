/// <reference path="./declarationMerging.d.ts" />
// ↑ 昔はこのようにreferece commentを使ってファイル間の依存関係を明示していましたが、
//   最近はtsconfig.jsonに全ての依存関係を書くようにしたため見かける事が大変少なくなりました

let foo: Foo = null as any;

foo.hello();
foo.bye();

export { }
