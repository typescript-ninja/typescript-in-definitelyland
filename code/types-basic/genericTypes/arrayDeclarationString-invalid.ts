interface ArrayConstructor {
	new (...items: string[]): string[];
};

interface Array {
	length: number;
	push(...items: string[]): number;
	pop(): string | undefined;
	indexOf(searchElement: string, fromIndex?: number): number;
	[n: number]: string;
}
