declare var Array: {
    new <T>(...items: T[]): T[];
};

interface Array<T> {
    length: number;
    push(...items: T[]): number;
    pop(): T;
    forEach(callbackfn: (value: T) => void, thisArg?: any): void;
    [n: number]: T;
}
