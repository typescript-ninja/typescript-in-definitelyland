declare function assert(value: any, message?: string): void;
declare namespace assert {

	export function deepEqual(actual: any, expected: any): void;
	export function notDeepEqual(acutal: any, expected: any): void;

	export interface Options {
		assertion?: any;
		output?: any;
	}

	export function customize(options: Options): typeof assert;
}
