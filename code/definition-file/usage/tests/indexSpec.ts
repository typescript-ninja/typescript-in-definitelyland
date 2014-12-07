/// <reference path="../typings/mocha/mocha.d.ts" />
/// <reference path="../typings/power-assert/power-assert.d.ts" />

import assert = require("power-assert");

import lib = require("../lib/index");

describe("lib", ()=> {
    describe("hello function", ()=> {
        it("generate string with default value", ()=> {
            var str = lib.hello();
            assert(str === "Hello, TypeScript");
        });
        it("generate string with parameter", ()=> {
            var str = lib.hello("JavaScript");
            assert(str === "Hello, JavaScript");
        });
    });
});
