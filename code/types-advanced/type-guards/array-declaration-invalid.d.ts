// lib.d.tsから抜粋 本当はもうちょっと色々ある
declare var Array: {
	new (arrayLength?: number): any[];
	(arrayLength?: number): any[];
	isArray(arg: any): boolean;
	prototype: Array<any>;
};

interface Array<T> {
	length: number;
	push(...items: T[]): number;
	pop(): T;
}
