/// <reference path="./sample2.d.ts" />

// fは"foo"のFooそのものだよ！
import f = require("foo");

// sample2-usage-invalid.ts(7,1): error TS2304: Cannot find name 'f'.
f.num;

// この書き方は正しい
import Foo = require("foo");
var foo: Foo;
foo.num;
