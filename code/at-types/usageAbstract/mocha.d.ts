interface MochaDone {
	(error?: any): void;
}
declare let describe: {
	(description: string, callback: () => void): any;
};
declare let it: {
	(expectation: string, callback?: () => void): any;
	(expectation: string, callback?: (done: MochaDone) => void): any;
};
