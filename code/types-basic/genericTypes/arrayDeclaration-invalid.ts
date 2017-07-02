interface ArrayConstructor {
	new <T>(...items: T[]): T[];
};

interface Array<T> {
	length: number;
	push(...items: T[]): number;
	pop(): T | undefined;
    indexOf(searchElement: T, fromIndex?: number): number;
	[n: number]: T;
}
