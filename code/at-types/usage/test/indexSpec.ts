import * as assert from "power-assert";

import { hello } from "../lib/";

describe("lib", () => {
	describe("hello function", () => {
		it("generate string with default value", () => {
			let str = hello();
			assert(str === "Hello, TypeScript");
		});
		it("generate string with parameter", () => {
			let str = hello("JavaScript");
			assert(str === "Hello, JavaScript");
		});
	});
});
