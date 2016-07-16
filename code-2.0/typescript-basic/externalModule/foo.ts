// defaultをbarに hello関数をそのままimport
import bar, {hello} from "./bar";
// モジュール全体をbar2に束縛
import * as bar2 from "./bar";
// ECMAScript 2015形式のモジュールでもCommonJS形式でimportできる
import bar3 = require("./bar");

// Hello, TypeScript! と表示される
console.log(hello());
// Hi!, default と表示される
console.log(bar());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar2.hello());
console.log(bar2.default());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar3.hello());
console.log(bar3.default());

// export = xxx 形式の場合モジュール全体をbuzzに束縛 
import * as buzz from "./buzz";   
// CommonJS形式のモジュールに対して一番素直で真っ当な書き方 in TypeScript
import buzz2 = require("./buzz");
// 両方 Good bye, TypeScript! と表示される
console.log(buzz());
console.log(buzz2());
