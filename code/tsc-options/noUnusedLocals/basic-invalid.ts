// importした後、一回も使わないのはエラー
// error TS6133: 'readFile' is declared but never used.
import { readFile } from "fs";

// 1回も参照されていないとエラーになる
// error TS6133: 'objA' is declared but never used.
let objA = {};

// どこかで参照されていればOK
let objB = {};
export { objB }

// exportしていればどこかで使われるかもしれないからOK
export let objC = {};
