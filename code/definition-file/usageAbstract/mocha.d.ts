interface MochaDone {
	(error?: Error): void;
}
declare let describe: {
	(description: string, spec: () => void): any;
};
declare let it: {
	(expectation: string, assertion?: () => void): any;
	(expectation: string, assertion?: (done: MochaDone) => void): any;
};
