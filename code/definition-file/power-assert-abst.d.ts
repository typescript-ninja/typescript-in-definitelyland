declare function assert(value: any, message?: string): void;
declare module assert {

    export function deepEqual(actual: any, expected: any, message?: string): void;
    export function notDeepEqual(acutal: any, expected: any, message?: string): void;

    export interface Options {
        assertion?: any;
        output?: any;
    }

    export function customize(options: Options): typeof assert;
}
