interface Cat {
	kind: string;
	name: string;
	age: number;
}

type PCat = Pick<Cat, keyof Cat>;
