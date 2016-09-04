interface ArrayConstructor {
	new <T>(...items: T[]): T[];
};

interface Array<T> {
	length: number;
	push(...items: T[]): number;
	pop(): T | undefined;
	forEach(callbackfn: (value: T) => void, thisArg?: any): void;
	[n: number]: T;
}
