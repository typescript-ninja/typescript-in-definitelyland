declare var Array: {
    new (...items: string[]): string[];
};

interface Array {
    length: number;
    push(...items: string[]): number;
    pop(): string;
    forEach(callbackfn: (value: string) => void, thisArg?: any): void;
    [n: number]: string;
}
