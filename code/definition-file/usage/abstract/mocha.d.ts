interface MochaDone {
	(error?: Error): void;
}

declare var describe: {
	(description: string, spec: () => void): void;
	only(description: string, spec: () => void): void;
	skip(description: string, spec: () => void): void;
};

declare var it: {
	(expectation: string, assertion?: (done: MochaDone) => void): void;
	only(expectation: string, assertion?: (done: MochaDone) => void): void;
	skip(expectation: string, assertion?: (done: MochaDone) => void): void;
};
