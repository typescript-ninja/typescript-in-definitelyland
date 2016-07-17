interface ArrayConstructor {
	new (...items: string[]): string[];
};

interface Array {
	length: number;
	push(...items: string[]): number;
	pop(): string | undefined;
	forEach(callbackfn: (value: string) => void, thisArg?: any): void;
	[n: number]: string;
}
