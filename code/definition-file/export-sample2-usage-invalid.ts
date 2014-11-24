/// <reference path="./export-sample2.d.ts" />

// f は "foo" の Fooそのもの だよ！
import f = require("foo");

// export-sample2-usage-invalid.ts(7,1): error TS2304: Cannot find name 'f'.
f.num;

// この書き方は正しい
import Foo = require("foo");
var foo: Foo;
foo.num;
